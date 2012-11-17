#import "StatusWindow.h"
#include "bridge.h"

@implementation StatusWindow

NSString* notificationName_extra                = @"Extra Message";
NSString* notificationName_modifier_lock        = @"Locked Modifier Keys";
NSString* notificationName_modifier_sticky      = @"Sticky Modifier Keys";
NSString* notificationName_pointing_button_lock = @"Locked Pointing Buttons";

// ------------------------------------------------------------
- (id) init
{
  self = [super init];

  if (self) {
    lines_ = [NSMutableArray new];
    lastMessages_ = [NSMutableArray new];
    for (NSUInteger i = 0; i < BRIDGE_USERCLIENT_STATUS_MESSAGE__END__; ++i) {
      [lines_ addObject:@""];
      [lastMessages_ addObject:@""];
    }

    [GrowlApplicationBridge setGrowlDelegate:self];

    // Workaround for Growl problem.
    //
    // In the following case, the notification is not displayed after Growl started either.
    // (1) KeyRemap4MacBook is not registered with Growl.
    // (2) Growl is not running when we call setGrowlDelegate.
    //
    // We need to call setWillRegisterWhenGrowlIsReady:YES to prevent the above problem.
    // reregisterGrowlNotifications solves this problem.
    //
    // Note:
    // To reproduce this problem, do the following procedures.
    // (You need to comment out setWillRegisterWhenGrowlIsReady:YES below.)
    //
    // (1) Stop Growl.
    // (2) Delete KeyRemap4MacBook entry from Growl Preferences.
    // (3) Stop KeyRemap4MacBook server process.
    // (4) Start KeyRemap4MacBook server process.
    // (5) Start Growl.
    // (6) call notifyWithTitle in KeyRemap4MacBook server process.
    [GrowlApplicationBridge setWillRegisterWhenGrowlIsReady:YES];
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
// return YES if Growl is not running.
- (BOOL) displayGrowlNotRunningWarning
{
  NSString* message1 = @"KeyRemap4MacBook uses Growl to display extra messages.";

  NSString* message2 = nil;

  // If user uses Growl-1.3 and we use Growl SDK 1.2.2,
  // isGrowlInstalled returns NO even if Growl is installed.
  // Therefore, *DO NOT* call isGrowlInstalled here.

  if (! [GrowlApplicationBridge isGrowlRunning]) {
    message2 = @"* Growl is not running.\n* Please install or start Growl.";
  }
  if (! message2) {
    // clear isGrowlNotRunningWarningDisplayed_
    isGrowlNotRunningWarningDisplayed_ = NO;
    return NO;
  }

  NSString* message3 = @"(Activate \"General > Suppress Growl warning\" in System Preference to hide this message.)";

  if ([preferencesManager_ value:@"general.suppress_growl_warning"]) return YES;

  @synchronized(self) {
    if (! isGrowlNotRunningWarningDisplayed_) {
      isGrowlNotRunningWarningDisplayed_ = YES;

      NSAlert* alert = [[NSAlert new] autorelease];
      [alert setMessageText:@"KeyRemap4MacBook Warning"];
      [alert addButtonWithTitle:@"Close"];
      [alert setInformativeText:[NSString stringWithFormat:@"%@\n\n%@\n\n%@", message1, message2, message3]];

      [alert performSelectorOnMainThread:@selector(runModal)
                              withObject:nil
                           waitUntilDone:YES];
    }
  }

  return YES;
}

- (void) updateStatusMessage
{
  NSString* message = nil;

  // ------------------------------------------------------------
  // Modifier Message
  int indexes[] = {
    BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_LOCK,
    BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_STICKY,
    BRIDGE_USERCLIENT_STATUS_MESSAGE_POINTING_BUTTON_LOCK,
  };
  for (size_t i = 0; i < sizeof(indexes) / sizeof(indexes[0]); ++i) {
    int idx = indexes[i];
    message = [lines_ objectAtIndex:idx];

    BOOL isSticky = ([message length] > 0);

    if (! [message isEqualToString:[lastMessages_ objectAtIndex:idx]]) {
      [lastMessages_ replaceObjectAtIndex:idx withObject:message];

      if (! [self displayGrowlNotRunningWarning]) {
        NSString* name = nil;
        switch (idx) {
          case BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_LOCK:   name = notificationName_modifier_lock;   break;
          case BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_STICKY: name = notificationName_modifier_sticky; break;
          case BRIDGE_USERCLIENT_STATUS_MESSAGE_POINTING_BUTTON_LOCK: name = notificationName_pointing_button_lock; break;
        }

        [GrowlApplicationBridge
          notifyWithTitle:name
              description:message
         notificationName:name
                 iconData:nil
                 priority:0
                 isSticky:isSticky
             clickContext:nil
               identifier:[NSString stringWithFormat:@"org_pqrs_KeyRemap4MacBook_modifier_%d", idx]];
      }
    }
  }

  // ------------------------------------------------------------
  // Extra Message
  message = [lines_ objectAtIndex:BRIDGE_USERCLIENT_STATUS_MESSAGE_EXTRA];

  if ([message length] > 0) {
    [lastMessages_ replaceObjectAtIndex:BRIDGE_USERCLIENT_STATUS_MESSAGE_EXTRA withObject:message];

    if (! [self displayGrowlNotRunningWarning]) {
      [GrowlApplicationBridge
        notifyWithTitle:@"Enabling"
            description:message
       notificationName:notificationName_extra
               iconData:nil
               priority:0
               isSticky:YES
           clickContext:nil
             identifier:@"org_pqrs_KeyRemap4MacBook_extra"];
    }

  } else {
    message = [lastMessages_ objectAtIndex:BRIDGE_USERCLIENT_STATUS_MESSAGE_EXTRA];
    if ([message length] > 0) {
      if (! [self displayGrowlNotRunningWarning]) {
        [GrowlApplicationBridge
          notifyWithTitle:@"Disabling"
              description:message
         notificationName:notificationName_extra
                 iconData:nil
                 priority:0
                 isSticky:NO
             clickContext:nil
               identifier:@"org_pqrs_KeyRemap4MacBook_extra"];

        [lastMessages_ replaceObjectAtIndex:BRIDGE_USERCLIENT_STATUS_MESSAGE_EXTRA withObject:@""];
      }
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
  for (NSUInteger i = 0; i < BRIDGE_USERCLIENT_STATUS_MESSAGE__END__; ++i) {
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
  NSArray* array = [NSArray arrayWithObjects:notificationName_modifier_lock,
                    notificationName_modifier_sticky,
                    notificationName_pointing_button_lock,
                    notificationName_extra, nil];
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
  return [NSImage imageNamed:@"app.icon"];
}

@end
