#import "CheckboxOutlineView.h"
#import "CheckboxCellView.h"
#import "SharedXMLCompilerTree.h"

@interface CheckboxOutlineView ()

@property(weak) CheckboxCellView* mouseDownCheckboxCellView;

@end

@implementation CheckboxOutlineView

- (BOOL)acceptsFirstMouse:(NSEvent*)theEvent {
  return NO;
}

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
      SharedXMLCompilerTree* item = [self itemAtRow:row];
      if ([self isExpandable:item]) {
        if ([self isItemExpanded:item]) {
          if ([self selectedRow] == row) {
            [self collapseItem:item];
          } else {
            [self selectRowIndexes:[NSIndexSet indexSetWithIndex:(NSUInteger)(row)] byExtendingSelection:NO];
          }
        } else {
          [self selectRowIndexes:[NSIndexSet indexSetWithIndex:(NSUInteger)(row)] byExtendingSelection:NO];
          [self expandItem:item];
        }
      } else {
        [view toggle];
      }
      [[self window] makeFirstResponder:self];
    }
  }
  self.mouseDownCheckboxCellView = nil;
}

@end
