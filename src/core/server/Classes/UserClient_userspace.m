#import "UserClient_userspace.h"
#import "WorkSpaceData.h"
#include "bridge.h"

@implementation UserClient_userspace

@synthesize connected;
@synthesize statusWindow;
@synthesize workSpaceData;

static void callback_NotificationFromKext(void* refcon, IOReturn result, uint32_t type, uint32_t option)
{
  UserClient_userspace* self = (UserClient_userspace*)(refcon);

  switch (type) {
    case BRIDGE_USERCLIENT_NOTIFICATION_TYPE_STATUS_MESSAGE_UPDATED:
    {
      char buf[512];

      struct BridgeUserClientStruct bridgestruct;
      bridgestruct.type   = BRIDGE_USERCLIENT_TYPE_GET_STATUS_MESSAGE;
      bridgestruct.option = option;
      bridgestruct.data   = (uintptr_t)(buf);
      bridgestruct.size   = sizeof(buf);

      if (! [self synchronized_communication:&bridgestruct]) return;

      [[self statusWindow] setStatusMessage:option message:[NSString stringWithUTF8String:buf]];
      break;
    }

    case BRIDGE_USERCLIENT_NOTIFICATION_TYPE_CHANGE_INPUT_SOURCE:
      [[self workSpaceData] selectInputSource:option];
      break;
  }
}

- (void) closeUserClient
{
  // ----------------------------------------
  // call BRIDGE_USERCLIENT_CLOSE
  if (service_ != IO_OBJECT_NULL && connect_ != IO_OBJECT_NULL) {
    // BRIDGE_USERCLIENT_CLOSE may be failed. (when kext is unloaded, etc.)
    // So we don't output a log message when it is failed.
    IOConnectCallScalarMethod(connect_, BRIDGE_USERCLIENT_CLOSE, NULL, 0, NULL, NULL);
  }

  // ----------------------------------------
  // release previous values.
  if (connect_ != IO_OBJECT_NULL) {
    IOServiceClose(connect_);
    connect_ = IO_OBJECT_NULL;
  }
  if (service_ != IO_OBJECT_NULL) {
    IOObjectRelease(service_);
    service_ = IO_OBJECT_NULL;
  }
}

- (void) openUserClient
{
  io_iterator_t iterator;

  kern_return_t kernResult = IOServiceGetMatchingServices(kIOMasterPortDefault, IOServiceMatching("org_pqrs_driver_KeyRemap4MacBook"), &iterator);
  if (kernResult != KERN_SUCCESS) {
    NSLog(@"[ERROR] IOServiceGetMatchingServices returned 0x%08x\n\n", kernResult);
    return;
  }

  for (;;) {
    io_service_t s = IOIteratorNext(iterator);
    if (s == IO_OBJECT_NULL) break;

    [self closeUserClient];

    // ----------------------------------------
    // setup service_
    service_ = s;
    kernResult = IOObjectRetain(service_);
    if (kernResult != KERN_SUCCESS) {
      NSLog(@"[ERROR] IOObjectRetain returned 0x%08x\n", kernResult);
      continue;
    }

    // ----------------------------------------
    // setup connect_
    kernResult = IOServiceOpen(service_, mach_task_self(), 0, &connect_);
    if (kernResult != KERN_SUCCESS) {
      NSLog(@"[ERROR] IOServiceOpen returned 0x%08x\n", kernResult);
      continue;
    }

    // ----------------------------------------
    // open
    kernResult = IOConnectCallScalarMethod(connect_, BRIDGE_USERCLIENT_OPEN, NULL, 0, NULL, NULL);
    if (kernResult != KERN_SUCCESS) {
      NSLog(@"[ERROR] BRIDGE_USERCLIENT_OPEN returned 0x%08x\n", kernResult);
      continue;
    }

    // ----------------------------------------
    // set notification
    if (notifyport_) {
      io_async_ref64_t asyncRef;
      asyncRef[kIOAsyncCalloutFuncIndex] = (io_user_reference_t)(callback_NotificationFromKext);
      asyncRef[kIOAsyncCalloutRefconIndex] = (io_user_reference_t)(self);

      kernResult = IOConnectCallAsyncScalarMethod(connect_,
                                                  BRIDGE_USERCLIENT_NOTIFICATION_FROM_KEXT,
                                                  IONotificationPortGetMachPort(notifyport_),
                                                  asyncRef,
                                                  kOSAsyncRef64Count,
                                                  NULL,                // input
                                                  0,                   // inputCnt
                                                  NULL,                // output
                                                  NULL);               // outputCnt
      if (kernResult != KERN_SUCCESS) {
        NSLog(@"[ERROR] BRIDGE_USERCLIENT_NOTIFICATION_FROM_KEXT returned 0x%08x\n", kernResult);
        continue;
      }

      connected = YES;
    }
  }

  IOObjectRelease(iterator);
}

- (id) init
{
  self = [super init];

  if (self) {
    service_ = IO_OBJECT_NULL;
    connect_ = IO_OBJECT_NULL;
    connected = NO;
  }

  return self;
}

- (void) dealloc
{
  [super dealloc];
}

// ======================================================================
- (void) connect_to_kext
{
  @synchronized(self) {
    service_ = IO_OBJECT_NULL;
    connect_ = IO_OBJECT_NULL;
    connected = NO;

    // ----------------------------------------
    // setup IONotification
    notifyport_ = IONotificationPortCreate(kIOMasterPortDefault);
    if (! notifyport_) {
      NSLog(@"[ERROR] IONotificationPortCreate failed\n");

    } else {
      loopsource_ = IONotificationPortGetRunLoopSource(notifyport_);
      if (! loopsource_) {
        NSLog(@"[ERROR] IONotificationPortGetRunLoopSource failed\n");

      } else {
        CFRunLoopAddSource(CFRunLoopGetCurrent(), loopsource_, kCFRunLoopDefaultMode);
        [self openUserClient];
      }
    }

    if (! connected) {
      [self closeUserClient];
    }
  }
}

- (void) disconnect_from_kext
{
  @synchronized(self) {
    [self closeUserClient];

    if (notifyport_) {
      if (loopsource_) {
        CFRunLoopSourceInvalidate(loopsource_);
        loopsource_ = nil;
      }
      IONotificationPortDestroy(notifyport_);
      notifyport_ = nil;
    }
  }
}

- (void) refresh_connection
{
  [self disconnect_from_kext];
  [self connect_to_kext];
}

- (BOOL) do_synchronized_communication:(struct BridgeUserClientStruct*)bridgestruct
{
  if (connect_ == IO_OBJECT_NULL) return NO;
  if (! bridgestruct) return NO;

  uint64_t output = 0;
  uint32_t outputCnt = 1;
  kern_return_t kernResult = IOConnectCallMethod(connect_,
                                                 BRIDGE_USERCLIENT_SYNCHRONIZED_COMMUNICATION,
                                                 NULL, 0,                             // scalar input
                                                 bridgestruct, sizeof(*bridgestruct), // struct input
                                                 &output, &outputCnt,                 // scalar output
                                                 NULL, NULL);                         // struct output
  if (kernResult != KERN_SUCCESS) {
    NSLog(@"[ERROR] BRIDGE_USERCLIENT_SYNCHRONIZED_COMMUNICATION returned 0x%08x\n", kernResult);
    return NO;
  }
  if (output != BRIDGE_USERCLIENT_SYNCHRONIZED_COMMUNICATION_RETURN_SUCCESS) {
    NSLog(@"[ERROR] BRIDGE_USERCLIENT_SYNCHRONIZED_COMMUNICATION output is not SUCCESS (%lld)\n", output);
    return NO;
  }

  return YES;
}

- (BOOL) synchronized_communication:(struct BridgeUserClientStruct*)bridgestruct
{
  BOOL retval = NO;

  @synchronized(self) {
    if ([self do_synchronized_communication:bridgestruct]) {
      retval = YES;
    }
  }

  return retval;
}

@end
