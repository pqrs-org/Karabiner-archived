// -*- Mode: objc -*-
//
//  GetActiveApplicationNameAppDelegate.h
//  GetActiveApplicationName
//
//  Created by Takayama Fumihiko on 09/11/19.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface GetActiveApplicationNameAppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* window;
}

@property (assign) IBOutlet NSWindow* window;

@end
