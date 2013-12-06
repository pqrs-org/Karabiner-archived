// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>
#include "bridge.h"

@class IOHIDPostEventWrapper;
@class PreferencesManager;
@class StatusWindow;
@class UserClient_userspace;
@class WorkSpaceData;
@class XMLCompiler;

@interface ClientForKernelspace : NSObject {
  io_async_ref64_t asyncref_;
  UserClient_userspace* userClient_userspace;

  IBOutlet IOHIDPostEventWrapper* __weak iohidPostEventWrapper;
  IBOutlet PreferencesManager* __weak preferencesManager;
  IBOutlet StatusWindow* __weak statusWindow;
  IBOutlet WorkSpaceData* __weak workSpaceData;
  IBOutlet XMLCompiler* __weak xmlCompiler;
}

@property (weak) IOHIDPostEventWrapper* iohidPostEventWrapper;
@property (weak) PreferencesManager* preferencesManager;
@property  UserClient_userspace* userClient_userspace;
@property (weak) StatusWindow* statusWindow;
@property (weak) WorkSpaceData* workSpaceData;
@property (weak) XMLCompiler* xmlCompiler;

- (void) refresh_connection_with_retry;
- (void) disconnect_from_kext;

- (void) send_workspacedata_to_kext:(struct BridgeWorkSpaceData*)bridgeworkspacedata;
- (void) send_config_to_kext;
- (void) set_config_one:(struct BridgeSetConfigOne*)bridgeSetConfigOne;
- (NSArray*) device_information:(NSInteger)type;

@end
