//
//  AppDelegate.m
//

#import "AppDelegate.h"
#import "KeyRemap4MacBookKeys.h"
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
  dispatch_async(dispatch_get_main_queue(), ^{
                   [appQueue_ push:[[notification userInfo] objectForKey:@"name"]];
                 });
}

// ------------------------------------------------------------
- (void) distributedObserver_inputSourceChanged:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
                   [otherinformationstore_ setLanguageCode:[[notification userInfo] objectForKey:@"languageCode"]];
                   [otherinformationstore_ setInputSourceID:[[notification userInfo] objectForKey:@"inputSourceID"]];
                   [otherinformationstore_ setInputModeID:[[notification userInfo] objectForKey:@"inputModeID"]];
                 });
}

// ------------------------------------------------------------
- (void) applicationDidFinishLaunching:(NSNotification*)aNotification {
  [self setKeyResponder];

  [otherinformationstore_ setLanguageCode:nil];
  [otherinformationstore_ setInputSourceID:nil];
  [otherinformationstore_ setInputModeID:nil];

  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(distributedObserver_applicationChanged:)
                                                          name:kKeyRemap4MacBookApplicationChangedNotification
                                                        object:nil];

  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(distributedObserver_inputSourceChanged:)
                                                          name:kKeyRemap4MacBookInputSourceChangedNotification
                                                        object:nil];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication {
  return YES;
}

- (void) dealloc
{
  [[NSDistributedNotificationCenter defaultCenter] removeObserver:self];

  [super dealloc];
}

- (void) tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem
{
  if ([[tabViewItem identifier] isEqualToString:@"Main"]) {
    [self setKeyResponder];
  } else if ([[tabViewItem identifier] isEqualToString:@"Devices"]) {
    [devices_ refresh:nil];
  }
}

@end
