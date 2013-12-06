// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "KeyRemap4MacBookClient.h"

@interface AppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* __weak window;
  IBOutlet KeyRemap4MacBookClient* client_;
}

@property (weak) IBOutlet NSWindow* window;

@end
