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

    // ------------------------------------------------------------
    animation_ = [NSViewAnimation new];
    [animation_ setAnimationBlockingMode:NSAnimationNonblocking];
    [animation_ setAnimationCurve:NSAnimationEaseIn];
    [animation_ setDuration:0];

    // ------------------------------------------------------------
    [GrowlApplicationBridge setGrowlDelegate:self];
  }

  return self;
}

- (void) dealloc
{
  [lines_ release];
  [animation_ release];
  [messageForGrowl_ release];

  [super dealloc];
}

- (void) setupStatusWindow {
  NSWindowCollectionBehavior behavior = NSWindowCollectionBehaviorCanJoinAllSpaces |
                                        NSWindowCollectionBehaviorStationary |
                                        NSWindowCollectionBehaviorIgnoresCycle;

  [statuswindow_ setBackgroundColor:[NSColor clearColor]];
  [statuswindow_ setOpaque:NO];
  [statuswindow_ setStyleMask:NSBorderlessWindowMask];
  [statuswindow_ setLevel:NSStatusWindowLevel];
  [statuswindow_ setIgnoresMouseEvents:YES];
  [statuswindow_ setCollectionBehavior:behavior];
  [statuswindow_ center];

  // ------------------------------------------------------------
  NSMutableDictionary* dict = [[NSMutableDictionary new] autorelease];
  [dict setObject:statuswindow_ forKey:NSViewAnimationTargetKey];
  [dict setObject:NSViewAnimationFadeOutEffect forKey:NSViewAnimationEffectKey];
  [animation_ setViewAnimations:[NSArray arrayWithObjects:dict, nil]];
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

  [label_ setStringValue:message];

  if ([message length] > 0) {
    // show

    if (! [preferencesmanager_ value:@"general.use_growl_instead_of_statuswindow"]) {
      [statuswindow_ orderFront:nil];

      [animation_ stopAnimation];
      [animation_ setCurrentProgress:0];
      if ([animation_ duration] > 0) {
        [animation_ startAnimation];
      } else {
        [statuswindow_ setAlphaValue:(CGFloat)(1.0)];
      }

    } else {
      // use Growl
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
    }

  } else {
    // hide

    if (! [preferencesmanager_ value:@"general.use_growl_instead_of_statuswindow"]) {
      [statuswindow_ orderOut:nil];

    } else {
      // use Growl
      if (messageForGrowl_) {
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

// ------------------------------------------------------------
- (void) refreshWindowPosition:(BOOL)force
{
  int alpha_font       = [preferencesmanager_ value:@"parameter.statuswindow_alpha_font"];
  int alpha_background = [preferencesmanager_ value:@"parameter.statuswindow_alpha_background"];
  int posx_adjustment  = [preferencesmanager_ value:@"parameter.statuswindow_posx_adjustment"];
  int posy_adjustment  = [preferencesmanager_ value:@"parameter.statuswindow_posy_adjustment"];
  int fadeout_duration = [preferencesmanager_ value:@"parameter.statuswindow_fadeout_duration"];

  if (! force &&
      alpha_font       == last_parameter_statuswindow_alpha_font_ &&
      alpha_background == last_parameter_statuswindow_alpha_background_ &&
      posx_adjustment  == last_parameter_statuswindow_posx_adjustment_ &&
      posy_adjustment  == last_parameter_statuswindow_posy_adjustment_ &&
      fadeout_duration == last_parameter_statuswindow_fadeout_duration_) {
    return;
  }
  last_parameter_statuswindow_alpha_font_       = alpha_font;
  last_parameter_statuswindow_alpha_background_ = alpha_background;
  last_parameter_statuswindow_posx_adjustment_  = posx_adjustment;
  last_parameter_statuswindow_posy_adjustment_  = posy_adjustment;
  last_parameter_statuswindow_fadeout_duration_ = fadeout_duration;

  // ----------------------------------------------------------------------
  CGFloat af = (CGFloat)(alpha_font) / (CGFloat)(100.0);
  CGFloat ab = (CGFloat)(alpha_background) / (CGFloat)(100.0);

  if (af < 0) af = 0;
  if (af > 100) af = 100;

  if (ab < 0) ab = 0;
  if (ab > 100) ab = 100;

  [label_ setAlphaValue:af];
  [backgroud_ setAlphaValue:ab];

  [statuswindow_ center];
  NSRect frame = [statuswindow_ frame];
  frame.origin.x += posx_adjustment;
  frame.origin.y += posy_adjustment;
  [statuswindow_ setFrameOrigin:frame.origin];

  NSTimeInterval duration = (NSTimeInterval)(fadeout_duration) / 1000;
  if (duration < 0) duration = 0;
  [animation_ setDuration:duration];
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
