// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>
#import "ClientForKernelspace.h"
#import "StatusBar.h"
#import "StatusWindow.h"
#import "Updater.h"
#import "WorkSpaceData.h"
#include "bridge.h"

@interface AppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* window;

  // for IONotification
  IONotificationPortRef notifyport_;
  CFRunLoopSourceRef loopsource_;

  struct BridgeWorkSpaceData bridgeworkspacedata_;

  IBOutlet PreferencesManager* preferencesManager_;
  IBOutlet StatusBar* statusbar_;
  IBOutlet StatusWindow* statusWindow_;
  IBOutlet Updater* updater_;
  IBOutlet ClientForKernelspace* clientForKernelspace;
  IBOutlet WorkSpaceData* workSpaceData_;
  IBOutlet XMLCompiler* xmlCompiler_;
}

@property (assign) IBOutlet NSWindow* window;
@property (assign) ClientForKernelspace* clientForKernelspace;

@end
