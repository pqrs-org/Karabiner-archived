#import "StatusMessageManager.h"
#import "NotificationKeys.h"
#import "PreferencesModel.h"
#import "StatusMessageView_edge.h"
#import "StatusMessageView_nano.h"
#import "StatusMessageView_normal.h"
#include "bridge.h"
#import "weakify.h"

@interface StatusMessageManager ()

@property(weak) IBOutlet PreferencesModel* preferencesModel;

@property BOOL showExampleStatusWindow;
@property NSMutableArray* windowControllers;
@property NSMutableArray* lines;

@end

@implementation StatusMessageManager

// ------------------------------------------------------------
- (void)observer_NSApplicationDidChangeScreenParametersNotification:(NSNotification*)notification {
  @weakify(self);

  dispatch_async(dispatch_get_main_queue(), ^{
    @strongify(self);
    if (!self) return;

    [self refresh];
  });
}

- (instancetype)init {
  self = [super init];

  if (self) {
    self.showExampleStatusWindow = NO;

    self.windowControllers = [NSMutableArray new];
    self.lines = [NSMutableArray new];
    for (NSUInteger i = 0; i < BRIDGE_USERCLIENT_STATUS_MESSAGE__END__; ++i) {
      [self.lines addObject:@""];
    }

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_NSApplicationDidChangeScreenParametersNotification:)
                                                 name:NSApplicationDidChangeScreenParametersNotification
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
  [[[controller window] contentView] setAlphaValue:((double)(self.preferencesModel.statusWindowOpacity) / 100)];
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

  StatusMessageView* view = [window contentView];
  view.preferencesModel = self.preferencesModel;
  [view updateMessage:@""];
}

- (BOOL)isNeedReplaceWindows {
  // ----------------------------------------
  // Check count.
  if ([self.windowControllers count] == 0) {
    return YES;
  }

  if ([self.windowControllers count] != [[NSScreen screens] count]) {
    return YES;
  }

  // ----------------------------------------
  // Check view's class.
  Class expectedViewClass = nil;

  switch (self.preferencesModel.statusWindowType) {
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

  if (![[[self.windowControllers[0] window] contentView] isKindOfClass:expectedViewClass]) {
    return YES;
  }

  return NO;
}

- (NSString*)windowNibName {
  switch (self.preferencesModel.statusWindowType) {
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
    for (NSWindowController* controller in self.windowControllers) {
      [self hideStatusWindow:controller];
    }
    [self.windowControllers removeAllObjects];

    NSString* nibName = [self windowNibName];
    for (NSUInteger i = 0; i < screenCount; ++i) {
      NSWindowController* controller = [[NSWindowController alloc] initWithWindowNibName:nibName];
      [self setupStatusWindow:controller];
      [self.windowControllers addObject:controller];
    }
  }

  // updateWindowFrame
  if ([self.windowControllers count] == screenCount) {
    for (NSUInteger i = 0; i < screenCount; ++i) {
      [[[self.windowControllers[i] window] contentView] updateWindowFrame:screens[i]];
    }
  }
}

- (void)setupStatusMessageManager {
  [self updateWindows];
}

// ------------------------------------------------------------
- (void)refresh {
  [self updateWindows];

  if (!self.preferencesModel.useStatusWindow) {
    for (NSWindowController* controller in self.windowControllers) {
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
      if (!self.preferencesModel.showStickyModifiersStateInStatusWindow) {
        continue;
      }
    }
    if (idx == BRIDGE_USERCLIENT_STATUS_MESSAGE_POINTING_BUTTON_LOCK) {
      if (!self.preferencesModel.showPointingButtonLockStateInStatusWindow) {
        continue;
      }
    }

    NSString* message = self.lines[idx];

    // append caps lock status to modifier lock.
    if (self.preferencesModel.showCapsLockStateInStatusWindow) {
      if (idx == BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_LOCK) {
        NSString* capslock = self.lines[BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_CAPS_LOCK];
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
  NSString* message = self.lines[BRIDGE_USERCLIENT_STATUS_MESSAGE_EXTRA];

  [statusMessage appendString:message];

  // ------------------------------------------------------------
  // Show status message when Status Message on Preferences is shown.
  if ([statusMessage length] == 0 && self.showExampleStatusWindow) {
    [statusMessage appendString:@"Example"];
  }

  // ------------------------------------------------------------
  if ([statusMessage length] > 0) {
    for (NSWindowController* controller in self.windowControllers) {
      NSWindow* window = [controller window];
      [[window contentView] updateMessage:statusMessage];
      [window orderFront:self];
      [self showStatusWindow:controller];
      [[window contentView] setNeedsDisplay:YES];
    }
  } else {
    for (NSWindowController* controller in self.windowControllers) {
      [self hideStatusWindow:controller];
    }
  }
}

- (void)resetStatusMessage {
  for (NSUInteger i = 0; i < BRIDGE_USERCLIENT_STATUS_MESSAGE__END__; ++i) {
    self.lines[i] = @"";
  }

  [self refresh];
}

- (void)setStatusMessage:(NSUInteger)lineIndex message:(NSString*)message {
  self.lines[lineIndex] = message;
  [self refresh];
}

- (void)showExampleStatusWindow:(BOOL)visibility {
  self.showExampleStatusWindow = visibility;
  [self refresh];
}

@end
