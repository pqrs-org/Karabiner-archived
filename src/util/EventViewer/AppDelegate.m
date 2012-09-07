//
//  AppDelegate.m
//

#import "AppDelegate.h"
#import "KeyRemap4MacBookKeys.h"
#import "KeyRemap4MacBookNSDistributedNotificationCenter.h"
#import <Carbon/Carbon.h>

@implementation AppDelegate

@synthesize window;

- (void) setKeyResponder
{
  [window makeFirstResponder:keyResponder_];
}

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
  [self setKeyResponder];

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

- (void) tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem
{
  if ([[tabViewItem identifier] isEqualToString:@"Main"]) {
    [self setKeyResponder];
  }
}

@end
