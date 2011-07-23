// -*- Mode: objc -*-
//
//  KeyRemap4MacBook_serverAppDelegate.h
//  KeyRemap4MacBook_server
//
//  Created by Takayama Fumihiko on 09/11/01.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "StatusBar.h"
#import "WorkSpaceData.h"
#import "ConfigXMLParser.h"
#include "bridge.h"

@interface KeyRemap4MacBook_serverAppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* window;
  BOOL isSessionActive_;

  // for IONotification
  IONotificationPortRef notifyport_;
  CFRunLoopSourceRef loopsource_;

  struct BridgeWorkSpaceData bridgeworkspacedata_;

  IBOutlet StatusBar* statusbar_;
}

@property (assign) IBOutlet NSWindow* window;

@end
