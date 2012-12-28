// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "KeyRemap4MacBookClient.h"

@interface AppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* window;
  IBOutlet KeyRemap4MacBookClient* client_;
}

@property (assign) IBOutlet NSWindow* window;

@end
