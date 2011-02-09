#import "StatusWindow.h"
#import "server_objc_part.h"

@implementation StatusWindow

NSString* notificationName_extra = @"Extra Message";
NSString* notificationName_lock  = @"Modifier Lock";

- (id) init
{
  self = [super init];

  if (self) {
    lines_ = [NSMutableArray new];
    lastMessages_ = [NSMutableArray new];
    for (NSUInteger i = 0; i < STATUSMESSAGETYPE__END__; ++i) {
      [lines_ addObject:@""];
      [lastMessages_ addObject:@""];
    }

    [GrowlApplicationBridge setGrowlDelegate:self];
  }

  return self;
}

- (void) dealloc
{
  [lines_ release];
  [lastMessages_ release];

  [super dealloc];
}

// ------------------------------------------------------------
- (void) displayGrowlNotRunningWarning
{
  NSString* message = nil;
  /*  */ if (! [GrowlApplicationBridge isGrowlInstalled]) {
    message = @"Growl is not installed.\nPlease install Growl and restart the system.";
  } else if (! [GrowlApplicationBridge isGrowlRunning]) {
    message = @"Growl is not running.\nPlease start Growl.";
  }

  if (message) {
    if (! isGrowlNotRunningWarningDisplayed_) {
      isGrowlNotRunningWarningDisplayed_ = YES;

      NSAlert* alert = [NSAlert alertWithMessageText:@"KeyRemap4MacBook Warning"
                                       defaultButton:@"Close"
                                     alternateButton:nil
                                         otherButton:nil
                           informativeTextWithFormat:[NSString stringWithFormat:@"KeyRemap4MacBook uses Growl to display extra messages.\n\n%@", message]];
      [alert runModal];
    }
  }
}

- (void) updateStatusMessage
{
  NSString* message = nil;

  // ------------------------------------------------------------
  // STATUSMESSAGETYPE_LOCK
  message = [lines_ objectAtIndex:STATUSMESSAGETYPE_LOCK];

  BOOL isSticky = ([message length] > 0);

  if (! [message isEqualToString:[lastMessages_ objectAtIndex:STATUSMESSAGETYPE_LOCK]]) {
    [lastMessages_ replaceObjectAtIndex:STATUSMESSAGETYPE_LOCK withObject:message];

    [self displayGrowlNotRunningWarning];
    [GrowlApplicationBridge
      notifyWithTitle:notificationName_lock
          description:message
     notificationName:notificationName_lock
             iconData:nil
             priority:0
             isSticky:isSticky
         clickContext:nil
           identifier:@"org_pqrs_KeyRemap4MacBook_lock"];
  }

  // ------------------------------------------------------------
  // STATUSMESSAGETYPE_EXTRA
  message = [lines_ objectAtIndex:STATUSMESSAGETYPE_EXTRA];

  if ([message length] > 0) {
    [lastMessages_ replaceObjectAtIndex:STATUSMESSAGETYPE_EXTRA withObject:message];

    [self displayGrowlNotRunningWarning];
    [GrowlApplicationBridge
      notifyWithTitle:@"Enabling"
          description:message
     notificationName:notificationName_extra
             iconData:nil
             priority:0
             isSticky:YES
         clickContext:nil
           identifier:@"org_pqrs_KeyRemap4MacBook_extra"];

  } else {
    message = [lastMessages_ objectAtIndex:STATUSMESSAGETYPE_EXTRA];
    if ([message length] > 0) {
      [self displayGrowlNotRunningWarning];
      [GrowlApplicationBridge
        notifyWithTitle:@"Disabling"
            description:message
       notificationName:notificationName_extra
               iconData:nil
               priority:0
               isSticky:NO
           clickContext:nil
             identifier:@"org_pqrs_KeyRemap4MacBook_extra"];

      [lastMessages_ replaceObjectAtIndex:STATUSMESSAGETYPE_EXTRA withObject:@""];
    }
  }
}

- (void) callUpdateStatusMessage
{
  [self performSelectorOnMainThread:@selector(updateStatusMessage)
                         withObject:nil
                      waitUntilDone:NO];
}

- (void) resetStatusMessage
{
  for (NSUInteger i = 0; i < STATUSMESSAGETYPE__END__; ++i) {
    [lines_ replaceObjectAtIndex:i withObject:@""];
  }

  [self callUpdateStatusMessage];
}

- (void) setStatusMessage:(NSUInteger)lineIndex message:(NSString*)message
{
  [lines_ replaceObjectAtIndex:lineIndex withObject:message];
  [self callUpdateStatusMessage];
}

// ============================================================
// Growl delegate
- (NSDictionary*) registrationDictionaryForGrowl
{
  NSArray* array = [NSArray arrayWithObjects:@"Modifier Lock", @"Extra Message", nil];
  NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                        [NSNumber numberWithInt:1],
                        @"TicketVersion",
                        array,
                        @"AllNotifications",
                        array,
                        @"DefaultNotifications",
                        nil];
  return dict;
}

- (NSString*) applicationNameForGrowl
{
  return @"KeyRemap4MacBook";
}

- (NSData*) applicationIconDataForGrowl
{
  return [NSImage imageNamed:@"applicationIconDataForGrowl"];
}

@end
