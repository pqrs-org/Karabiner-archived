#import "AppDelegate.h"
#import "PreferencesModel.h"
#import "PreferencesWindowController.h"
#import "ServerClient.h"

@interface AppDelegate ()

@property(weak) IBOutlet AXNotifierPreferencesModel* axNotifierPreferencesModel;
@property(weak) IBOutlet PreferencesModel* preferencesModel;
@property(weak) IBOutlet ServerClient* client;
@property(weak) IBOutlet PreferencesWindowController* preferencesWindowController;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
  [self.client.proxy loadPreferencesModel:self.preferencesModel];
  [self.client.proxy loadAXNotifierPreferencesModel:self.axNotifierPreferencesModel];

  [self.preferencesWindowController setup];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication {
  return YES;
}

- (BOOL)applicationShouldHandleReopen:(NSApplication*)theApplication hasVisibleWindows:(BOOL)flag {
  [self.preferencesWindowController show];
  return YES;
}

@end
