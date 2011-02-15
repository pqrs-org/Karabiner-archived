// -*- Mode: objc -*-
//
//  KeyRemap4MacBook_multitouchextensionAppDelegate.h
//  KeyRemap4MacBook_multitouchextension
//
//  Created by Takayama Fumihiko on 09/11/24.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "KeyRemap4MacBookClient.h"
#import "PreferencesController.h"

@interface KeyRemap4MacBook_multitouchextensionAppDelegate : NSObject <NSApplicationDelegate> {
  NSStatusItem* statusItem_;
  NSMutableArray* mtdevices_;

  IBOutlet NSMenu* statusMenu_;
  IBOutlet PreferencesController* preferences_;
  IBOutlet org_pqrs_KeyRemap4MacBook_Client* client_;
}

- (IBAction) quit:(id)sender;

@end
