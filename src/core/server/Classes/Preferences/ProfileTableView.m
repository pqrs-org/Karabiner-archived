#import "ProfileTableView.h"

@implementation ProfileTableView

- (BOOL)validateProposedFirstResponder:(NSResponder*)responder forEvent:(NSEvent*)event {
  return YES;
}

- (BOOL)acceptsFirstMouse:(NSEvent*)theEvent {
  return NO;
}

- (void)mouseDown:(NSEvent*)theEvent {
}

- (void)mouseUp:(NSEvent*)theEvent {
}

@end
