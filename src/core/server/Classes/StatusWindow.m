#import "StatusWindow.h"
#import "server_objc_part.h"

@implementation StatusWindow

- (id) init
{
  self = [super init];

  if (self) {
    lines_ = [NSMutableArray new];
    for (NSUInteger i = 0; i < STATUSMESSAGETYPE__END__; ++i) {
      [lines_ addObject:@""];
    }

    [GrowlApplicationBridge setGrowlDelegate:self];
  }

  return self;
}

- (void) dealloc
{
  [lines_ release];
  [messageForGrowl_ release];

  [super dealloc];
}

// ------------------------------------------------------------
- (void) updateStatusMessage
{
  NSMutableString* message = [[NSMutableString new] autorelease];

  for (NSUInteger i = 0; i < STATUSMESSAGETYPE__END__; ++i) {
    NSString* s = [lines_ objectAtIndex:i];
    if ([s length] > 0) {
      [message appendString:s];
      [message appendString:@"\n"];
    }
  }

  if ([message length] > 0) {
    [messageForGrowl_ release];
    messageForGrowl_ = [[NSString alloc] initWithString:message];

    [GrowlApplicationBridge
        notifyWithTitle:@"Enabling"
            description:messageForGrowl_
       notificationName:@"Enabled"
               iconData:nil
               priority:0
               isSticky:NO
           clickContext:nil
             identifier:@"org_pqrs_KeyRemap4MacBook"];

  } else {
    if (! messageForGrowl_) {
      messageForGrowl_ = [[NSString alloc] initWithString:@""];
    }

    [GrowlApplicationBridge
          notifyWithTitle:@"Disabling"
              description:messageForGrowl_
         notificationName:@"Disabled"
                 iconData:nil
                 priority:0
                 isSticky:NO
             clickContext:nil
               identifier:@"org_pqrs_KeyRemap4MacBook"];
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
  NSArray* array = [NSArray arrayWithObjects:@"Enabled", @"Disabled", nil];
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
