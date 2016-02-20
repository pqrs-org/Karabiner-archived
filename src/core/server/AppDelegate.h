// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>
#include "bridge.h"

@class ClientForKernelspace;
@class PreferencesManager;
@class ServerForUserspace;
@class StatusBar;
@class StatusMessageManager;
@class WorkSpaceData;
@class XMLCompiler;

@interface AppDelegate : NSObject <NSApplicationDelegate> {
  IBOutlet ClientForKernelspace* __weak clientForKernelspace;
  IBOutlet PreferencesManager* preferencesManager_;
  IBOutlet ServerForUserspace* serverForUserspace_;
  IBOutlet StatusBar* statusbar_;
  IBOutlet StatusMessageManager* statusMessageManager_;
  IBOutlet WorkSpaceData* workSpaceData_;
  IBOutlet XMLCompiler* xmlCompiler_;
}

@property(weak) ClientForKernelspace* clientForKernelspace;

- (void)updateFocusedUIElementInformation:(NSDictionary*)information;
- (NSDictionary*)getFocusedUIElementInformation;
- (NSArray*)getWorkspaceAppIds;
- (NSArray*)getWorkspaceWindowNameIds;
- (NSArray*)getWorkspaceInputSourceIds;
- (NSDictionary*)getInputSourceInformation;

+ (void)quitWithConfirmation;

@end
