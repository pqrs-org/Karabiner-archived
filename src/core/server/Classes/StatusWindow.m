#import "NotificationKeys.h"
#import "PreferencesKeys.h"
#import "StatusMessageView.h"
#import "StatusWindow.h"
#include "bridge.h"

@implementation StatusWindow

// ------------------------------------------------------------
- (void) observer_NSApplicationDidChangeScreenParametersNotification:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
                   [self updateFrameOrigin];
                 });
}

- (void) observer_StatusWindowPreferencesOpened:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
                   statusWindowPreferencesOpened_ = YES;
                   [self refresh:self];
                 });
}

- (void) observer_StatusWindowPreferencesClosed:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
                   statusWindowPreferencesOpened_ = NO;
                   [self refresh:self];
                 });
}

- (id) init
{
  self = [super init];

  if (self) {
    statusWindowPreferencesOpened_ = NO;
    lines_ = [NSMutableArray new];
    lastMessages_ = [NSMutableArray new];
    for (NSUInteger i = 0; i < BRIDGE_USERCLIENT_STATUS_MESSAGE__END__; ++i) {
      [lines_ addObject:@""];
      [lastMessages_ addObject:@""];
    }

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_NSApplicationDidChangeScreenParametersNotification:)
                                                 name:NSApplicationDidChangeScreenParametersNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_StatusWindowPreferencesOpened:)
                                                 name:kStatusWindowPreferencesOpenedNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_StatusWindowPreferencesClosed:)
                                                 name:kStatusWindowPreferencesClosedNotification object:nil];
  }

  return self;
}

- (void) dealloc
{
  [[NSNotificationCenter defaultCenter] removeObserver:self];

  [lines_ release];
  [lastMessages_ release];

  [super dealloc];
}

// ------------------------------------------------------------
- (void) setupStatusWindow {
  NSWindowCollectionBehavior behavior = NSWindowCollectionBehaviorCanJoinAllSpaces |
                                        NSWindowCollectionBehaviorStationary |
                                        NSWindowCollectionBehaviorIgnoresCycle;

  [window_ orderOut:self];
  [window_ setBackgroundColor:[NSColor clearColor]];
  [window_ setOpaque:NO];
  [window_ setStyleMask:NSBorderlessWindowMask];
  [window_ setLevel:NSStatusWindowLevel];
  [window_ setIgnoresMouseEvents:YES];
  [window_ setCollectionBehavior:behavior];
  [self updateFrameOrigin];

  // ------------------------------------------------------------
  [statusMessageView_ setMessage:@""];
}

// ------------------------------------------------------------
- (IBAction) refresh:(id)sender;
{
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  if (! [defaults boolForKey:kIsStatusWindowEnabled]) {
    [window_ orderOut:self];
    return;
  }

  // ------------------------------------------------------------
  NSMutableString* statusMessage = [NSMutableString string];

  // ------------------------------------------------------------
  // Modifier Message
  int indexes[] = {
    BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_LOCK,
    BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_STICKY,
    BRIDGE_USERCLIENT_STATUS_MESSAGE_POINTING_BUTTON_LOCK,
  };
  for (size_t i = 0; i < sizeof(indexes) / sizeof(indexes[0]); ++i) {
    int idx = indexes[i];

    // Skip message if configured as hide.
    if (idx == BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_STICKY) {
      if (! [defaults boolForKey:kIsStatusWindowShowStickyModifier]) {
        continue;
      }
    }
    if (idx == BRIDGE_USERCLIENT_STATUS_MESSAGE_POINTING_BUTTON_LOCK) {
      if (! [defaults boolForKey:kIsStatusWindowShowPointingButtonLock]) {
        continue;
      }
    }

    NSString* message = [lines_ objectAtIndex:idx];
    if ([message length] > 0) {
      NSString* name = nil;
      switch (idx) {
        case BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_LOCK:
          name = @"Lock";
          break;
        case BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_STICKY:
          name = @"Sticky";
          break;
        case BRIDGE_USERCLIENT_STATUS_MESSAGE_POINTING_BUTTON_LOCK:
          name = @"Click Lock";
          break;
      }
      if (name) {
        [statusMessage appendFormat:@"%@: %@\n", name, message];
      }
    }
  }

  // ------------------------------------------------------------
  // Extra Message
  NSString* message = [lines_ objectAtIndex:BRIDGE_USERCLIENT_STATUS_MESSAGE_EXTRA];

  [statusMessage appendString:message];

  // ------------------------------------------------------------
  // Show status message when Status Message on Preferences is shown.
  if ([statusMessage length] == 0 &&
      statusWindowPreferencesOpened_) {
    [statusMessage appendString:@"Example"];
  }

  // ------------------------------------------------------------
  [statusMessageView_ setMessage:statusMessage];

  if ([statusMessage length] > 0) {
    [self updateFrameOrigin];
    [window_ orderFront:self];
    [statusMessageView_ setNeedsDisplay:YES];
  } else {
    [window_ orderOut:self];
  }
}

- (void) resetStatusMessage
{
  for (NSUInteger i = 0; i < BRIDGE_USERCLIENT_STATUS_MESSAGE__END__; ++i) {
    [lines_ replaceObjectAtIndex:i withObject:@""];
  }

  [self refresh:self];
}

- (void) setStatusMessage:(NSUInteger)lineIndex message:(NSString*)message
{
  [lines_ replaceObjectAtIndex:lineIndex withObject:message];
  [self refresh:self];
}

- (void) updateFrameOrigin {
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  NSInteger position = [defaults integerForKey:kStatusWindowPosition];

  NSRect screenFrame = [[NSScreen mainScreen] visibleFrame];
  NSRect windowFrame = [window_ frame];
  int margin = 10;
  NSPoint point;

  switch (position) {
    case 0:
      // Top left
      point.x = screenFrame.origin.x + margin;
      point.y = screenFrame.origin.y + screenFrame.size.height - windowFrame.size.height - margin;
      break;
    case 1:
      // Top right
      point.x = screenFrame.origin.x + screenFrame.size.width  - windowFrame.size.width - margin,
      point.y = screenFrame.origin.y + screenFrame.size.height - windowFrame.size.height - margin;
      break;
    case 2:
      // Bottom left
      point.x = screenFrame.origin.x + margin;
      point.y = screenFrame.origin.y + margin;
      break;
    case 3:
    default:
      // Bottom right
      point.x = screenFrame.origin.x + screenFrame.size.width  - windowFrame.size.width - margin,
      point.y = screenFrame.origin.y + margin;
      break;
  }

  [window_ setFrameOrigin:point];
}

@end
