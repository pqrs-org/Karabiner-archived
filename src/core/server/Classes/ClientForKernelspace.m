#import "ClientForKernelspace.h"
#import "EnvironmentChecker.h"
#import "IOHIDPostEventWrapper.h"
#import "NotificationKeys.h"
#import "PreferencesManager.h"
#import "StatusMessageManager.h"
#import "UserClient_userspace.h"
#import "WorkSpaceData.h"
#import "XMLCompiler.h"

@interface ClientForKernelspace ()

@property(weak) IBOutlet IOHIDPostEventWrapper* iohidPostEventWrapper;
@property(weak) IBOutlet PreferencesManager* preferencesManager;
@property(weak) IBOutlet StatusMessageManager* statusMessageManager;
@property(weak) IBOutlet WorkSpaceData* workSpaceData;
@property(weak) IBOutlet XMLCompiler* xmlCompiler;

@property io_async_ref64_t* asyncref;
@property UserClient_userspace* userClient_userspace;
@property NSTimer* timer;
@property int retryCounter;

@end

@implementation ClientForKernelspace

- (void)callback_NotificationFromKext:(uint32_t)type option:(uint32_t)option {
  dispatch_async(dispatch_get_main_queue(), ^{
    switch (type) {
    case BRIDGE_USERCLIENT_NOTIFICATION_TYPE_CONFIG_ENABLED_UPDATED: {
      uint32_t enabled = 0;

      struct BridgeUserClientStruct bridgestruct;
      bridgestruct.type = BRIDGE_USERCLIENT_TYPE_GET_CONFIG_ENABLED;
      bridgestruct.option = option;
      bridgestruct.data = (user_addr_t)(&enabled);
      bridgestruct.size = sizeof(enabled);

      if (![self.userClient_userspace synchronized_communication:&bridgestruct]) return;

      uint32_t configindex = option;
      NSString* name = [self.xmlCompiler identifier:(int)(configindex)];
      if (name) {
        // Do not call set_config_one here. (== Call setValue with tellToKext:NO.)
        [self.preferencesManager setValue:enabled forName:name tellToKext:NO];
      }
      break;
    }

    case BRIDGE_USERCLIENT_NOTIFICATION_TYPE_STATUS_MESSAGE_UPDATED: {
      char buf[512];

      struct BridgeUserClientStruct bridgestruct;
      bridgestruct.type = BRIDGE_USERCLIENT_TYPE_GET_STATUS_MESSAGE;
      bridgestruct.option = option;
      bridgestruct.data = (user_addr_t)(buf);
      bridgestruct.size = sizeof(buf);

      if (![self.userClient_userspace synchronized_communication:&bridgestruct]) return;

      [self.statusMessageManager setStatusMessage:option message:@(buf)];
      break;
    }

    case BRIDGE_USERCLIENT_NOTIFICATION_TYPE_CHANGE_INPUT_SOURCE:
      [self.workSpaceData selectInputSource:option];
      break;

    case BRIDGE_USERCLIENT_NOTIFICATION_TYPE_OPEN_URL: {
      NSString* url = [self.xmlCompiler url:option];
      if (url) {
        NSString* urlType = [self.xmlCompiler urlType:option];

        if ([urlType isEqualToString:@"shell"]) {
          [[NSTask launchedTaskWithLaunchPath:@"/bin/sh" arguments:@[ @"-c", url ]] waitUntilExit];
        } else if ([urlType isEqualToString:@"file"]) {
          [[NSWorkspace sharedWorkspace] openFile:url];
        } else {
          BOOL openInBackground = [self.xmlCompiler urlIsBackground:option];
          if (openInBackground) {
            NSArray* urls = [NSArray arrayWithObject:[NSURL URLWithString:url]];
            [[NSWorkspace sharedWorkspace] openURLs:urls
                            withAppBundleIdentifier:nil
                                            options:NSWorkspaceLaunchWithoutActivation
                     additionalEventParamDescriptor:nil
                                  launchIdentifiers:nil];
          } else {
            [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:url]];
          }
        }
      }
      break;
    }

    case BRIDGE_USERCLIENT_NOTIFICATION_TYPE_IOHIDPOSTEVENT:
      [self.iohidPostEventWrapper postKey:option];
      break;
    }
  });
}

static void static_callback_NotificationFromKext(void* refcon, IOReturn result, uint32_t type, uint32_t option) {
  ClientForKernelspace* self = (__bridge ClientForKernelspace*)(refcon);
  [self callback_NotificationFromKext:type option:option];
}

- (void)observer_ConfigXMLReloaded:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self send_remapclasses_initialize_vector_to_kext];
    [self.preferencesManager clearNotSave];
    [self send_config_to_kext];
    [self set_initialized];
  });
}

- (void)observer_ConfigListChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self.preferencesManager clearNotSave];
    [self send_config_to_kext];
    [self set_initialized];
  });
}

- (id)init {
  self = [super init];

  if (self) {
    self.asyncref = (io_async_ref64_t*)(malloc(sizeof(io_async_ref64_t)));
    (*(self.asyncref))[kIOAsyncCalloutFuncIndex] = (io_user_reference_t)(static_callback_NotificationFromKext);
    (*(self.asyncref))[kIOAsyncCalloutRefconIndex] = (io_user_reference_t)(self);

    self.userClient_userspace = [[UserClient_userspace alloc] init:self.asyncref];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_ConfigXMLReloaded:)
                                                 name:kConfigXMLReloadedNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_ConfigListChanged:)
                                                 name:kConfigListChangedNotification
                                               object:nil];
  }

  return self;
}

- (void)dealloc {
  [self.timer invalidate];
  [[NSNotificationCenter defaultCenter] removeObserver:self];

  if (self.asyncref) {
    free(self.asyncref);
    self.asyncref = NULL;
  }
}

- (void)refresh_connection_with_retry {
  @synchronized(self) {
    // [UserClient_userspace connect_to_kext] may fail by kIOReturnExclusiveAccess
    // when connect_to_kext is called in NSWorkspaceSessionDidBecomeActiveNotification.
    // So, we retry the connection some times.
    //
    // Try one minute
    // (There are few seconds between kext::init and registerService is called.
    // So we need to wait for a while.)

    [self.timer invalidate];
    self.timer = [NSTimer scheduledTimerWithTimeInterval:0.5
                                              target:self
                                            selector:@selector(timerFireMethod:)
                                            userInfo:nil
                                             repeats:YES];
    self.retryCounter = 0;
    [self.timer fire];
  }
}

- (void)timerFireMethod:(NSTimer*)timer {
  dispatch_async(dispatch_get_main_queue(), ^{
    @synchronized(self) {
      if (![timer isValid]) {
        // disconnect_from_kext is already called.
        // return immediately.
        return;
      }

      @try {
        if ([self.userClient_userspace refresh_connection]) {
          // connected

          [timer invalidate];
          [[NSNotificationCenter defaultCenter] postNotificationName:kConfigXMLReloadedNotification object:nil];
          return;

        } else {
          // retry

          ++(self.retryCounter);
          if (self.retryCounter > 120) {
            [timer invalidate];

            NSAlert* alert = [NSAlert new];
            [alert setMessageText:@"Karabiner Alert"];
            [alert addButtonWithTitle:@"Close"];
            [alert setInformativeText:@"Karabiner cannot connect with kernel extension.\nPlease restart your system in order to solve the problem.\n"];
            [alert runModal];

            return;
          }
        }

      } @catch (NSException* e) {
        // unrecoverable error occurred.

        [timer invalidate];

        NSAlert* alert = [NSAlert new];
        [alert setMessageText:@"Karabiner Alert"];
        [alert addButtonWithTitle:@"Close"];
        [alert setInformativeText:[e reason]];
        [alert runModal];

        return;
      }
    }
  });
}

- (void)disconnect_from_kext {
  @synchronized(self) {
    [self.timer invalidate];
    [self.userClient_userspace disconnect_from_kext];
  }
}

- (void)send_remapclasses_initialize_vector_to_kext {
  const uint32_t* p = [self.xmlCompiler remapclasses_initialize_vector_data];
  size_t size = [self.xmlCompiler remapclasses_initialize_vector_size] * sizeof(uint32_t);

  // --------------------
  struct BridgeUserClientStruct bridgestruct;
  bridgestruct.type = BRIDGE_USERCLIENT_TYPE_SET_REMAPCLASSES_INITIALIZE_VECTOR;
  bridgestruct.option = 0;
  bridgestruct.data = (user_addr_t)(p);
  bridgestruct.size = size;

  [self.userClient_userspace synchronized_communication:&bridgestruct];
}

- (void)send_config_to_kext {
  // ------------------------------------------------------------
  // Set notsave.automatically_enable_* before sending config into kext.
  {
    // set automatically_enable_keyboard_device
    int newvalue = 1;
    if ([EnvironmentChecker checkDoubleCommand]) {
      newvalue = 0;
    }
    if ([EnvironmentChecker checkKeyRemap4MacBook]) {
      newvalue = 0;
    }
    if ([EnvironmentChecker checkKirgudu]) {
      newvalue = 0;
    }
    [self.preferencesManager setValue:newvalue forName:@"notsave.automatically_enable_keyboard_device"];
  }
  {
    // set automatically_enable_pointing_device
    int newvalue = 1;
    if ([EnvironmentChecker checkKeyRemap4MacBook]) {
      newvalue = 0;
    }
    if ([EnvironmentChecker checkSmoothMouse]) {
      newvalue = 0;
    }
    [self.preferencesManager setValue:newvalue forName:@"notsave.automatically_enable_pointing_device"];
  }

  // ------------------------------------------------------------
  NSArray* essential_config = [self.preferencesManager essential_config];
  if (!essential_config) {
    NSLog(@"[WARNING] essential_config == nil.");
    return;
  }

  // ------------------------------------------------------------
  NSUInteger essential_config_count = [essential_config count];
  NSUInteger remapclasses_count = [self.xmlCompiler remapclasses_initialize_vector_config_count];
  size_t size = (essential_config_count + remapclasses_count) * sizeof(int32_t);
  int32_t* data = (int32_t*)(malloc(size));
  if (!data) {
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
      NSString* name = [self.xmlCompiler identifier:(int)(i)];
      if (!name) {
        NSLog(@"[WARNING] %s name == nil. private.xml has error?", __FUNCTION__);
        *p++ = 0;
      } else {
        *p++ = [self.preferencesManager value:name];
      }
    }

    // --------------------
    struct BridgeUserClientStruct bridgestruct;
    bridgestruct.type = BRIDGE_USERCLIENT_TYPE_SET_CONFIG_ALL;
    bridgestruct.option = 0;
    bridgestruct.data = (user_addr_t)(data);
    bridgestruct.size = size;

    [self.userClient_userspace synchronized_communication:&bridgestruct];

    free(data);
  }
}

- (void)set_config_one:(struct BridgeSetConfigOne*)bridgeSetConfigOne {
  struct BridgeUserClientStruct bridgestruct;
  bridgestruct.type = BRIDGE_USERCLIENT_TYPE_SET_CONFIG_ONE;
  bridgestruct.option = 0;
  bridgestruct.data = (user_addr_t)(bridgeSetConfigOne);
  bridgestruct.size = sizeof(*bridgeSetConfigOne);

  [self.userClient_userspace synchronized_communication:&bridgestruct];
}

- (void)set_initialized {
  uint32_t value = 1;
  struct BridgeUserClientStruct bridgestruct;
  bridgestruct.type = BRIDGE_USERCLIENT_TYPE_SET_INITIALIZED;
  bridgestruct.option = 0;
  bridgestruct.data = &value;
  bridgestruct.size = sizeof(value);

  [self.userClient_userspace synchronized_communication:&bridgestruct];
}

- (void)send_workspacedata_to_kext:(NSArray*)array {
  NSUInteger count = [array count];
  if (count > 0) {
    size_t size = count * sizeof(uint32_t);
    uint32_t* data = (uint32_t*)(malloc(size));

    for (NSUInteger i = 0; i < count; ++i) {
      data[i] = [array[i] unsignedIntValue];
    }

    struct BridgeUserClientStruct bridgestruct;
    bridgestruct.type = BRIDGE_USERCLIENT_TYPE_SET_WORKSPACEDATA;
    bridgestruct.option = 0;
    bridgestruct.data = (user_addr_t)(data);
    bridgestruct.size = size;

    [self.userClient_userspace synchronized_communication:&bridgestruct];

    free(data);
  }
}

- (NSArray*)device_information:(NSInteger)type {
  NSMutableArray* information = [NSMutableArray new];

  for (size_t i = 0;; ++i) {
    struct BridgeDeviceInformation deviceInformation;

    struct BridgeUserClientStruct bridgestruct;
    bridgestruct.type = (uint32_t)(type);
    bridgestruct.option = (uint32_t)(i);
    bridgestruct.data = (user_addr_t)(&deviceInformation);
    bridgestruct.size = sizeof(deviceInformation);

    if (![self.userClient_userspace synchronized_communication:&bridgestruct]) break;

    if (!deviceInformation.isFound) break;

    NSDictionary* newdict = @{ @"manufacturer" : @(deviceInformation.manufacturer),
                               @"product" : @(deviceInformation.product),
                               @"vendorID" : [NSString stringWithFormat:@"0x%04x", deviceInformation.vendorID],
                               @"productID" : [NSString stringWithFormat:@"0x%04x", deviceInformation.productID],
                               @"locationID" : [NSString stringWithFormat:@"0x%04x", deviceInformation.locationID] };

    // skip if newdict is already exists.
    BOOL found = NO;
    for (NSDictionary* d in information) {
      if ([newdict isEqualToDictionary:d]) {
        found = YES;
      }
    }
    if (!found) {
      [information addObject:newdict];
    }
  }

  return information;
}

- (void)unset_debug_flags {
  uint32_t dummy = 1;
  struct BridgeUserClientStruct bridgestruct;
  bridgestruct.type = BRIDGE_USERCLIENT_TYPE_UNSET_DEBUG_FLAGS;
  bridgestruct.option = 0;
  bridgestruct.data = &dummy;
  bridgestruct.size = sizeof(dummy);

  [self.userClient_userspace synchronized_communication:&bridgestruct];
}

@end
