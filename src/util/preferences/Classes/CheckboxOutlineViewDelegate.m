#import "CheckboxOutlineViewDelegate.h"
#import "CheckboxBackgroundView.h"
#import "CheckboxCellView.h"
#import "CheckboxTree.h"
#import "PreferencesClient.h"
#import "PreferencesModel.h"
#import "PreferencesWindowController.h"
#import "ServerClient.h"

#define kLabelLeadingSpaceWithCheckbox 24
#define kLabelLeadingSpaceWithoutCheckbox 4
#define kLabelTopSpace 4
#define kLabelBottomSpace 4

@interface CheckboxOutlineViewDelegate ()

@property(weak) IBOutlet NSTextField* wrappedTextHeightCalculator;
@property(weak) IBOutlet PreferencesClient* preferencesClient;
@property(weak) IBOutlet PreferencesWindowController* preferencesWindowController;
@property NSFont* font;
@property NSMutableDictionary* heightCache;
@property dispatch_queue_t textsHeightQueue;

@end

@implementation CheckboxOutlineViewDelegate

- (instancetype)init {
  self = [super init];

  if (self) {
    self.textsHeightQueue = dispatch_queue_create("org.pqrs.Karabiner.CheckboxOutlineViewDelegate.textsHeightQueue", NULL);
    self.heightCache = [NSMutableDictionary new];
  }

  return self;
}

- (void)updateFont {
  switch (self.preferencesClient.pm.preferencesCheckboxFont) {
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
  CheckboxTree* tree = (CheckboxTree*)(item);
  if (!tree || !tree.node) return nil;

  CheckboxCellView* result = [outlineView makeViewWithIdentifier:@"CheckboxCellView" owner:self];
  result.preferencesClient = self.preferencesClient;
  result.preferencesWindowController = self.preferencesWindowController;
  result.settingIdentifier = tree.node.identifier;

  result.textField.stringValue = tree.node.name;
  result.textField.font = self.font;

  result.labelTopSpace.constant = kLabelTopSpace;
  result.labelBottomSpace.constant = kLabelBottomSpace;

  if (!tree.node.needsShowCheckbox) {
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
    if ([self.preferencesClient.pm value:tree.node.identifier]) {
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
  if ([tree.node.style isEqualToString:@"caution"]) {
    backgroundColor = [NSColor greenColor];
  } else if ([tree.node.style isEqualToString:@"important"]) {
    backgroundColor = [NSColor orangeColor];
  } else if ([tree.node.style isEqualToString:@"slight"]) {
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
  CheckboxTree* tree = (CheckboxTree*)(item);
  if (!tree || !tree.node) {
    return [outlineView rowHeight];
  }

  if (!self.heightCache[tree.node.id]) {
    NSTableColumn* column = [outlineView outlineTableColumn];

    CGFloat indentation = outlineView.indentationPerLevel * ([outlineView levelForItem:item] + 1);
    NSInteger preferredMaxLayoutWidth = (NSInteger)(column.width) - indentation;

    if (!tree.node.needsShowCheckbox) {
      preferredMaxLayoutWidth -= kLabelLeadingSpaceWithoutCheckbox;
    } else {
      preferredMaxLayoutWidth -= kLabelLeadingSpaceWithCheckbox;
    }

    dispatch_sync(self.textsHeightQueue, ^{
      self.wrappedTextHeightCalculator.stringValue = tree.node.name;
      self.wrappedTextHeightCalculator.font = self.font;
      self.wrappedTextHeightCalculator.preferredMaxLayoutWidth = preferredMaxLayoutWidth;

      NSSize size = [self.wrappedTextHeightCalculator fittingSize];
      self.heightCache[tree.node.id] = @(size.height + kLabelTopSpace + kLabelBottomSpace);
    });
  }

  return [self.heightCache[tree.node.id] integerValue];
}

@end
