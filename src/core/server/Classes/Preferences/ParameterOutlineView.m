#import "ParameterOutlineView.h"
#import "XMLCompiler.h"

@interface ParameterOutlineView ()
@property(weak) NSTableCellView* mouseDownCellView;
@end

@implementation ParameterOutlineView

- (BOOL)validateProposedFirstResponder:(NSResponder*)responder forEvent:(NSEvent*)event {
  return YES;
}

- (void)mouseDown:(NSEvent*)theEvent {
  NSPoint point = [self convertPoint:theEvent.locationInWindow fromView:nil];
  NSInteger row = [self rowAtPoint:point];
  NSInteger column = [self columnAtPoint:point];
  if (column == 0 && row >= 0) {
    self.mouseDownCellView = [self viewAtColumn:column row:row makeIfNecessary:NO];
  } else {
    self.mouseDownCellView = nil;
  }
}

- (void)mouseUp:(NSEvent*)theEvent {
  NSPoint point = [self convertPoint:theEvent.locationInWindow fromView:nil];
  NSInteger row = [self rowAtPoint:point];
  NSInteger column = [self columnAtPoint:point];
  if (column == 0 && row >= 0) {
    NSTableCellView* view = [self viewAtColumn:column row:row makeIfNecessary:NO];
    if (self.mouseDownCellView == view) {
      // clicked
      XMLCompilerTree* item = [self itemAtRow:row];
      if ([self isExpandable:item]) {
        [self selectRowIndexes:[NSIndexSet indexSetWithIndex:(NSUInteger)(row)] byExtendingSelection:NO];
        if ([self isItemExpanded:item]) {
          [self collapseItem:item];
        } else {
          [self expandItem:item];
        }
        [[self window] makeFirstResponder:self];
      }
    }
  }
  self.mouseDownCellView = nil;
}

@end
