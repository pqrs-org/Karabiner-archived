// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>
#import "Sparkle/SUUpdater.h"
#import "StatusBar.h"
#import "WorkSpaceData.h"
#include "bridge.h"

@interface KeyRemap4MacBook_serverAppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* window;

  // for IONotification
  IONotificationPortRef notifyport_;
  CFRunLoopSourceRef loopsource_;

  struct BridgeWorkSpaceData bridgeworkspacedata_;

  IBOutlet SUUpdater* suupdater_;
  IBOutlet StatusBar* statusbar_;
}

@property (assign) IBOutlet NSWindow* window;

@end
