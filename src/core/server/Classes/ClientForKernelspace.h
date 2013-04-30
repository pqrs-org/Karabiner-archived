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

  IBOutlet IOHIDPostEventWrapper* iohidPostEventWrapper;
  IBOutlet PreferencesManager* preferencesManager;
  IBOutlet StatusWindow* statusWindow;
  IBOutlet WorkSpaceData* workSpaceData;
  IBOutlet XMLCompiler* xmlCompiler;
}

@property (assign) IOHIDPostEventWrapper* iohidPostEventWrapper;
@property (assign) PreferencesManager* preferencesManager;
@property (assign) UserClient_userspace* userClient_userspace;
@property (assign) StatusWindow* statusWindow;
@property (assign) WorkSpaceData* workSpaceData;
@property (assign) XMLCompiler* xmlCompiler;

- (void) refresh_connection_with_retry;
- (void) disconnect_from_kext;

- (void) send_workspacedata_to_kext:(struct BridgeWorkSpaceData*)bridgeworkspacedata;
- (void) send_config_to_kext;
- (void) set_config_one:(struct BridgeSetConfigOne*)bridgeSetConfigOne;
- (NSArray*) device_information:(NSInteger)type;
- (void) enqueue_power_key;
- (BOOL) is_power_key_changed;

@end
