#import "CheckboxBackgroundView.h"
#import "CheckboxCellView.h"
#import "CheckboxOutlineViewDataSource.h"
#import "CheckboxOutlineViewDelegate.h"
#import "PreferencesKeys.h"
#import "PreferencesManager.h"
#import "XMLCompiler.h"

#define kLabelLeadingSpaceWithCheckbox 24
#define kLabelLeadingSpaceWithoutCheckbox 4
#define kLabelTopSpace 2
#define kLabelBottomSpace 2

@interface CheckboxOutlineViewDelegate () {
  dispatch_queue_t textsHeightQueue_;
}
@property(weak) IBOutlet NSTextField* wrappedTextHeightCalculator;
@property NSFont* font;
@property NSMutableDictionary* heightCache;
@end

@implementation CheckboxOutlineViewDelegate

- (instancetype)init {
  self = [super init];

  if (self) {
    textsHeightQueue_ = dispatch_queue_create("org.pqrs.Karabiner.CheckboxOutlineViewDelegate.textsHeightQueue_", NULL);
    self.heightCache = [NSMutableDictionary new];
    [self updateFont];
  }

  return self;
}

- (void)updateFont {
  switch ([[NSUserDefaults standardUserDefaults] integerForKey:kKarabinerPreferencesCheckboxFont]) {
  case 0:
    self.font = [NSFont systemFontOfSize:[NSFont smallSystemFontSize]];
    break;
  default:
    self.font = [NSFont systemFontOfSize:[NSFont systemFontSize]];
    break;
  }
}

- (void)clearHeightCache {
  [self.heightCache removeAllObjects];
}

- (NSView*)outlineView:(NSOutlineView*)outlineView viewForTableColumn:(NSTableColumn*)tableColumn item:(id)item {
  NSString* identifier = item[@"identifier"];
  CheckboxItem* checkboxItem = (CheckboxItem*)(item[@"checkboxItem"]);
  NSString* name = [checkboxItem getName];
  NSString* style = [checkboxItem getStyle];

  CheckboxCellView* result = [outlineView makeViewWithIdentifier:@"CheckboxCellView" owner:self];
  result.settingIdentifier = identifier;

  result.textField.stringValue = name;
  result.textField.font = self.font;

  result.labelTopSpace.constant = kLabelTopSpace;
  result.labelBottomSpace.constant = kLabelBottomSpace;

  if (![CheckboxOutlineViewDataSource isCheckbox:identifier]) {
    result.labelLeadingSpace.constant = kLabelLeadingSpaceWithoutCheckbox;
  } else {
    result.labelLeadingSpace.constant = kLabelLeadingSpaceWithCheckbox;

    // ----------------------------------------
    // Add checkbox
    result.checkbox = [NSButton new];
    [result.checkbox setButtonType:NSSwitchButton];
    result.checkbox.imagePosition = NSImageOnly;
    result.checkbox.target = result;
    result.checkbox.action = @selector(valueChanged:);
    if ([self.preferencesManager value:identifier]) {
      result.checkbox.state = NSOnState;
    } else {
      result.checkbox.state = NSOffState;
    }
    result.checkbox.translatesAutoresizingMaskIntoConstraints = NO;
    [result addSubview:result.checkbox positioned:NSWindowBelow relativeTo:nil];

    [result addConstraint:[NSLayoutConstraint constraintWithItem:result.checkbox
                                                       attribute:NSLayoutAttributeLeading
                                                       relatedBy:NSLayoutRelationEqual
                                                          toItem:result
                                                       attribute:NSLayoutAttributeLeading
                                                      multiplier:1.0
                                                        constant:4]];
    [result addConstraint:[NSLayoutConstraint constraintWithItem:result.checkbox
                                                       attribute:NSLayoutAttributeCenterY
                                                       relatedBy:NSLayoutRelationEqual
                                                          toItem:result
                                                       attribute:NSLayoutAttributeCenterY
                                                      multiplier:1.0
                                                        constant:0]];
  }

  // ----------------------------------------
  // Set backgroundView

  NSColor* backgroundColor = nil;
  if ([style isEqualToString:@"caution"]) {
    backgroundColor = [NSColor greenColor];
  } else if ([style isEqualToString:@"important"]) {
    backgroundColor = [NSColor orangeColor];
  } else if ([style isEqualToString:@"slight"]) {
    backgroundColor = [NSColor lightGrayColor];
  }

  if (backgroundColor) {
    result.backgroundView = [CheckboxBackgroundView new];
    result.backgroundView.color = backgroundColor;
    result.backgroundView.translatesAutoresizingMaskIntoConstraints = NO;
    [result addSubview:result.backgroundView positioned:NSWindowBelow relativeTo:nil];
    [result addLayoutConstraint:result.backgroundView top:0 bottom:0 leading:0 trailing:0];
  }

  return result;
}

// heightOfRowByItem will be called before viewForTableColumn.
// So, we need to calculate the height by using wrappedTextHeightCalculator.

- (CGFloat)outlineView:(NSOutlineView*)outlineView heightOfRowByItem:(id)item {
  CheckboxItem* checkboxItem = item[@"checkboxItem"];
  if (!checkboxItem) {
    return [outlineView rowHeight];
  }

  if (!self.heightCache[checkboxItem.id]) {
    NSString* identifier = item[@"identifier"];

    NSTableColumn* column = [outlineView outlineTableColumn];

    CGFloat indentation = outlineView.indentationPerLevel * ([outlineView levelForItem:item] + 1);
    NSInteger preferredMaxLayoutWidth = (NSInteger)(column.width) - indentation;

    if ([CheckboxOutlineViewDataSource isCheckbox:identifier]) {
      preferredMaxLayoutWidth -= kLabelLeadingSpaceWithCheckbox;
    } else {
      preferredMaxLayoutWidth -= kLabelLeadingSpaceWithoutCheckbox;
    }

    dispatch_sync(textsHeightQueue_, ^{
      self.wrappedTextHeightCalculator.stringValue = [checkboxItem getName];
      self.wrappedTextHeightCalculator.font = self.font;
      self.wrappedTextHeightCalculator.preferredMaxLayoutWidth = preferredMaxLayoutWidth;

      NSSize size = [self.wrappedTextHeightCalculator fittingSize];
      self.heightCache[checkboxItem.id] = @(size.height + kLabelTopSpace + kLabelBottomSpace);
    });
  }

  return [self.heightCache[checkboxItem.id] integerValue];
}

@end
