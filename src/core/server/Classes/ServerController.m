#import "ServerController.h"
#import "StartAtLoginUtilities.h"

@implementation ServerController

+ (void)quitWithConfirmation {
  NSAlert* alert = [NSAlert new];
  alert.messageText = @"Are you sure you want to quit Karabiner?";
  alert.informativeText = @"The changed key will be restored after Karabiner is quit.";
  [alert addButtonWithTitle:@"Quit"];
  [alert addButtonWithTitle:@"Cancel"];
  if ([alert runModal] != NSAlertFirstButtonReturn) {
    return;
  }

  [StartAtLoginUtilities setStartAtLogin:NO];
  [NSApp terminate:nil];
}

@end
