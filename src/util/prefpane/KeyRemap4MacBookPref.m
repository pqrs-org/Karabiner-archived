// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "KeyRemap4MacBookPref.h"
#import "sharecode.h"

@implementation KeyRemap4MacBookPref

// ----------------------------------------------------------------------
- (void) drawVersion
{
  NSString *version = [BUNDLEPREFIX_SysctlWrapper getString:@"keyremap4macbook.version"];
  if (! version) {
    version = @"-.-.-";
  }
  [_versionText setStringValue:version];
}

// ----------------------------------------------------------------------
- (NSString *) getExecResult:(NSArray *)args
{
  NSTask *task = [[NSTask alloc] init];
  NSPipe *pipe = [NSPipe pipe];
  [task setStandardOutput:pipe];
  [task setLaunchPath:@"/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_ctl"];
  [task setArguments:args];
  [task launch];
  [task waitUntilExit];

  NSData *data = [[pipe fileHandleForReading] readDataToEndOfFile];
  NSString *result = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
  return result;
}

- (void) setStatusBarState
{
  NSString *result = [self getExecResult:[NSArray arrayWithObjects:@"statusbar", nil]];
  if (! result) return;

  if ([result intValue] == 1) {
    [_checkbox_statusbar setState:NSOnState];
  } else {
    [_checkbox_statusbar setState:NSOffState];
  }
}

- (void) startStatusBar
{
  NSString *killall = @"/usr/bin/killall";
  NSArray *args = [NSArray arrayWithObjects:@"KeyRemap4MacBook_statusbar", nil];
  NSTask *task_killall = [NSTask launchedTaskWithLaunchPath:killall arguments:args];
  [task_killall waitUntilExit];

  NSString *app = @"/Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook_statusbar.app";
  [[NSWorkspace sharedWorkspace] launchApplication:app];
}

- (IBAction) toggleStatusBar:(id)sender
{
  [self getExecResult:[NSArray arrayWithObjects:@"toggle_statusbar", nil]];
  [self setStatusBarState];
  [self startStatusBar];
}

// ----------------------------------------------------------------------
- (void) mainViewDidLoad
{
  [self drawVersion];
  [self setStatusBarState];
}

@end
