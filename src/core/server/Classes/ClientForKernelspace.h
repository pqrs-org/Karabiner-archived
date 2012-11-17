// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>
#import "UserClient_userspace.h"
#import "StatusWindow.h"
#import "WorkSpaceData.h"

@interface ClientForKernelspace : NSObject {
  io_async_ref64_t asyncref_;
  UserClient_userspace* userClient_userspace;

  IBOutlet PreferencesManager* preferencesManager_;
  IBOutlet StatusWindow* statusWindow;
  IBOutlet WorkSpaceData* workSpaceData;
  IBOutlet XMLCompiler* xmlCompiler_;
}

@property (assign) UserClient_userspace* userClient_userspace;
@property (assign) StatusWindow* statusWindow;
@property (assign) WorkSpaceData* workSpaceData;

- (void) refresh_connection_with_retry;
- (void) disconnect_from_kext;

- (void) configXMLReloaded;
- (void) preferencesChanged;
- (void) send_workspacedata_to_kext:(struct BridgeWorkSpaceData*)bridgeworkspacedata;

@end
