// -*- Mode: objc -*-
//
//  KeyRemap4MacBook_serverAppDelegate.h
//  KeyRemap4MacBook_server
//
//  Created by Takayama Fumihiko on 09/11/01.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "server_objc_part.h"

@interface KeyRemap4MacBook_serverAppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* window;
  IBOutlet ServerObjcPart* serverobjcpart_;
  IBOutlet NSTextField* statusmessage_;
  IBOutlet NSImageView* statusmessage_background_;
}

@property (assign) IBOutlet NSWindow* window;

@end
