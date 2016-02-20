#import "AXNotifierManager.h"
#import "PreferencesKeys.h"

static dispatch_queue_t queue_;

@implementation AXNotifierManager

+ (void)initialize {
  queue_ = dispatch_queue_create("org.pqrs.Karabiner.AXNotifierManager", NULL);
}

+ (NSString*)AXNotifierPath {
  return @"/Applications/Karabiner.app/Contents/Applications/Karabiner_AXNotifier.app";
}

+ (void)launchNewAXNotifier {
  dispatch_sync(queue_, ^{
    NSString* path = [AXNotifierManager AXNotifierPath];
    NSURL* url = [NSURL fileURLWithPath:path];
    // Set NSWorkspaceLaunchNewInstance because
    // AXNotifier might be running (terminating) immediately after terminateAXNotifier.
    NSWorkspaceLaunchOptions options = NSWorkspaceLaunchDefault | NSWorkspaceLaunchNewInstance;
    [[NSWorkspace sharedWorkspace] launchApplicationAtURL:url
                                                  options:options
                                            configuration:@{}
                                                    error:nil];
  });
}

+ (void)terminateAXNotifiers {
  dispatch_sync(queue_, ^{
    NSString* path = [AXNotifierManager AXNotifierPath];
    NSString* bundleIdentifier = [[NSBundle bundleWithPath:path] bundleIdentifier];

    // If Karabiner has been moved into /Applications/Utilities, bundleIdentifier will be nil.

    if (bundleIdentifier) {
      NSArray* applications = [NSRunningApplication runningApplicationsWithBundleIdentifier:bundleIdentifier];
      for (NSRunningApplication* runningApplication in applications) {
        [runningApplication terminate];
      }
    }
  });
}

+ (void)restartAXNotifier {
  [AXNotifierManager terminateAXNotifiers];

  if ([[NSUserDefaults standardUserDefaults] boolForKey:kIsAXNotifierEnabled]) {
    [AXNotifierManager launchNewAXNotifier];
  }
}

@end
