//
//  KeyDumpAppDelegate.m
//  KeyDump
//
//  Created by Takayama Fumihiko on 09/10/25.
//

#import "KeyDumpAppDelegate.h"

@implementation KeyDumpAppDelegate

@synthesize window;

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification {
  [window makeFirstResponder:_keyResponder];
}

- (IBAction) quit:(id)sender
{
  [NSApp terminate:self];
}

@end
