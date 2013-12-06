#import "NotificationKeys.h"
#import "PreferencesKeys.h"
#import "StatusMessageView_normal.h"
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

    windows_ = [NSMutableArray new];
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


}

// ------------------------------------------------------------
- (void) setupStatusWindow:(NSWindow*)window
{
  NSWindowCollectionBehavior behavior = NSWindowCollectionBehaviorCanJoinAllSpaces |
                                        NSWindowCollectionBehaviorStationary |
                                        NSWindowCollectionBehaviorIgnoresCycle;

  [window orderOut:self];
  [window setBackgroundColor:[NSColor clearColor]];
  [window setOpaque:NO];
  [window setStyleMask:NSBorderlessWindowMask];
  [window setLevel:NSStatusWindowLevel];
  [window setIgnoresMouseEvents:YES];
  [window setCollectionBehavior:behavior];

  [[window contentView] setMessage:@""];
}

- (void) setupStatusWindow
{
  if ([windows_ count] == 0) {
    [windows_ addObject:statusMessage_normal_];
    [windows_ addObject:statusMessage_nano_];
    [windows_ addObject:statusMessage_edge_];
  }

  for (NSWindow* window in windows_) {
    [self setupStatusWindow:window];
  }
  [self updateFrameOrigin];
}

// ------------------------------------------------------------
- (IBAction) refresh:(id)sender;
{
  NSWindow* window = [self currentWindow];
  // Hide windows which have an unselected type.
  for (NSWindow* w in windows_) {
    if (w != window) {
      [w orderOut:self];
    }
  }

  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  if (! [defaults boolForKey:kIsStatusWindowEnabled]) {
    [window orderOut:self];
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

    NSString* message = lines_[idx];
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
  NSString* message = lines_[BRIDGE_USERCLIENT_STATUS_MESSAGE_EXTRA];

  [statusMessage appendString:message];

  // ------------------------------------------------------------
  // Show status message when Status Message on Preferences is shown.
  if ([statusMessage length] == 0 &&
      statusWindowPreferencesOpened_) {
    [statusMessage appendString:@"Example"];
  }

  // ------------------------------------------------------------
  [[[self currentWindow] contentView] setMessage:statusMessage];

  if ([statusMessage length] > 0) {
    [self updateFrameOrigin];
    [window orderFront:self];
    [[[self currentWindow] contentView] setNeedsDisplay:YES];
  } else {
    [window orderOut:self];
  }
}

- (void) resetStatusMessage
{
  for (NSUInteger i = 0; i < BRIDGE_USERCLIENT_STATUS_MESSAGE__END__; ++i) {
    lines_[i] = @"";
  }

  [self refresh:self];
}

- (void) setStatusMessage:(NSUInteger)lineIndex message:(NSString*)message
{
  lines_[lineIndex] = message;
  [self refresh:self];
}

- (NSWindow*) currentWindow {
  @try {
    return windows_[[[NSUserDefaults standardUserDefaults] integerForKey:kStatusWindowType]];
  } @catch (NSException* exception) {
    return nil;
  }
}

- (void) updateFrameOrigin {
  for (NSWindow* window in windows_) {
    [[window contentView] updateWindowFrame];
  }
}

@end
