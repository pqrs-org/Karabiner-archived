#import "ClientForKernelspace.h"
#import "StatusWindow.h"
#import "UserClient_userspace.h"
#import "WorkSpaceData.h"

@implementation ClientForKernelspace

@synthesize userClient_userspace;
@synthesize statusWindow;
@synthesize workSpaceData;

static void callback_NotificationFromKext(void* refcon, IOReturn result, uint32_t type, uint32_t option)
{
  ClientForKernelspace* self = (ClientForKernelspace*)(refcon);

  switch (type) {
    case BRIDGE_USERCLIENT_NOTIFICATION_TYPE_STATUS_MESSAGE_UPDATED:
    {
      char buf[512];

      struct BridgeUserClientStruct bridgestruct;
      bridgestruct.type   = BRIDGE_USERCLIENT_TYPE_GET_STATUS_MESSAGE;
      bridgestruct.option = option;
      bridgestruct.data   = (uintptr_t)(buf);
      bridgestruct.size   = sizeof(buf);

      if (! [[self userClient_userspace] synchronized_communication:&bridgestruct]) return;

      [[self statusWindow] setStatusMessage:option message:[NSString stringWithUTF8String:buf]];
      break;
    }

    case BRIDGE_USERCLIENT_NOTIFICATION_TYPE_CHANGE_INPUT_SOURCE:
      [[self workSpaceData] selectInputSource:option];
      break;
  }
}

- (id) init
{
  self = [super init];

  if (self) {
    asyncref_[kIOAsyncCalloutFuncIndex] = (io_user_reference_t)(callback_NotificationFromKext);
    asyncref_[kIOAsyncCalloutRefconIndex] = (io_user_reference_t)(self);

    userClient_userspace = [[UserClient_userspace alloc] init:&asyncref_];
  }

  return self;
}

- (void) dealloc
{
  [userClient_userspace release];

  [super dealloc];
}

- (void) refresh_connection_with_retry
{
  [userClient_userspace refresh_connection_with_retry:10 wait:0.5];
}

- (void) disconnect_from_kext
{
  [userClient_userspace disconnect_from_kext];
}

- (void) send_remapclasses_initialize_vector_to_kext
{
  const uint32_t* p = [xmlCompiler_ remapclasses_initialize_vector_data];
  size_t size = [xmlCompiler_ remapclasses_initialize_vector_size] * sizeof(uint32_t);

  // --------------------
  struct BridgeUserClientStruct bridgestruct;
  bridgestruct.type   = BRIDGE_USERCLIENT_TYPE_SET_REMAPCLASSES_INITIALIZE_VECTOR;
  bridgestruct.option = 0;
  bridgestruct.data   = (uintptr_t)(p);
  bridgestruct.size   = size;

  [userClient_userspace synchronized_communication:&bridgestruct];
}

- (void) send_config_to_kext
{
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
    bridgestruct.type   = BRIDGE_USERCLIENT_TYPE_SET_CONFIG;
    bridgestruct.option = 0;
    bridgestruct.data   = (uintptr_t)(data);
    bridgestruct.size   = size;

    [userClient_userspace synchronized_communication:&bridgestruct];

    free(data);
  }
}

- (void) configXMLReloaded
{
  [self send_remapclasses_initialize_vector_to_kext];
  [self send_config_to_kext];
}

- (void) preferencesChanged
{
  [self send_config_to_kext];
}

- (void) send_workspacedata_to_kext:(struct BridgeWorkSpaceData*)bridgeworkspacedata
{
  struct BridgeUserClientStruct bridgestruct;
  bridgestruct.type   = BRIDGE_USERCLIENT_TYPE_SET_WORKSPACEDATA;
  bridgestruct.option = 0;
  bridgestruct.data   = (uintptr_t)(bridgeworkspacedata);
  bridgestruct.size   = sizeof(*bridgeworkspacedata);

  [userClient_userspace synchronized_communication:&bridgestruct];
}

- (NSArray*) device_information:(NSInteger)type
{
  NSMutableArray* information = [[NSMutableArray new] autorelease];

  for (size_t i = 0;; ++i) {
    struct BridgeDeviceInformation deviceInformation;

    struct BridgeUserClientStruct bridgestruct;
    bridgestruct.type   = (uint32_t)(type);
    bridgestruct.option = i;
    bridgestruct.data   = (uintptr_t)(&deviceInformation);
    bridgestruct.size   = sizeof(deviceInformation);

    if (! [userClient_userspace synchronized_communication:&bridgestruct]) break;

    if (! deviceInformation.isFound) break;

    NSDictionary* newdict = [NSDictionary dictionaryWithObjectsAndKeys:
                             [NSString stringWithUTF8String:deviceInformation.manufacturer], @"manufacturer",
                             [NSString stringWithUTF8String:deviceInformation.product], @"product",
                             [NSString stringWithFormat:@"0x%x", deviceInformation.vendorID], @"vendorID",
                             [NSString stringWithFormat:@"0x%x", deviceInformation.productID], @"productID",
                             [NSString stringWithFormat:@"0x%x", deviceInformation.locationID], @"locationID",
                             nil];

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
