// -*- Mode: objc -*-
//
//  KeyDumpAppDelegate.h
//  KeyDump
//
//  Created by Takayama Fumihiko on 09/10/25.
//

#import <Cocoa/Cocoa.h>

@interface KeyDumpAppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow *window;
  IBOutlet id _keyResponder;
  IBOutlet id _result;
}

@property (assign) IBOutlet NSWindow *window;

//
// Actions
//
- (IBAction) clear:(id)sender;
- (IBAction) copy:(id)sender;
- (IBAction) quit:(id)sender;

@end
