//
//  KeyRemap4MacBookAppDelegate.h
//  KeyRemap4MacBook
//
//  Created by Takayama Fumihiko on 09/12/24.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "KeyRemap4MacBookClient.h"
#import "Sparkle/SUUpdater.h"

@interface KeyRemap4MacBookAppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* window;
  IBOutlet org_pqrs_KeyRemap4MacBook_Client* client_;
  IBOutlet SUUpdater* suupdater_;
}

@property (assign) IBOutlet NSWindow* window;

@end
