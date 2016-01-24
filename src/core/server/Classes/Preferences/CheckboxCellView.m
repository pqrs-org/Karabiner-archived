#import "CheckboxCellView.h"
#import "CheckboxOutlineViewDelegate.h"
#import "PreferencesManager.h"

@implementation CheckboxCellView

- (IBAction)valueChanged:(id)sender {
  if (self.checkbox.imagePosition != NSNoImage) {
    int value = (self.checkbox.state == NSOnState);
    [self.checkboxOutlineViewDelegate.preferencesManager setValue:value forName:self.settingIdentifier];
  }

  [self selectSelf];
  if (self.checkbox.imagePosition == NSNoImage) {
    [self expandOrCollapseTree];
  }

  // Call makeFirstResponder via dispatch_async for safe.
  dispatch_async(dispatch_get_main_queue(), ^{
    [[self.checkboxOutlineViewDelegate.outlineView window] makeFirstResponder:self.checkboxOutlineViewDelegate.outlineView];
  });
}

- (void)selectSelf {
  NSInteger row = [self.checkboxOutlineViewDelegate.outlineView rowForView:self];
  if (row == -1) return;

  [self.checkboxOutlineViewDelegate.outlineView selectRowIndexes:[NSIndexSet indexSetWithIndex:(NSUInteger)(row)] byExtendingSelection:NO];
}

- (void)expandOrCollapseTree {
  NSInteger row = [self.checkboxOutlineViewDelegate.outlineView rowForView:self];
  if (row == -1) return;

  NSDictionary* item = [self.checkboxOutlineViewDelegate.outlineView itemAtRow:row];
  if (![self.checkboxOutlineViewDelegate.outlineView isExpandable:item]) {
    return;
  }

  if ([self.checkboxOutlineViewDelegate.outlineView isItemExpanded:item]) {
    [self.checkboxOutlineViewDelegate.outlineView collapseItem:item];
  } else {
    [self.checkboxOutlineViewDelegate.outlineView expandItem:item];
  }
  [self.checkboxOutlineViewDelegate.outlineView setNeedsDisplay:YES];
}

@end
