// -*- Mode: objc -*-
//
//  AppDelegate.h
//

#import <Cocoa/Cocoa.h>
#import "AppQueue.h"
#import "Devices.h"
#import "EventQueue.h"
#import "OtherInformationStore.h"

@class KeyRemap4MacBookClient;
@class KeyResponder;

@interface AppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* window;
  IBOutlet AppQueue* appQueue_;
  IBOutlet Devices* devices_;
  IBOutlet KeyRemap4MacBookClient* client_;
  IBOutlet OtherInformationStore* otherinformationstore_;
  IBOutlet KeyResponder* keyResponder_;
}

@property (assign) IBOutlet NSWindow* window;

@end
