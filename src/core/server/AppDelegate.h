// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>
#import "StatusBar.h"
#import "Updater.h"
#import "UserClient_userspace.h"
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
  IBOutlet UserClient_userspace* userClient_userspace;
  IBOutlet WorkSpaceData* workSpaceData_;
  IBOutlet XMLCompiler* xmlCompiler_;
}

@property (assign) IBOutlet NSWindow* window;
@property (assign) UserClient_userspace* userClient_userspace;

@end
