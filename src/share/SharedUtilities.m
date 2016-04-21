#import "SharedUtilities.h"

@implementation SharedUtilities

+ (BOOL)confirmQuit {
  NSAlert* alert = [NSAlert new];
  alert.messageText = @"Are you sure you want to quit Karabiner?";
  alert.informativeText = @"The changed key will be restored after Karabiner is quit.";
  [alert addButtonWithTitle:@"Quit"];
  [alert addButtonWithTitle:@"Cancel"];
  return [alert runModal] == NSAlertFirstButtonReturn;
}

@end
