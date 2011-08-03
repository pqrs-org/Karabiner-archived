//
//  KeyDumpAppDelegate.m
//  KeyDump
//
//  Created by Takayama Fumihiko on 09/10/25.
//

#import "KeyDumpAppDelegate.h"
#import "KeyRemap4MacBookKeys.h"
#import "KeyRemap4MacBookNSDistributedNotificationCenter.h"
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

  [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter addObserver:self
                                                                selector:@selector(observer_applicationChanged:)
                                                                    name:kKeyRemap4MacBookApplicationChangedNotification];

  [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter addObserver:self
                                                                selector:@selector(observer_inputSourceChanged:)
                                                                    name:kKeyRemap4MacBookInputSourceChangedNotification];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication {
  return YES;
}

@end
