//
//  KeyDumpAppDelegate.m
//  KeyDump
//
//  Created by Takayama Fumihiko on 09/10/25.
//

#import "KeyDumpAppDelegate.h"
#import "KeyRemap4MacBookKeys.h"
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

  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(observer_applicationChanged:)
                                                          name:kKeyRemap4MacBookApplicationChangedNotification
                                                        object:kKeyRemap4MacBookNotificationKey];

  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(observer_inputSourceChanged:)
                                                          name:kKeyRemap4MacBookInputSourceChangedNotification
                                                        object:kKeyRemap4MacBookNotificationKey];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication {
  return YES;
}

@end
