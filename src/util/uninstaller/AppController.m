#import "AppController.h"

@implementation AppController

static NSString *appName = @"KeyRemap4MacBook";
static NSString *uninstallCommand = @"/Library/org.pqrs/KeyRemap4MacBook/bin/uninstall";

- (void) setMessage
{
  [_message setStringValue:[NSString stringWithFormat:@"%@ Uninstaller", appName]];
}

// ----------------------------------------------------------------------
- (IBAction) uninstall:(id)sender
{
  NSTask *task = [[NSTask alloc] init];
  [task setLaunchPath:uninstallCommand];
  [task launch];
  [task waitUntilExit];

  NSRunAlertPanel(@"Uninstaller", [NSString stringWithFormat:@"%@ is uninstalled.", appName], @"OK", nil, nil);
  [NSApp terminate:self];
}

// ----------------------------------------------------------------------
- (void) applicationDidFinishLaunching:(NSNotification *)notification
{
  [self setMessage];
}

@end
