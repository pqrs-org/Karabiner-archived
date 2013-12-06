#import "ClientForKernelspace.h"
#import "EnvironmentChecker.h"
#import "IOHIDPostEventWrapper.h"
#import "NotificationKeys.h"
#import "PreferencesManager.h"
#import "StatusWindow.h"
#import "UserClient_userspace.h"
#import "WorkSpaceData.h"

@interface ClientForKernelspace ()
{
  io_async_ref64_t asyncref_;
  UserClient_userspace* userClient_userspace_;
}
@end

@implementation ClientForKernelspace

- (void) callback_NotificationFromKext:(uint32_t)type option:(uint32_t)option
{
  dispatch_async(dispatch_get_main_queue(), ^{
    switch (type) {
      case BRIDGE_USERCLIENT_NOTIFICATION_TYPE_CONFIG_ENABLED_UPDATED:
        {
          uint32_t enabled = 0;

          struct BridgeUserClientStruct bridgestruct;
          bridgestruct.type   = BRIDGE_USERCLIENT_TYPE_GET_CONFIG_ENABLED;
          bridgestruct.option = option;
          bridgestruct.data   = (user_addr_t)(&enabled);
          bridgestruct.size   = sizeof(enabled);

          if (! [userClient_userspace_ synchronized_communication:&bridgestruct]) return;

          uint32_t configindex = option;
          NSString* name = [xmlCompiler_ identifier:(int)(configindex)];
          if (name) {
            // Do not call set_config_one here. (== Call setValue with tellToKext:NO.)
            [preferencesManager_ setValue:enabled forName:name tellToKext:NO];
          }
          break;
        }

      case BRIDGE_USERCLIENT_NOTIFICATION_TYPE_STATUS_MESSAGE_UPDATED:
        {
          char buf[512];

          struct BridgeUserClientStruct bridgestruct;
          bridgestruct.type   = BRIDGE_USERCLIENT_TYPE_GET_STATUS_MESSAGE;
          bridgestruct.option = option;
          bridgestruct.data   = (user_addr_t)(buf);
          bridgestruct.size   = sizeof(buf);

          if (! [userClient_userspace_ synchronized_communication:&bridgestruct]) return;

          [statusWindow_ setStatusMessage:option message:@(buf)];
          break;
        }

      case BRIDGE_USERCLIENT_NOTIFICATION_TYPE_CHANGE_INPUT_SOURCE:
        [workSpaceData_ selectInputSource:option];
        break;

      case BRIDGE_USERCLIENT_NOTIFICATION_TYPE_OPEN_URL:
        {
          NSString* url = [xmlCompiler_ url:option];
          if (url) {
            NSString* urlType = [xmlCompiler_ urlType:option];

            if ([urlType isEqualToString:@"shell"]) {
              [[NSTask launchedTaskWithLaunchPath:@"/bin/sh" arguments:@[@"-c", url]] waitUntilExit];
            } else if ([urlType isEqualToString:@"file"]) {
              [[NSWorkspace sharedWorkspace] openFile:url];
            } else {
              [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:url]];
            }
          }
          break;
        }

      case BRIDGE_USERCLIENT_NOTIFICATION_TYPE_IOHIDPOSTEVENT:
        [iohidPostEventWrapper_ postKey:option];
        break;
    }
  });
}

static void static_callback_NotificationFromKext(void* refcon, IOReturn result, uint32_t type, uint32_t option)
{
  ClientForKernelspace* self = (__bridge ClientForKernelspace*)(refcon);
  [self callback_NotificationFromKext:type option:option];
}

- (void) observer_ConfigXMLReloaded:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    [self send_remapclasses_initialize_vector_to_kext];
    [preferencesManager_ clearNotSave];
    [self send_config_to_kext];
    [self set_initialized];
  });
}

- (void) observer_ConfigListChanged:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    [preferencesManager_ clearNotSave];
    [self send_config_to_kext];
    [self set_initialized];
  });
}

- (id) init
{
  self = [super init];

  if (self) {
    asyncref_[kIOAsyncCalloutFuncIndex] = (io_user_reference_t)(static_callback_NotificationFromKext);
    asyncref_[kIOAsyncCalloutRefconIndex] = (io_user_reference_t)(self);

    userClient_userspace_ = [[UserClient_userspace alloc] init:&asyncref_];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_ConfigXMLReloaded:)
                                                 name:kConfigXMLReloadedNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_ConfigListChanged:)
                                                 name:kConfigListChangedNotification object:nil];
  }

  return self;
}

- (void) dealloc
{
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void) refresh_connection_with_retry
{
  // Try one minute
  [userClient_userspace_ refresh_connection_with_retry:120 wait:0.5];
  [self observer_ConfigXMLReloaded:nil];
}

- (void) disconnect_from_kext
{
  [userClient_userspace_ disconnect_from_kext];
}

- (void) send_remapclasses_initialize_vector_to_kext
{
  const uint32_t* p = [xmlCompiler_ remapclasses_initialize_vector_data];
  size_t size = [xmlCompiler_ remapclasses_initialize_vector_size] * sizeof(uint32_t);

  // --------------------
  struct BridgeUserClientStruct bridgestruct;
  bridgestruct.type   = BRIDGE_USERCLIENT_TYPE_SET_REMAPCLASSES_INITIALIZE_VECTOR;
  bridgestruct.option = 0;
  bridgestruct.data   = (user_addr_t)(p);
  bridgestruct.size   = size;

  [userClient_userspace_ synchronized_communication:&bridgestruct];
}

- (void) send_config_to_kext
{
  // ------------------------------------------------------------
  // Set notsave.automatically_enable_* before sending config into kext.
  {
    // set automatically_enable_keyboard_device
    int newvalue = 1;
    if ([EnvironmentChecker checkDoubleCommand]) {
      newvalue = 0;
    }
    [preferencesManager_ setValue:newvalue forName:@"notsave.automatically_enable_keyboard_device"];
  }
  {
    // set automatically_enable_pointing_device
    int newvalue = 1;
    if ([EnvironmentChecker checkSmoothMouse]) {
      newvalue = 0;
    }
    [preferencesManager_ setValue:newvalue forName:@"notsave.automatically_enable_pointing_device"];
  }

  // ------------------------------------------------------------
  NSArray* essential_config = [preferencesManager_ essential_config];
  if (! essential_config) {
    NSLog(@"[WARNING] essential_config == nil.");
    return;
  }

  // ------------------------------------------------------------
  NSUInteger essential_config_count = [essential_config count];
  NSUInteger remapclasses_count     = [xmlCompiler_ remapclasses_initialize_vector_config_count];
  size_t size = (essential_config_count + remapclasses_count) * sizeof(int32_t);
  int32_t* data = (int32_t*)(malloc(size));
  if (! data) {
    NSLog(@"[WARNING] malloc failed.");
    return;

  } else {
    int32_t* p = data;

    // --------------------
    // essential_config
    for (NSNumber* number in essential_config) {
      *p++ = [number intValue];
    }

    // --------------------
    // remapclasses config
    for (NSUInteger i = 0; i < remapclasses_count; ++i) {
      NSString* name = [xmlCompiler_ identifier:(int)(i)];
      if (! name) {
        NSLog(@"[WARNING] %s name == nil. private.xml has error?", __FUNCTION__);
        *p++ = 0;
      } else {
        *p++ = [preferencesManager_ value:name];
      }
    }

    // --------------------
    struct BridgeUserClientStruct bridgestruct;
    bridgestruct.type   = BRIDGE_USERCLIENT_TYPE_SET_CONFIG_ALL;
    bridgestruct.option = 0;
    bridgestruct.data   = (user_addr_t)(data);
    bridgestruct.size   = size;

    [userClient_userspace_ synchronized_communication:&bridgestruct];

    free(data);
  }
}

- (void) set_config_one:(struct BridgeSetConfigOne*)bridgeSetConfigOne
{
  struct BridgeUserClientStruct bridgestruct;
  bridgestruct.type   = BRIDGE_USERCLIENT_TYPE_SET_CONFIG_ONE;
  bridgestruct.option = 0;
  bridgestruct.data   = (user_addr_t)(bridgeSetConfigOne);
  bridgestruct.size   = sizeof(*bridgeSetConfigOne);

  [userClient_userspace_ synchronized_communication:&bridgestruct];
}

- (void) set_initialized
{
  uint32_t value = 1;
  struct BridgeUserClientStruct bridgestruct;
  bridgestruct.type   = BRIDGE_USERCLIENT_TYPE_SET_INITIALIZED;
  bridgestruct.option = 0;
  bridgestruct.data   = &value;
  bridgestruct.size   = sizeof(value);

  [userClient_userspace_ synchronized_communication:&bridgestruct];
}

- (void) send_workspacedata_to_kext:(struct BridgeWorkSpaceData*)bridgeworkspacedata
{
  struct BridgeUserClientStruct bridgestruct;
  bridgestruct.type   = BRIDGE_USERCLIENT_TYPE_SET_WORKSPACEDATA;
  bridgestruct.option = 0;
  bridgestruct.data   = (user_addr_t)(bridgeworkspacedata);
  bridgestruct.size   = sizeof(*bridgeworkspacedata);

  [userClient_userspace_ synchronized_communication:&bridgestruct];
}

- (NSArray*) device_information:(NSInteger)type
{
  NSMutableArray* information = [NSMutableArray new];

  for (size_t i = 0;; ++i) {
    struct BridgeDeviceInformation deviceInformation;

    struct BridgeUserClientStruct bridgestruct;
    bridgestruct.type   = (uint32_t)(type);
    bridgestruct.option = (uint32_t)(i);
    bridgestruct.data   = (user_addr_t)(&deviceInformation);
    bridgestruct.size   = sizeof(deviceInformation);

    if (! [userClient_userspace_ synchronized_communication:&bridgestruct]) break;

    if (! deviceInformation.isFound) break;

    NSDictionary* newdict = @{ @"manufacturer": @(deviceInformation.manufacturer),
                               @"product": @(deviceInformation.product),
                               @"vendorID": [NSString stringWithFormat:@"0x%04x", deviceInformation.vendorID],
                               @"productID": [NSString stringWithFormat:@"0x%04x", deviceInformation.productID],
                               @"locationID": [NSString stringWithFormat:@"0x%04x", deviceInformation.locationID] };

    // skip if newdict is already exists.
    BOOL found = NO;
    for (NSDictionary* d in information) {
      if ([newdict isEqualToDictionary:d]) {
        found = YES;
      }
    }
    if (! found) {
      [information addObject:newdict];
    }
  }

  return information;
}

@end
