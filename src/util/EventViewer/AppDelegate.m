#import "AppDelegate.h"
#import "KeyRemap4MacBookClient.h"
#import "KeyRemap4MacBookKeys.h"
#import "KeyResponder.h"
#import <Carbon/Carbon.h>

@implementation AppDelegate

@synthesize window;

- (void) setKeyResponder
{
  [window makeFirstResponder:keyResponder_];
}

- (void) addToAppQueue
{
  @try {
    [appQueue_ push:[[client_ proxy] application_information][@"name"]];
  } @catch (NSException* exception) {
    NSLog(@"%@", exception);
  }
}

- (void) updateOtherInformationStore
{
  @try {
    NSDictionary* d = [[client_ proxy] inputsource_information];
    [otherinformationstore_ setLanguageCode:d[@"languageCode"]];
    [otherinformationstore_ setInputSourceID:d[@"inputSourceID"]];
    [otherinformationstore_ setInputModeID:d[@"inputModeID"]];
  } @catch (NSException* exception) {
    NSLog(@"%@", exception);
  }
}

// ------------------------------------------------------------
- (void) distributedObserver_applicationChanged:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    [self addToAppQueue];
  });
}

// ------------------------------------------------------------
- (void) distributedObserver_inputSourceChanged:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    [self updateOtherInformationStore];
  });
}

// ------------------------------------------------------------
- (void) applicationDidFinishLaunching:(NSNotification*)aNotification {
  [self setKeyResponder];
  [self updateOtherInformationStore];

  // We need to speficy NSNotificationSuspensionBehaviorDeliverImmediately for NSDistributedNotificationCenter
  // in order to get notifications when this app is not active.
  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(distributedObserver_applicationChanged:)
                                                          name:kKeyRemap4MacBookApplicationChangedNotification
                                                        object:nil
                                            suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];

  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(distributedObserver_inputSourceChanged:)
                                                          name:kKeyRemap4MacBookInputSourceChangedNotification
                                                        object:nil
                                            suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication {
  return YES;
}

- (void) dealloc
{
  [[NSDistributedNotificationCenter defaultCenter] removeObserver:self];

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
