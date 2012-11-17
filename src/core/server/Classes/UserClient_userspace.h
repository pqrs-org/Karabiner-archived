// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>
#import <IOKit/IOKitLib.h>
#include "bridge.h"

@interface UserClient_userspace : NSObject {
  io_service_t service_;
  io_connect_t connect_;
  IONotificationPortRef notifyport_;
  CFRunLoopSourceRef loopsource_;
  io_async_ref64_t* asyncref_;
  BOOL connected;
}

@property (assign) BOOL connected;

- (id) init:(io_async_ref64_t*)asyncref;

- (void) refresh_connection_with_retry:(int)retrycount wait:(NSTimeInterval)wait;
- (void) disconnect_from_kext;
- (BOOL) synchronized_communication:(struct BridgeUserClientStruct*)bridgestruct;

@end
