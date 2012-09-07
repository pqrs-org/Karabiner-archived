// -*- Mode: objc -*-
//
//  AppDelegate.h
//

#import <Cocoa/Cocoa.h>
#import "EventQueue.h"
#import "OtherInformationStore.h"

@interface AppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* window;
  IBOutlet id keyResponder_;
  IBOutlet OtherInformationStore* otherinformationstore_;
}

@property (assign) IBOutlet NSWindow* window;

@end
