#import "OutlineViewDataSourceNumber.h"
#import "PreferencesManager.h"
#import "XMLCompiler.h"

@interface OutlineViewDataSourceNumber ()
@property NSMutableArray* dataSource;
@property(weak) IBOutlet PreferencesManager* preferencesManager;
@property(weak) IBOutlet XMLCompiler* xmlCompiler;
@end

@implementation OutlineViewDataSourceNumber

- (void)load:(BOOL)force {
  if (force) {
    if (self.dataSource) {
      self.dataSource = nil;
    }
  }

  if (!self.dataSource) {
    self.dataSource = [self.xmlCompiler preferencepane_number];
  }
}

- (NSInteger)outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item {
  [self load:NO];

  return item ? [item[@"children"] count] : [self.dataSource count];
}

- (id)outlineView:(NSOutlineView*)outlineView child:(NSInteger)index ofItem:(id)item {
  [self load:NO];

  // ----------------------------------------
  NSMutableArray* a = nil;

  // root object
  if (!item) {
    a = self.dataSource;
  } else {
    a = item[@"children"];
  }

  if ((NSUInteger)(index) >= [a count]) return nil;
  return a[index];
}

- (BOOL)outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item {
  return [item[@"children"] count] > 0;
}

- (id)outlineView:(NSOutlineView*)outlineView objectValueForTableColumn:(NSTableColumn*)tableColumn byItem:(id)item {
  NSString* identifier = item[@"identifier"];

  NSString* columnIdentifier = [tableColumn identifier];

  if ([columnIdentifier isEqualToString:@"name"]) {
    return item[columnIdentifier];

  } else if ([columnIdentifier isEqualToString:@"baseunit"] ||
             [columnIdentifier isEqualToString:@"default"]) {
    if (!identifier) return nil;
    return item[columnIdentifier];

  } else if ([columnIdentifier isEqualToString:@"value"]) {
    if (!identifier) return nil;

    // @(12345) will be @"12,345" in view.
    // So we return NSString to show numbers without comma.
    return [[NSString alloc] initWithFormat:@"%d", [self.preferencesManager value:identifier]];

  } else if ([columnIdentifier isEqualToString:@"stepper"]) {
    if (!identifier) return nil;
    return @([self.preferencesManager value:identifier]);
  }

  return nil;
}

- (void)outlineView:(NSOutlineView*)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn byItem:(id)item {
  NSString* identifier = item[@"identifier"];

  if (identifier) {
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
