#import "Relauncher.h"
#import "UserClient_userspace.h"
#include "bridge.h"

@interface UserClient_userspace ()

@property io_service_t service;
@property io_connect_t connect;
@property IONotificationPortRef notifyport;
@property CFRunLoopSourceRef loopsource;
@property io_async_ref64_t* asyncref;

@end

@implementation UserClient_userspace

- (void)closeUserClient {
  // ----------------------------------------
  // call BRIDGE_USERCLIENT_CLOSE
  if (self.service != IO_OBJECT_NULL && self.connect != IO_OBJECT_NULL) {
    // BRIDGE_USERCLIENT_CLOSE may be failed. (when kext is unloaded, etc.)
    // So we don't output a log message when it is failed.
    IOConnectCallScalarMethod(self.connect, BRIDGE_USERCLIENT_CLOSE, NULL, 0, NULL, NULL);
  }

  // ----------------------------------------
  // release previous values.
  if (self.connect != IO_OBJECT_NULL) {
    IOServiceClose(self.connect);
    self.connect = IO_OBJECT_NULL;
  }
  if (self.service != IO_OBJECT_NULL) {
    IOObjectRelease(self.service);
    self.service = IO_OBJECT_NULL;
  }
}

- (void)openUserClient {
  io_iterator_t iterator;

  kern_return_t kernResult = IOServiceGetMatchingServices(kIOMasterPortDefault, IOServiceMatching("org_pqrs_driver_Karabiner"), &iterator);
  if (kernResult != KERN_SUCCESS) {
    NSLog(@"[ERROR] IOServiceGetMatchingServices returned 0x%08x\n\n", kernResult);
    return;
  }

  for (;;) {
    // service will not be found until registerService() is called in kext.
    // If service does not exist, wait for a while and retry.
    io_service_t s = IOIteratorNext(iterator);
    if (s == IO_OBJECT_NULL) {
      NSLog(@"[INFO] IOService is not found.");
      break;
    }

    [self closeUserClient];

    // ----------------------------------------
    // setup service
    self.service = s;
    kernResult = IOObjectRetain(self.service);
    if (kernResult != KERN_SUCCESS) {
      NSLog(@"[ERROR] IOObjectRetain returned 0x%08x\n", kernResult);
      continue;
    }

    // ----------------------------------------
    // setup connect
    {
      io_connect_t c;
      kernResult = IOServiceOpen(self.service, mach_task_self(), 0, &c);
      if (kernResult != KERN_SUCCESS) {
        NSLog(@"[ERROR] IOServiceOpen returned 0x%08x\n", kernResult);
        continue;
      }

      self.connect = c;
    }

    // ----------------------------------------
    // open
    {
      uint64_t bridge_version =
#include "../../../../src/bridge/output/include.bridge_version.h"
          ;
      uint64_t open_result = 0;
      uint32_t count = 1;

      kernResult = IOConnectCallScalarMethod(self.connect, BRIDGE_USERCLIENT_OPEN,
                                             &bridge_version,
                                             1,
                                             &open_result,
                                             &count);
      if (kernResult != KERN_SUCCESS) {
        NSLog(@"[ERROR] BRIDGE_USERCLIENT_OPEN returned 0x%08x\n", kernResult);
        continue;

      } else {
        if (open_result == BRIDGE_USERCLIENT_OPEN_RETURN_ERROR_BRIDGE_VERSION_MISMATCH) {
          NSLog(@"[ERROR] BRIDGE_USERCLIENT_OPEN_RETURN_ERROR_BRIDGE_VERSION_MISMATCH\n");
          if ([Relauncher isEqualPreviousProcessVersionAndCurrentProcessVersion]) {
            @throw [NSException exceptionWithName:@"UserClient_userspaceException"
                                           reason:@"Kernel extension and app version are mismatched.\nPlease restart your system in order to reload kernel extension."
                                         userInfo:nil];
          } else {
            NSLog(@"Karabiner might have been upgraded.");
            [Relauncher relaunch];
          }
          continue;
        }
      }
    }

    // ----------------------------------------
    // set notification
    if (self.notifyport) {
      kernResult = IOConnectCallAsyncScalarMethod(self.connect,
                                                  BRIDGE_USERCLIENT_NOTIFICATION_FROM_KEXT,
                                                  IONotificationPortGetMachPort(self.notifyport),
                                                  *(self.asyncref),
                                                  kOSAsyncRef64Count,
                                                  NULL,  // input
                                                  0,     // inputCnt
                                                  NULL,  // output
                                                  NULL); // outputCnt
      if (kernResult != KERN_SUCCESS) {
        NSLog(@"[ERROR] BRIDGE_USERCLIENT_NOTIFICATION_FROM_KEXT returned 0x%08x\n", kernResult);
        continue;
      }

      // succeed
      goto finish;
    }
  }

  // failed to open connection.
  [self closeUserClient];

finish:
  IOObjectRelease(iterator);
}

- (id)init:(io_async_ref64_t*)asyncref;
{
  self = [super init];

  if (self) {
    self.service = IO_OBJECT_NULL;
    self.connect = IO_OBJECT_NULL;
    self.asyncref = asyncref;
  }

  return self;
}

// ======================================================================
- (void)connect_to_kext {
  @synchronized(self) {
    [self disconnect_from_kext];

    self.service = IO_OBJECT_NULL;
    self.connect = IO_OBJECT_NULL;

    // ----------------------------------------
    // setup IONotification
    self.notifyport = IONotificationPortCreate(kIOMasterPortDefault);
    if (!self.notifyport) {
      NSLog(@"[ERROR] IONotificationPortCreate failed\n");

    } else {
      self.loopsource = IONotificationPortGetRunLoopSource(self.notifyport);
      if (!self.loopsource) {
        NSLog(@"[ERROR] IONotificationPortGetRunLoopSource failed\n");

      } else {
        CFRunLoopAddSource(CFRunLoopGetCurrent(), self.loopsource, kCFRunLoopDefaultMode);
        [self openUserClient];
      }
    }
  }
}

- (void)disconnect_from_kext {
  @synchronized(self) {
    [self closeUserClient];

    if (self.notifyport) {
      if (self.loopsource) {
        CFRunLoopSourceInvalidate(self.loopsource);
        self.loopsource = nil;
      }
      IONotificationPortDestroy(self.notifyport);
      self.notifyport = nil;
    }
  }
}

- (BOOL)refresh_connection {
  @synchronized(self) {
    [self disconnect_from_kext];
    [self connect_to_kext];

    return self.connect != IO_OBJECT_NULL;
  }
}

- (BOOL)synchronized_communication:(struct BridgeUserClientStruct*)bridgestruct {
  @synchronized(self) {
    if (self.connect == IO_OBJECT_NULL) {
      NSLog(@"[INFO] BRIDGE_USERCLIENT_SYNCHRONIZED_COMMUNICATION connection is null");
      return NO;
    }
    if (!bridgestruct) return NO;

    uint64_t output = 0;
    uint32_t outputCnt = 1;
    kern_return_t kernResult = IOConnectCallMethod(self.connect,
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

    // succeed
    return YES;
  }
}

@end
