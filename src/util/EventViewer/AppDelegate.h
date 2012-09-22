// -*- Mode: objc -*-
//
//  AppDelegate.h
//

#import <Cocoa/Cocoa.h>
#import "AppQueue.h"
#import "Devices.h"
#import "EventQueue.h"
#import "OtherInformationStore.h"

@interface AppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* window;
  IBOutlet id keyResponder_;
  IBOutlet AppQueue* appQueue_;
  IBOutlet Devices* devices_;
  IBOutlet OtherInformationStore* otherinformationstore_;
}

@property (assign) IBOutlet NSWindow* window;

@end
