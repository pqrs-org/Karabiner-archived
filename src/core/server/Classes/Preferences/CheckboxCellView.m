#import "CheckboxCellView.h"
#import "PreferencesManager.h"

@implementation CheckboxCellView

- (IBAction)valueChanged:(id)sender {
  NSInteger row = [self.outlineView rowForView:self];
  [self.outlineView selectRowIndexes:[NSIndexSet indexSetWithIndex:(NSUInteger)(row)] byExtendingSelection:NO];

  if (self.checkbox.imagePosition != NSNoImage) {
    int value = (self.checkbox.state == NSOnState);
    [self.preferencesManager setValue:value forName:self.settingIdentifier];

  } else {
    // expand/collapse tree
    NSDictionary* item = [self.outlineView itemAtRow:row];
    if ([self.outlineView isExpandable:item]) {
      if ([self.outlineView isItemExpanded:item]) {
        [self.outlineView collapseItem:item];
      } else {
        [self.outlineView expandItem:item];
      }
    }
  }
}

@end
