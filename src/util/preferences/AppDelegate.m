#import "AppDelegate.h"
#import "PreferencesWindowController.h"
#import "SharedPreferencesManager.h"

@interface AppDelegate ()

@property(weak) IBOutlet PreferencesWindowController* preferencesWindowController;
@property(weak) IBOutlet SharedPreferencesManager* sharedPreferencesManager;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
  [self.sharedPreferencesManager load];

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
