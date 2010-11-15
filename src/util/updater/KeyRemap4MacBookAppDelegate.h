//
//  KeyRemap4MacBookAppDelegate.h
//  KeyRemap4MacBook
//
//  Created by Takayama Fumihiko on 09/12/24.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "PreferencesClient.h"
#import "Sparkle/SUUpdater.h"

@interface KeyRemap4MacBookAppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* window;
  IBOutlet org_pqrs_KeyRemap4MacBook_PreferencesClient* preferencesclient_;
  IBOutlet SUUpdater* _suupdater;
}

@property (assign) IBOutlet NSWindow* window;

@end
