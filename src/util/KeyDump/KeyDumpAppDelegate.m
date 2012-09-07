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
- (void) distributedObserver_applicationChanged:(NSNotification*)notification
{
  // [NSAutoreleasePool drain] is never called from NSDistributedNotificationCenter.
  // Therefore, we need to make own NSAutoreleasePool.
  NSAutoreleasePool* pool = [NSAutoreleasePool new];
  {
    [otherinformationstore_ setApplicationName:[[notification userInfo] objectForKey:@"name"]];
  }
  [pool drain];
}

// ------------------------------------------------------------
- (void) distributedObserver_inputSourceChanged:(NSNotification*)notification
{
  // [NSAutoreleasePool drain] is never called from NSDistributedNotificationCenter.
  // Therefore, we need to make own NSAutoreleasePool.
  NSAutoreleasePool* pool = [NSAutoreleasePool new];
  {
    [otherinformationstore_ setInputSourceName:[[notification userInfo] objectForKey:@"inputSourceID"]];
    [otherinformationstore_ setInputModeName:[[notification userInfo] objectForKey:@"inputModeID"]];
  }
  [pool drain];
}

// ------------------------------------------------------------
- (void) applicationDidFinishLaunching:(NSNotification*)aNotification {
  [window makeFirstResponder:keyResponder_];

  [otherinformationstore_ setVersion];
  [otherinformationstore_ setApplicationName:nil];
  [otherinformationstore_ setInputSourceName:nil];
  [otherinformationstore_ setInputModeName:nil];
  [otherinformationstore_ setDeviceInformation];

  [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter addObserver:self
                                                                selector:@selector(distributedObserver_applicationChanged:)
                                                                    name:kKeyRemap4MacBookApplicationChangedNotification];

  [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter addObserver:self
                                                                selector:@selector(distributedObserver_inputSourceChanged:)
                                                                    name:kKeyRemap4MacBookInputSourceChangedNotification];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication {
  return YES;
}

- (void) dealloc
{
  [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter removeObserver:self];

  [super dealloc];
}

@end
