#import "CheckboxOutlineViewDelegate.h"
#import "CheckboxBackgroundView.h"
#import "CheckboxCellView.h"
#import "PreferencesKeys.h"
#import "PreferencesManager.h"
#import "PreferencesModel.h"
#import "PreferencesWindowController.h"
#import "ServerForUserspace.h"
#import "ServerObjects.h"
#import "SharedXMLCompilerTree.h"
#import "XMLCompiler.h"

#define kLabelLeadingSpaceWithCheckbox 24
#define kLabelLeadingSpaceWithoutCheckbox 4
#define kLabelTopSpace 4
#define kLabelBottomSpace 4

@interface CheckboxOutlineViewDelegate ()

@property(weak) IBOutlet PreferencesWindowController* preferencesWindowController;
@property(weak) IBOutlet PreferencesModel* preferencesModel;
@property(weak) IBOutlet NSTextField* wrappedTextHeightCalculator;
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
  switch (self.preferencesModel.preferencesCheckboxFont) {
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
  SharedXMLCompilerTree* tree = (SharedXMLCompilerTree*)(item);
  NSString* name = [self.preferencesWindowController.serverObjects.serverForUserspace checkboxItemGetName:tree.id];
  NSString* style = [self.preferencesWindowController.serverObjects.serverForUserspace checkboxItemGetStyle:tree.id];
  NSString* identifier = [self.preferencesWindowController.serverObjects.serverForUserspace checkboxItemGetIdentifier:tree.id];

  CheckboxCellView* result = [outlineView makeViewWithIdentifier:@"CheckboxCellView" owner:self];
  result.serverObjects = self.preferencesWindowController.serverObjects;
  result.settingIdentifier = identifier;

  result.textField.stringValue = name;
  result.textField.font = self.font;

  result.labelTopSpace.constant = kLabelTopSpace;
  result.labelBottomSpace.constant = kLabelBottomSpace;

  if (![self.preferencesWindowController.serverObjects.serverForUserspace checkboxItemNeedsShowCheckbox:tree.id]) {
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
    if ([self.preferencesWindowController.serverObjects.preferencesManager value:identifier]) {
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
  SharedXMLCompilerTree* tree = (SharedXMLCompilerTree*)(item);
  if (!tree.id) {
    return [outlineView rowHeight];
  }

  if (!self.heightCache[tree.id]) {
    NSTableColumn* column = [outlineView outlineTableColumn];

    CGFloat indentation = outlineView.indentationPerLevel * ([outlineView levelForItem:item] + 1);
    NSInteger preferredMaxLayoutWidth = (NSInteger)(column.width) - indentation;

    if (![self.preferencesWindowController.serverObjects.serverForUserspace checkboxItemNeedsShowCheckbox:tree.id]) {
      preferredMaxLayoutWidth -= kLabelLeadingSpaceWithCheckbox;
    } else {
      preferredMaxLayoutWidth -= kLabelLeadingSpaceWithoutCheckbox;
    }

    dispatch_sync(self.textsHeightQueue, ^{
      self.wrappedTextHeightCalculator.stringValue = [self.preferencesWindowController.serverObjects.serverForUserspace checkboxItemGetName:tree.id];
      self.wrappedTextHeightCalculator.font = self.font;
      self.wrappedTextHeightCalculator.preferredMaxLayoutWidth = preferredMaxLayoutWidth;

      NSSize size = [self.wrappedTextHeightCalculator fittingSize];
      self.heightCache[tree.id] = @(size.height + kLabelTopSpace + kLabelBottomSpace);
    });
  }

  return [self.heightCache[tree.id] integerValue];
}

@end
