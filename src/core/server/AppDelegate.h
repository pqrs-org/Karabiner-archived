// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>
#include "bridge.h"

@class ClientForKernelspace;
@class PreferencesController;
@class PreferencesManager;
@class ServerForUserspace;
@class StatusBar;
@class StatusMessageManager;
@class Updater;
@class WorkSpaceData;
@class XMLCompiler;

@interface AppDelegate : NSObject <NSApplicationDelegate> {
  IBOutlet ClientForKernelspace* __weak clientForKernelspace;
  IBOutlet PreferencesController* preferencesController_;
  IBOutlet PreferencesManager* preferencesManager_;
  IBOutlet ServerForUserspace* serverForUserspace_;
  IBOutlet StatusBar* statusbar_;
  IBOutlet StatusMessageManager* statusMessageManager_;
  IBOutlet Updater* updater_;
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

- (IBAction)restartAXNotifier:(id)sender;
- (IBAction)manageAXNotifier:(id)sender;
- (IBAction)launchEventViewer:(id)sender;
- (IBAction)launchMultiTouchExtension:(id)sender;
- (IBAction)launchUninstaller:(id)sender;
- (IBAction)openPreferences:(id)sender;
- (IBAction)openPrivateXML:(id)sender;
- (IBAction)quit:(id)sender;
- (IBAction)relaunch:(id)sender;

@end
