// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>
#import "IgnoredAreaView.h"
#import "FingerStatus.h"
#import "KeyRemap4MacBookClient.h"
#import "PreferencesController.h"

@interface KeyRemap4MacBook_multitouchextensionAppDelegate : NSObject <NSApplicationDelegate> {
  NSArray* mtdevices_;
  IONotificationPortRef notifyport_;
  CFRunLoopSourceRef loopsource_;

  IBOutlet PreferencesController* preferences_;
  IBOutlet IgnoredAreaView* ignoredAreaView_;
  IBOutlet org_pqrs_KeyRemap4MacBook_Client* client_;
}

@end
