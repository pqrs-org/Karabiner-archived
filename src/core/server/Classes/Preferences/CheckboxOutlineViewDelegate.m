#import "CheckboxBackgroundView.h"
#import "CheckboxCellView.h"
#import "CheckboxOutlineViewDataSource.h"
#import "CheckboxOutlineViewDelegate.h"
#import "PreferencesManager.h"

#define kLabelLeadingSpaceWithCheckbox 24
#define kLabelLeadingSpaceWithoutCheckbox 4
#define kLabelTopSpace 2
#define kLabelBottomSpace 2

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
  NSString* identifier = item[@"identifier"];
  NSString* name = item[@"name"];
  NSString* style = item[@"style"];
  NSInteger preferredMaxLayoutWidth = [item[@"preferredMaxLayoutWidth"] integerValue];

  CheckboxCellView* result = [outlineView makeViewWithIdentifier:@"CheckboxCellView" owner:self];
  result.settingIdentifier = identifier;

  result.textField.stringValue = name;
  result.textField.font = self.font;
  if (preferredMaxLayoutWidth) {
    result.textField.preferredMaxLayoutWidth = preferredMaxLayoutWidth;
  }

  result.labelTopSpace.constant = kLabelTopSpace;
  result.labelBottomSpace.constant = kLabelBottomSpace;

  if (![CheckboxOutlineViewDataSource isCheckbox:identifier]) {
    result.checkbox.imagePosition = NSNoImage;
    result.labelLeadingSpace.constant = kLabelLeadingSpaceWithoutCheckbox;
  } else {
    result.checkbox.imagePosition = NSImageLeft;
    result.labelLeadingSpace.constant = kLabelLeadingSpaceWithCheckbox;

    if ([self.preferencesManager value:identifier]) {
      result.checkbox.state = NSOnState;
    } else {
      result.checkbox.state = NSOffState;
    }
  }

  if ([style isEqualToString:@"caution"]) {
    result.background.hidden = NO;
    result.background.color = [NSColor greenColor];
    result.background.needsDisplay = YES;
    result.textField.textColor = [NSColor blackColor];
  } else if ([style isEqualToString:@"important"]) {
    result.background.hidden = NO;
    result.background.color = [NSColor orangeColor];
    result.background.needsDisplay = YES;
    result.textField.textColor = [NSColor blackColor];
  } else if ([style isEqualToString:@"slight"]) {
    result.background.hidden = NO;
    result.background.color = [NSColor lightGrayColor];
    result.background.needsDisplay = YES;
    result.textField.textColor = [NSColor blackColor];
  } else {
    result.background.hidden = YES;
    result.textField.textColor = [NSColor textColor];
  }

  return result;
}

// heightOfRowByItem will be called before viewForTableColumn.
// So, we need to calculate the height by using wrappedTextHeightCalculator.

- (CGFloat)outlineView:(NSOutlineView*)outlineView heightOfRowByItem:(id)item {
  NSString* identifier = item[@"identifier"];

  NSTableColumn* column = [outlineView outlineTableColumn];

  CGFloat indentation = outlineView.indentationPerLevel * ([outlineView levelForItem:item] + 1);
  NSInteger preferredMaxLayoutWidth = (NSInteger)(column.width) - indentation;

  if ([CheckboxOutlineViewDataSource isCheckbox:identifier]) {
    preferredMaxLayoutWidth -= kLabelLeadingSpaceWithCheckbox;
  } else {
    preferredMaxLayoutWidth -= kLabelLeadingSpaceWithoutCheckbox;
  }

  if ([item[@"preferredMaxLayoutWidth"] integerValue] != preferredMaxLayoutWidth) {
    dispatch_sync(textsHeightQueue_, ^{
      item[@"preferredMaxLayoutWidth"] = @(preferredMaxLayoutWidth);

      self.wrappedTextHeightCalculator.stringValue = item[@"name"];
      self.wrappedTextHeightCalculator.font = self.font;
      self.wrappedTextHeightCalculator.preferredMaxLayoutWidth = preferredMaxLayoutWidth;

      NSSize size = [self.wrappedTextHeightCalculator fittingSize];
      item[@"height"] = @(size.height + kLabelTopSpace + kLabelBottomSpace);
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
