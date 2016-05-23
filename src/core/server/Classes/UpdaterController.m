#import "UpdaterController.h"

static dispatch_queue_t queue_;

@implementation UpdaterController

+ (void)initialize {
  static dispatch_once_t once;
  dispatch_once(&once, ^{
    queue_ = dispatch_queue_create("org.pqrs.Karabiner.UpdaterController", NULL);
  });
}

+ (NSString*)path {
  return @"/Library/Application Support/org.pqrs/Karabiner/updater/Karabiner.app";
}

+ (void)launch:(NSString*)argument {
  [[NSWorkspace sharedWorkspace] launchApplicationAtURL:[NSURL fileURLWithPath:self.path]
                                                options:0
                                          configuration:@{ NSWorkspaceLaunchConfigurationArguments : @[ argument ] }
                                                  error:nil];
}

+ (void)checkForUpdatesInBackground {
  dispatch_sync(queue_, ^{
    [UpdaterController launch:@"checkForUpdatesInBackground"];
  });
}

+ (void)checkForUpdatesStableOnly {
  dispatch_sync(queue_, ^{
    [UpdaterController launch:@"checkForUpdatesStableOnly"];
  });
}

+ (void)checkForUpdatesWithBetaVersion {
  dispatch_sync(queue_, ^{
    [UpdaterController launch:@"checkForUpdatesWithBetaVersion"];
  });
}

@end
