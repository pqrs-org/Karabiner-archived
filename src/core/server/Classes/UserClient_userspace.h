// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>
#include <IOKit/IOKitLib.h>
#include "bridge.h"
#include "StatusWindow.h"
#include "WorkSpaceData.h"

@interface UserClient_userspace : NSObject {
  io_service_t service_;
  io_connect_t connect_;
  IONotificationPortRef notifyport_;
  CFRunLoopSourceRef loopsource_;
  BOOL connected;

  IBOutlet StatusWindow* statusWindow;
  IBOutlet WorkSpaceData* workSpaceData;
}

@property (assign) BOOL connected;
@property (assign) StatusWindow* statusWindow;
@property (assign) WorkSpaceData* workSpaceData;

- (void) refresh_connection;
- (void) disconnect_from_kext;
- (BOOL) synchronized_communication:(struct BridgeUserClientStruct*)bridgestruct;

@end
