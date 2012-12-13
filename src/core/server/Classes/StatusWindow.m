#import "StatusWindow.h"
#include "bridge.h"

@implementation StatusWindow

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
- (void) setupStatusWindow {
  NSWindowCollectionBehavior behavior = NSWindowCollectionBehaviorCanJoinAllSpaces |
                                        NSWindowCollectionBehaviorStationary |
                                        NSWindowCollectionBehaviorIgnoresCycle;

  [window_ setBackgroundColor:[NSColor clearColor]];
  [window_ setOpaque:NO];
  [window_ setStyleMask:NSBorderlessWindowMask];
  [window_ setLevel:NSStatusWindowLevel];
  [window_ setIgnoresMouseEvents:YES];
  [window_ setCollectionBehavior:behavior];
  [self updateFrameOrigin];
  [window_ setAlphaValue:0.0];
  [window_ orderFront:self];

  // ------------------------------------------------------------
  // [statusMessageView_ setAlphaValue:(CGFloat)(0.8)];
  [statusMessageView_ setMessage:@""];
}

- (void) updateFrameOrigin {
  NSRect screenFrame = [[NSScreen mainScreen] visibleFrame];
  NSRect windowFrame = [window_ frame];
  int margin = 10;
  [window_ setFrameOrigin:NSMakePoint(screenFrame.size.width - windowFrame.size.width - margin,
                                      screenFrame.origin.y + margin)];
}

// ------------------------------------------------------------
- (void) updateStatusMessage
{
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
  [statusMessageView_ setMessage:statusMessage];

  // ------------------------------------------------------------
  NSViewAnimation* animation = [[NSViewAnimation new] autorelease];
  [animation setAnimationBlockingMode:NSAnimationNonblocking];
  [animation setAnimationCurve:NSAnimationEaseIn];
  [animation setDuration:0.1];

  if ([statusMessage length] > 0) {
    NSDictionary* dict = @ { NSViewAnimationTargetKey : window_,
                             NSViewAnimationEffectKey : NSViewAnimationFadeInEffect, };
    [animation setViewAnimations:@[dict]];
    [animation startAnimation];

  } else {
    NSDictionary* dict = @ { NSViewAnimationTargetKey : window_,
                             NSViewAnimationEffectKey : NSViewAnimationFadeOutEffect, };
    [animation setViewAnimations:@[dict]];
    [animation startAnimation];
  }
}

- (void) resetStatusMessage
{
  for (NSUInteger i = 0; i < BRIDGE_USERCLIENT_STATUS_MESSAGE__END__; ++i) {
    [lines_ replaceObjectAtIndex:i withObject:@""];
  }

  [self updateStatusMessage];
}

- (void) setStatusMessage:(NSUInteger)lineIndex message:(NSString*)message
{
  [lines_ replaceObjectAtIndex:lineIndex withObject:message];
  [self updateStatusMessage];
}

@end
