#import "NotificationKeys.h"
#import "PreferencesKeys.h"
#import "StatusMessageView_edge.h"
#import "StatusMessageView_nano.h"
#import "StatusMessageView_normal.h"
#import "StatusMessageManager.h"
#include "bridge.h"

@interface StatusMessageManager () {
  BOOL statusWindowPreferencesOpened_;
  NSMutableArray* windowControllers_;
  NSMutableArray* lines_;
  NSMutableArray* lastMessages_;
}
@end

@implementation StatusMessageManager

// ------------------------------------------------------------
- (void)observer_NSApplicationDidChangeScreenParametersNotification:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self refresh:self];
  });
}

- (void)observer_StatusWindowPreferencesOpened:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    statusWindowPreferencesOpened_ = YES;
    [self refresh:self];
  });
}

- (void)observer_StatusWindowPreferencesClosed:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    statusWindowPreferencesOpened_ = NO;
    [self refresh:self];
  });
}

- (id)init {
  self = [super init];

  if (self) {
    statusWindowPreferencesOpened_ = NO;

    windowControllers_ = [NSMutableArray new];
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
                                                 name:kStatusWindowPreferencesOpenedNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_StatusWindowPreferencesClosed:)
                                                 name:kStatusWindowPreferencesClosedNotification
                                               object:nil];
  }

  return self;
}

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

// ------------------------------------------------------------
- (void)hideStatusWindow:(NSWindowController*)controller {
  // On OS X 10.9.2 and multi display environment,
  // if we set NSWindowCollectionBehaviorStationary to NSWindow,
  // the window becomes invisible when we call methods by the following procedures.
  // (maybe a bug of OS X.)
  //
  //   1. [window orderOut]
  //   2. [window setFrameOrigin] (move window to another screen)
  //   3. [window orderFront]
  //
  // So, we need to set alpha value zero in order to hide window.
  //
  // Note:
  // Do not change NSWindow's alpha value.
  // Change contentView's alpha value instead.
  // Setting NSWindow's alpha value causes flickering when you move a space.

  [[[controller window] contentView] setAlphaValue:0];
}

- (void)showStatusWindow:(NSWindowController*)controller {
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  double opacity = [defaults doubleForKey:kStatusWindowOpacity];
  [[[controller window] contentView] setAlphaValue:(opacity / 100)];
}

- (void)setupStatusWindow:(NSWindowController*)controller {
  NSWindow* window = [controller window];

  NSWindowCollectionBehavior behavior = NSWindowCollectionBehaviorCanJoinAllSpaces |
                                        NSWindowCollectionBehaviorStationary |
                                        NSWindowCollectionBehaviorIgnoresCycle;

  [self hideStatusWindow:controller];
  [window setBackgroundColor:[NSColor clearColor]];
  [window setOpaque:NO];
  [window setHasShadow:NO];
  [window setStyleMask:NSBorderlessWindowMask];
  [window setLevel:NSStatusWindowLevel];
  [window setIgnoresMouseEvents:YES];
  [window setCollectionBehavior:behavior];

  [[window contentView] setMessage:@""];
}

- (BOOL)isNeedReplaceWindows {
  // ----------------------------------------
  // Check count.
  if ([windowControllers_ count] == 0) {
    return YES;
  }

  if ([windowControllers_ count] != [[NSScreen screens] count]) {
    return YES;
  }

  // ----------------------------------------
  // Check view's class.
  Class expectedViewClass = nil;

  switch ([[NSUserDefaults standardUserDefaults] integerForKey:kStatusWindowType]) {
  case 1:
    expectedViewClass = [StatusMessageView_nano class];
    break;

  case 2:
    expectedViewClass = [StatusMessageView_edge class];
    break;

  case 0:
  default:
    expectedViewClass = [StatusMessageView_normal class];
    break;
  }

  if (![[[windowControllers_[0] window] contentView] isKindOfClass:expectedViewClass]) {
    return YES;
  }

  return NO;
}

- (NSString*)windowNibName {
  switch ([[NSUserDefaults standardUserDefaults] integerForKey:kStatusWindowType]) {
  case 1:
    return @"StatusMessageNanoWindow";

  case 2:
    return @"StatusMessageEdgeWindow";

  case 0:
  default:
    return @"StatusMessageNormalWindow";
  }
}

- (void)updateWindows {
  NSArray* screens = [NSScreen screens];
  NSUInteger screenCount = [screens count];

  if ([self isNeedReplaceWindows]) {
    for (NSWindowController* controller in windowControllers_) {
      [self hideStatusWindow:controller];
    }
    [windowControllers_ removeAllObjects];

    NSString* nibName = [self windowNibName];
    for (NSUInteger i = 0; i < screenCount; ++i) {
      NSWindowController* controller = [[NSWindowController alloc] initWithWindowNibName:nibName];
      [self setupStatusWindow:controller];
      [windowControllers_ addObject:controller];
    }
  }

  // updateWindowFrame
  if ([windowControllers_ count] == screenCount) {
    for (NSUInteger i = 0; i < screenCount; ++i) {
      [[[windowControllers_[i] window] contentView] updateWindowFrame:screens[i]];
    }
  }
}

- (void)setupStatusMessageManager {
  [self updateWindows];
}

// ------------------------------------------------------------
- (IBAction)refresh:(id)sender;
{
  [self updateWindows];

  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
  if (![defaults boolForKey:kIsStatusWindowEnabled]) {
    for (NSWindowController* controller in windowControllers_) {
      [self hideStatusWindow:controller];
    }
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
      if (![defaults boolForKey:kIsStatusWindowShowStickyModifier]) {
        continue;
      }
    }
    if (idx == BRIDGE_USERCLIENT_STATUS_MESSAGE_POINTING_BUTTON_LOCK) {
      if (![defaults boolForKey:kIsStatusWindowShowPointingButtonLock]) {
        continue;
      }
    }

    NSString* message = lines_[idx];

    // append caps lock status to modifier lock.
    if ([defaults boolForKey:kIsStatusWindowShowCapsLock]) {
      if (idx == BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_LOCK) {
        NSString* capslock = lines_[BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_CAPS_LOCK];
        if ([capslock length] > 0) {
          message = [NSString stringWithFormat:@"%@%@", capslock, message];
        }
      }
    }

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
  if ([statusMessage length] > 0) {
    for (NSWindowController* controller in windowControllers_) {
      NSWindow* window = [controller window];
      [[window contentView] setMessage:statusMessage];
      [window orderFront:self];
      [self showStatusWindow:controller];
      [[window contentView] setNeedsDisplay:YES];
    }
  } else {
    for (NSWindowController* controller in windowControllers_) {
      [self hideStatusWindow:controller];
    }
  }
}

- (void)resetStatusMessage {
  for (NSUInteger i = 0; i < BRIDGE_USERCLIENT_STATUS_MESSAGE__END__; ++i) {
    lines_[i] = @"";
  }

  [self refresh:self];
}

- (void)setStatusMessage:(NSUInteger)lineIndex message:(NSString*)message {
  lines_[lineIndex] = message;
  [self refresh:self];
}

@end
