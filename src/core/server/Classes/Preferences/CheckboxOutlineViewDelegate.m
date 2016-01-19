#import "CheckboxCellView.h"
#import "CheckboxOutlineViewDelegate.h"

@interface CheckboxOutlineViewDelegate () {
  dispatch_queue_t textsHeightQueue_;
}
@property(weak) IBOutlet NSTextField* wrappedTextHeightCalculator;
@property NSFont* font;
@end

@implementation CheckboxOutlineViewDelegate

- (instancetype)init {
  self = [super init];

  if (self) {
    textsHeightQueue_ = dispatch_queue_create("org.pqrs.Karabiner.CheckboxOutlineViewDelegate.textsHeightQueue_", NULL);
    self.font = [NSFont systemFontOfSize:[NSFont smallSystemFontSize]];
  }

  return self;
}

- (NSView*)outlineView:(NSOutlineView*)outlineView viewForTableColumn:(NSTableColumn*)tableColumn item:(id)item {
  CheckboxCellView* result = [outlineView makeViewWithIdentifier:@"OutlineViewDelegateTextCellView" owner:self];
  result.textField.stringValue = item[@"name"];
  result.textField.font = self.font;
  result.settingIdentifier = item[@"identifier"];
  return result;
}

// heightOfRowByItem will be called before viewForTableColumn.
// So, we need to calculate the height by using wrappedTextHeightCalculator.

- (CGFloat)outlineView:(NSOutlineView*)outlineView heightOfRowByItem:(id)item {
  NSTableColumn* column = [outlineView outlineTableColumn];

  CGFloat indentation = outlineView.indentationPerLevel * ([outlineView levelForItem:item] + 1);
  NSInteger preferredMaxLayoutWidth = (NSInteger)(column.width) - 24 - indentation;

  if ([item[@"preferredMaxLayoutWidth"] integerValue] != preferredMaxLayoutWidth) {
    dispatch_sync(textsHeightQueue_, ^{
      item[@"preferredMaxLayoutWidth"] = @(preferredMaxLayoutWidth);

      self.wrappedTextHeightCalculator.stringValue = item[@"name"];
      self.wrappedTextHeightCalculator.font = self.font;
      self.wrappedTextHeightCalculator.preferredMaxLayoutWidth = preferredMaxLayoutWidth;

      NSSize size = [self.wrappedTextHeightCalculator fittingSize];
      NSInteger margin = 2;
      item[@"height"] = @(size.height + margin * 2);
    });
  }

  NSInteger height = [item[@"height"] integerValue];
  if (height == 0) {
    return [outlineView rowHeight];
  } else {
    return height;
  }
}

@end
