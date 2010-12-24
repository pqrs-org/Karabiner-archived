//
//  KeyDumpAppDelegate.m
//  KeyDump
//
//  Created by Takayama Fumihiko on 09/10/25.
//

#import "KeyDumpAppDelegate.h"
#import <Carbon/Carbon.h>

@implementation KeyDumpAppDelegate

@synthesize window;

// ------------------------------------------------------------
- (void) observer_applicationChanged:(NSNotification*)notification
{
  [otherinformationstore_ setApplicationName:[[notification userInfo] objectForKey:@"name"]];
}

// ------------------------------------------------------------
- (void) observer_inputSourceChanged:(NSNotification*)notification
{
  [otherinformationstore_ setInputSourceName:[[notification userInfo] objectForKey:@"name"]];
}

// ------------------------------------------------------------
- (void) applicationDidFinishLaunching:(NSNotification*)aNotification {
  [window makeFirstResponder:keyResponder_];

  [otherinformationstore_ setVersion];
  [otherinformationstore_ setApplicationName:nil];
  [otherinformationstore_ setInputSourceName:nil];

  NSString* observedObject = @"org.pqrs.KeyRemap4MacBook.KeyDump";
  [[NSDistributedNotificationCenter defaultCenter] addObserver:self selector:@selector(observer_applicationChanged:) name:@"applicationChanged" object:observedObject];
  [[NSDistributedNotificationCenter defaultCenter] addObserver:self selector:@selector(observer_inputSourceChanged:) name:@"inputSourceChanged" object:observedObject];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication {
  return YES;
}

@end
