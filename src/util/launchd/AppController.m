#import "AppController.h"

@implementation AppController

NSTask *task_sysctl_confd = nil;
NSTask *task_server = nil;
NSTask *task_statusbar = nil;

- (void) applicationDidFinishLaunching:(NSNotification *)aNotification
{
  NSLog(@"KeyRemap4MacBook_launchd initialize");

  NSArray *args = [NSArray array];

  NSString *sysctl_confd = @"/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_confd";
  task_sysctl_confd = [NSTask launchedTaskWithLaunchPath:sysctl_confd arguments:args];

  NSString *server = @"/Library/org.pqrs/KeyRemap4MacBook/server/KeyRemap4MacBook_server";
  task_server = [NSTask launchedTaskWithLaunchPath:server arguments:args];

  NSString *statusbar = @"/Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook_statusbar.app";
  [[NSWorkspace sharedWorkspace] launchApplication:statusbar];
}

- (void) applicationWillTerminate:(NSNotification *)aNotification
{
  NSLog(@"KeyRemap4MacBook_launchd terminate");

  if (task_sysctl_confd) {
    [task_sysctl_confd terminate];
  }
  if (task_server) {
    [task_server terminate];
  }
}

@end
