#import "CheckboxCellView.h"
#import "CheckboxOutlineView.h"

@interface CheckboxOutlineView ()
@property(weak) CheckboxCellView* mouseDownCheckboxCellView;
@end

@implementation CheckboxOutlineView

- (void)mouseDown:(NSEvent*)theEvent {
  NSPoint point = [self convertPoint:theEvent.locationInWindow fromView:nil];
  NSInteger row = [self rowAtPoint:point];
  if (row >= 0) {
    self.mouseDownCheckboxCellView = [self viewAtColumn:0 row:row makeIfNecessary:NO];
  } else {
    self.mouseDownCheckboxCellView = nil;
  }
}

- (void)mouseUp:(NSEvent*)theEvent {
  NSPoint point = [self convertPoint:theEvent.locationInWindow fromView:nil];
  NSInteger row = [self rowAtPoint:point];
  if (row >= 0) {
    CheckboxCellView* view = [self viewAtColumn:0 row:row makeIfNecessary:NO];
    if (self.mouseDownCheckboxCellView == view) {
      // clicked
      [view toggle];
    }
    self.mouseDownCheckboxCellView = view;
  } else {
    self.mouseDownCheckboxCellView = nil;
  }
}

@end
