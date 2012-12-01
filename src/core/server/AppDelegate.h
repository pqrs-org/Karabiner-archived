// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>
#include "bridge.h"

@class ClientForKernelspace;
@class PreferencesManager;
@class ServerForUserspace;
@class StatusBar;
@class StatusWindow;
@class Updater;
@class WorkSpaceData;
@class XMLCompiler;

@interface AppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* window;

  // for IONotification
  IONotificationPortRef notifyport_;
  CFRunLoopSourceRef loopsource_;

  struct BridgeWorkSpaceData bridgeworkspacedata_;

  IBOutlet ClientForKernelspace* clientForKernelspace;
  IBOutlet NSWindow* preferencesWindow_;
  IBOutlet PreferencesManager* preferencesManager_;
  IBOutlet ServerForUserspace* serverForUserspace_;
  IBOutlet StatusBar* statusbar_;
  IBOutlet StatusWindow* statusWindow_;
  IBOutlet Updater* updater_;
  IBOutlet WorkSpaceData* workSpaceData_;
  IBOutlet XMLCompiler* xmlCompiler_;
}

@property (assign) IBOutlet NSWindow* window;
@property (assign) ClientForKernelspace* clientForKernelspace;

- (IBAction) launchEventViewer:(id)sender;
- (IBAction) launchMultiTouchExtension:(id)sender;
- (IBAction) launchUninstaller:(id)sender;
- (IBAction) openPreferences:(id)sender;
- (IBAction) openPrivateXML:(id)sender;

@end
