#import "CheckboxCellView.h"
#import "CheckboxOutlineViewDelegate.h"
#import "PreferencesManager.h"

@implementation CheckboxCellView

- (IBAction)valueChanged:(id)sender {
  // select row
  NSInteger row = [self.checkboxOutlineViewDelegate.outlineView rowForView:self];
  [self.checkboxOutlineViewDelegate.outlineView selectRowIndexes:[NSIndexSet indexSetWithIndex:(NSUInteger)(row)] byExtendingSelection:NO];
  [[self.checkboxOutlineViewDelegate.outlineView window] makeFirstResponder:self.checkboxOutlineViewDelegate.outlineView];

  // toggle setting
  if (self.checkbox.imagePosition != NSNoImage) {
    int value = (self.checkbox.state == NSOnState);
    [self.checkboxOutlineViewDelegate.preferencesManager setValue:value forName:self.settingIdentifier];

  } else {
    // expand/collapse tree
    NSDictionary* item = [self.checkboxOutlineViewDelegate.outlineView itemAtRow:row];
    if ([self.checkboxOutlineViewDelegate.outlineView isExpandable:item]) {
      if ([self.checkboxOutlineViewDelegate.outlineView isItemExpanded:item]) {
        [self.checkboxOutlineViewDelegate.outlineView collapseItem:item];
      } else {
        [self.checkboxOutlineViewDelegate.outlineView expandItem:item];
      }
    }
  }
}

@end
