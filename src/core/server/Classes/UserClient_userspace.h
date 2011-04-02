// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#include <IOKit/IOKitLib.h>
#include "bridge.h"

@interface UserClient_userspace : NSObject {
  io_service_t service_;
  io_connect_t connect_;
  IONotificationPortRef notifyport_;
  CFRunLoopSourceRef loopsource_;
  BOOL connected;
}

@property (assign) BOOL connected;

+ (void) refresh_connection;
+ (void) disconnect_from_kext;
+ (BOOL) synchronized_communication_with_retry:(struct BridgeUserClientStruct*)bridgestruct;

@end
