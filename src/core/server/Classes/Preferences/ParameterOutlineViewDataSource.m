#import "ParameterOutlineViewDataSource.h"
#import "PreferencesManager.h"
#import "XMLCompiler.h"

@interface ParameterOutlineViewDataSource ()
@property(weak) IBOutlet PreferencesManager* preferencesManager;
@property(weak) IBOutlet XMLCompiler* xmlCompiler;
@property XMLCompilerTree* dataSource;
@end

@implementation ParameterOutlineViewDataSource

- (void)load:(BOOL)force {
  if (force) {
    if (self.dataSource) {
      self.dataSource = nil;
    }
  }

  if (!self.dataSource) {
    self.dataSource = self.xmlCompiler.preferencepane_parameter;
  }
}

- (NSInteger)outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item {
  [self load:NO];

  XMLCompilerTree* tree = (XMLCompilerTree*)(item);
  return tree ? [tree.children count] : [self.dataSource.children count];
}

- (id)outlineView:(NSOutlineView*)outlineView child:(NSInteger)index ofItem:(id)item {
  [self load:NO];

  // ----------------------------------------
  XMLCompilerTree* tree = (XMLCompilerTree*)(item);
  NSArray* a = tree ? tree.children : self.dataSource.children;

  if ((NSUInteger)(index) >= [a count]) return nil;
  return a[index];
}

- (BOOL)outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item {
  XMLCompilerTree* tree = (XMLCompilerTree*)(item);
  NSArray* a = tree ? tree.children : self.dataSource.children;
  return [a count] > 0;
}

- (id)outlineView:(NSOutlineView*)outlineView objectValueForTableColumn:(NSTableColumn*)tableColumn byItem:(id)item {
  XMLCompilerTree* tree = (XMLCompilerTree*)(item);
  ParameterItem* parameterItem = [tree castNodeToParameterItem];
  NSString* identifier = [parameterItem getIdentifier];

  NSString* columnIdentifier = [tableColumn identifier];

  if ([columnIdentifier isEqualToString:@"name"]) {
    return [parameterItem getName];

  } else if ([columnIdentifier isEqualToString:@"baseunit"]) {
    if ([identifier length] == 0) return nil;
    return [parameterItem getBaseUnit];

  } else if ([columnIdentifier isEqualToString:@"default"]) {
    if ([identifier length] == 0) return nil;
    return @([parameterItem getDefaultValue]);

  } else if ([columnIdentifier isEqualToString:@"value"]) {
    if ([identifier length] == 0) return nil;

    // @(12345) will be @"12,345" in view.
    // So we return NSString to show numbers without comma.
    return [[NSString alloc] initWithFormat:@"%d", [self.preferencesManager value:identifier]];

  } else if ([columnIdentifier isEqualToString:@"stepper"]) {
    if ([identifier length] == 0) return nil;
    return @([self.preferencesManager value:identifier]);
  }

  return nil;
}

- (void)outlineView:(NSOutlineView*)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn byItem:(id)item {
  XMLCompilerTree* tree = (XMLCompilerTree*)(item);
  ParameterItem* parameterItem = [tree castNodeToParameterItem];
  NSString* identifier = [parameterItem getIdentifier];

  if ([identifier length] > 0) {
    NSString* columnIdentifier = [tableColumn identifier];
    if ([columnIdentifier isEqualToString:@"value"] ||
        [columnIdentifier isEqualToString:@"stepper"]) {
      NSInteger newvalue = 0;
      if ([object isKindOfClass:[NSString class]]) {
        NSNumberFormatter* f = [NSNumberFormatter new];
        f.numberStyle = NSNumberFormatterDecimalStyle;
        NSNumber* n = [f numberFromString:object];
        if (!n) {
          return;
        }
        newvalue = [n integerValue];
      } else {
        newvalue = [object integerValue];
      }

      if (newvalue < 0) {
        return;
      }
      if (newvalue > 1073741824) {
        return;
      }

      [self.preferencesManager setValue:(int)(newvalue) forName:identifier];
      [outlineView reloadItem:item];
    }
  } else {
    // expand/collapse tree
    if ([outlineView isExpandable:item]) {
      if ([outlineView isItemExpanded:item]) {
        [outlineView collapseItem:item];
      } else {
        [outlineView expandItem:item];
      }
    }
  }
}

@end
