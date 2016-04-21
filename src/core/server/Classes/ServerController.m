#import "ServerController.h"
#import "PreferencesKeys.h"
#import "SharedUtilities.h"
#import "StartAtLoginUtilities.h"

@implementation ServerController

+ (void)quitWithConfirmation {
  if ([SharedUtilities confirmQuit]) {
    [ServerController updateStartAtLogin:NO];
    [NSApp terminate:nil];
  }
}

+ (void)updateStartAtLogin:(BOOL)preferredValue {
  if (!preferredValue) {
    [StartAtLoginUtilities setStartAtLogin:NO];

  } else {
    // Do not register to StartAtLogin if kResumeAtLogin is NO.
    if (![[NSUserDefaults standardUserDefaults] boolForKey:kResumeAtLogin]) {
      [StartAtLoginUtilities setStartAtLogin:NO];
    } else {
      [StartAtLoginUtilities setStartAtLogin:YES];
    }
  }
}

@end
