// -*- Mode: objc -*-
//
//  ThreeFingerRelativeToScrollAppDelegate.h
//  ThreeFingerRelativeToScroll
//
//  Created by Takayama Fumihiko on 09/11/24.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ThreeFingerRelativeToScrollAppDelegate : NSObject <NSApplicationDelegate> {
  NSStatusItem* _statusItem;
  IBOutlet NSMenu* _statusMenu;
}

- (IBAction) quit:(id)sender;

@end
