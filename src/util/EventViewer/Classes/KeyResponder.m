// -*- Mode: objc -*-

#import "EventQueue.h"
#import "KeyResponder.h"

@implementation KeyResponder

- (void)drawRect:(NSRect)dirtyRect {
  [NSGraphicsContext saveGraphicsState];
  {
    NSRect bounds = [self bounds];

    CGFloat w = bounds.size.width / 40;
    NSRect text = NSMakeRect(w * 2,
                             w * 2,
                             bounds.size.width - w * 4,
                             bounds.size.height - w * 4);

    // Draw area
    [[[NSColor whiteColor] colorWithAlphaComponent:0.8] set];
    [[NSBezierPath bezierPathWithRoundedRect:bounds xRadius:10 yRadius:10] fill];

    [[[NSColor blackColor] colorWithAlphaComponent:0.6] set];
    NSBezierPath* path = [NSBezierPath bezierPathWithRoundedRect:bounds xRadius:10 yRadius:10];
    [path setLineWidth:w];
    [path stroke];

    // Draw texts
    NSDictionary* attributes = @{
      NSFontAttributeName : [NSFont boldSystemFontOfSize:(text.size.width / 10)],
      NSForegroundColorAttributeName : [[NSColor blackColor] colorWithAlphaComponent:0.6],
    };
    [@"Mouse Area" drawInRect:text withAttributes:attributes];
  }
  [NSGraphicsContext restoreGraphicsState];
}

// ============================================================
//
// Key event handlers
//

- (void)keyDown:(NSEvent*)event {
}

- (void)keyUp:(NSEvent*)event {
}

- (void)flagsChanged:(NSEvent*)event {
}

- (void)mouseDown:(NSEvent*)event {
  [eventQueue_ pushMouseEvent:event];
}

- (void)mouseUp:(NSEvent*)event {
  [eventQueue_ pushMouseEvent:event];
}

- (void)mouseDragged:(NSEvent*)event {
  [eventQueue_ pushMouseEvent:event];
}

- (void)rightMouseDown:(NSEvent*)event {
  [eventQueue_ pushMouseEvent:event];
}

- (void)rightMouseUp:(NSEvent*)event {
  [eventQueue_ pushMouseEvent:event];
}

- (void)rightMouseDragged:(NSEvent*)event {
  [eventQueue_ pushMouseEvent:event];
}

- (void)otherMouseDown:(NSEvent*)event {
  [eventQueue_ pushMouseEvent:event];
}

- (void)otherMouseUp:(NSEvent*)event {
  [eventQueue_ pushMouseEvent:event];
}

- (void)otherMouseDragged:(NSEvent*)event {
  [eventQueue_ pushMouseEvent:event];
}

- (void)scrollWheel:(NSEvent*)event {
  [eventQueue_ pushMouseEvent:event];
}

- (void)swipeWithEvent:(NSEvent*)event {
}

@end
