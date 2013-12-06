// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>
#include "bridge.h"

@class IOHIDPostEventWrapper;
@class PreferencesManager;
@class StatusWindow;
@class UserClient_userspace;
@class WorkSpaceData;
@class XMLCompiler;

@interface ClientForKernelspace : NSObject
{
  IBOutlet IOHIDPostEventWrapper* iohidPostEventWrapper_;
  IBOutlet PreferencesManager* preferencesManager_;
  IBOutlet StatusWindow* statusWindow_;
  IBOutlet WorkSpaceData* workSpaceData_;
  IBOutlet XMLCompiler* xmlCompiler_;
}

- (void) refresh_connection_with_retry;
- (void) disconnect_from_kext;

- (void) send_workspacedata_to_kext:(struct BridgeWorkSpaceData*)bridgeworkspacedata;
- (void) send_config_to_kext;
- (void) set_config_one:(struct BridgeSetConfigOne*)bridgeSetConfigOne;
- (NSArray*) device_information:(NSInteger)type;

@end
