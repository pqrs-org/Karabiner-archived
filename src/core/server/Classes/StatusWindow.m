#import "StatusWindow.h"
#import "server_objc_part.h"
#include "bridge.h"

static StatusWindow* global_instance = nil;

@implementation StatusWindow

NSString* notificationName_extra = @"Extra Message";
NSString* notificationName_lock  = @"Modifier Lock";

- (id) init
{
  self = [super init];

  if (self) {
    lines_ = [NSMutableArray new];
    lastMessages_ = [NSMutableArray new];
    for (NSUInteger i = 0; i < BRIDGE_USERCLIENT_NOTIFICATION_DATA_STATUS_MESSAGE__END__; ++i) {
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
    // (3) Stop KeyRemap4MacBook_server.
    // (4) Start KeyRemap4MacBook_server.
    // (5) Start Growl.
    // (6) call notifyWithTitle in KeyRemap4MacBook_server.
    [GrowlApplicationBridge setWillRegisterWhenGrowlIsReady:YES];
  }

  global_instance = self;

  return self;
}

- (void) dealloc
{
  global_instance = nil;

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
  /*  */ if (! [GrowlApplicationBridge isGrowlInstalled]) {
    message2 = @"* Growl is not installed.\n* Please install Growl and restart the system.";
  } else if (! [GrowlApplicationBridge isGrowlRunning]) {
    message2 = @"* Growl is not running.\n* Please start Growl.";
  }
  if (! message2) {
    // clear isGrowlNotRunningWarningDisplayed_
    isGrowlNotRunningWarningDisplayed_ = NO;
    return NO;
  }

  NSString* message3 = @"(Activate \"General > Suppress Growl warning\" in System Preference to hide this message.)";

  if ([preferencesmanager_ value:@"general.suppress_growl_warning"]) return YES;

  @synchronized(self) {
    if (! isGrowlNotRunningWarningDisplayed_) {
      isGrowlNotRunningWarningDisplayed_ = YES;

      NSAlert* alert = [NSAlert alertWithMessageText:@"KeyRemap4MacBook Warning"
                                       defaultButton:@"Close"
                                     alternateButton:nil
                                         otherButton:nil
                           informativeTextWithFormat:[NSString stringWithFormat:@"%@\n\n%@\n\n%@", message1, message2, message3]];

      [alert runModal];
    }
  }

  return YES;
}

- (void) updateStatusMessage
{
  NSString* message = nil;

  // ------------------------------------------------------------
  // Modifier Message
  message = [lines_ objectAtIndex:BRIDGE_USERCLIENT_NOTIFICATION_DATA_STATUS_MESSAGE_MODIFIER];

  BOOL isSticky = ([message length] > 0);

  if (! [message isEqualToString:[lastMessages_ objectAtIndex:BRIDGE_USERCLIENT_NOTIFICATION_DATA_STATUS_MESSAGE_MODIFIER]]) {
    [lastMessages_ replaceObjectAtIndex:BRIDGE_USERCLIENT_NOTIFICATION_DATA_STATUS_MESSAGE_MODIFIER withObject:message];

    if (! [self displayGrowlNotRunningWarning]) {
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
  }

  // ------------------------------------------------------------
  // Extra Message
  message = [lines_ objectAtIndex:BRIDGE_USERCLIENT_NOTIFICATION_DATA_STATUS_MESSAGE_EXTRA];

  if ([message length] > 0) {
    [lastMessages_ replaceObjectAtIndex:BRIDGE_USERCLIENT_NOTIFICATION_DATA_STATUS_MESSAGE_EXTRA withObject:message];

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
    message = [lastMessages_ objectAtIndex:BRIDGE_USERCLIENT_NOTIFICATION_DATA_STATUS_MESSAGE_EXTRA];
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

        [lastMessages_ replaceObjectAtIndex:BRIDGE_USERCLIENT_NOTIFICATION_DATA_STATUS_MESSAGE_EXTRA withObject:@""];
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
  for (NSUInteger i = 0; i < BRIDGE_USERCLIENT_NOTIFICATION_DATA_STATUS_MESSAGE__END__; ++i) {
    [lines_ replaceObjectAtIndex:i withObject:@""];
  }

  [self callUpdateStatusMessage];
}

- (void) setStatusMessage:(NSUInteger)lineIndex message:(NSString*)message
{
  [lines_ replaceObjectAtIndex:lineIndex withObject:message];
  [self callUpdateStatusMessage];
}

+ (StatusWindow*) getInstance
{
  return global_instance;
}

// ============================================================
// Growl delegate
- (NSDictionary*) registrationDictionaryForGrowl
{
  NSArray* array = [NSArray arrayWithObjects:notificationName_lock, notificationName_extra, nil];
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
