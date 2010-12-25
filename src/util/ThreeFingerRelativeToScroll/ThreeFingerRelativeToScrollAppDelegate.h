// -*- Mode: objc -*-
//
//  ThreeFingerRelativeToScrollAppDelegate.h
//  ThreeFingerRelativeToScroll
//
//  Created by Takayama Fumihiko on 09/11/24.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "KeyRemap4MacBookClient.h"

@interface ThreeFingerRelativeToScrollAppDelegate : NSObject <NSApplicationDelegate> {
  NSStatusItem* _statusItem;
  IBOutlet NSMenu* _statusMenu;
  IBOutlet NSMenuItem* _startAtLoginMenuItem;
  IBOutlet org_pqrs_KeyRemap4MacBook_Client* client_;
}

- (IBAction) quit:(id)sender;
- (IBAction) setStartAtLogin:(id)sender;

@end
