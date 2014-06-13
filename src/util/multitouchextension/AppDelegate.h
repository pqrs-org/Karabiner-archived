// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>
#import "IgnoredAreaView.h"
#import "FingerStatus.h"
#import "KarabinerClient.h"
#import "PreferencesController.h"

@interface AppDelegate : NSObject <NSApplicationDelegate> {
  NSArray* mtdevices_;
  IONotificationPortRef notifyport_;
  CFRunLoopSourceRef loopsource_;

  IBOutlet PreferencesController* preferences_;
  IBOutlet IgnoredAreaView* ignoredAreaView_;
  IBOutlet KarabinerClient* client_;
}

@end
