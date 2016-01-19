#import "CheckboxCellView.h"
#import "CheckboxOutlineView.h"

@implementation CheckboxOutlineView

- (void)mouseDown:(NSEvent*)theEvent {
  [super mouseDown:theEvent];

  // toggle checkbox
  NSPoint point = [self convertPoint:theEvent.locationInWindow fromView:nil];
  NSInteger row = [self rowAtPoint:point];
  if (row >= 0) {
    CheckboxCellView* view = [self viewAtColumn:0 row:row makeIfNecessary:NO];
    [view toggleCheckboxState];
  }
}

@end
