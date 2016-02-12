#import "CheckboxOutlineViewDataSource.h"
#import "PreferencesManager.h"
#import "XMLCompiler.h"

@interface FilterCondition : NSObject
@property BOOL isEnabledOnly;
@property NSString* string;

- (BOOL)isEqualToFilterCondition:(FilterCondition*)other;
@end

@implementation FilterCondition

- (instancetype)init:(BOOL)isEnabledOnly string:(NSString*)string {
  self = [super init];

  if (self) {
    self.isEnabledOnly = isEnabledOnly;
    self.string = string;
  }

  return self;
}

- (BOOL)isEqualToFilterCondition:(FilterCondition*)other {
  if (self.isEnabledOnly != other.isEnabledOnly) {
    return NO;
  }

  if (![self compareString:other.string]) {
    return NO;
  }

  return YES;
}

- (BOOL)compareString:(NSString*)otherString {
  if (self.string == nil && otherString == nil) {
    return YES;
  }
  if (self.string != nil && otherString != nil) {
    return [self.string compare:otherString] == NSOrderedSame;
  }
  return NO;
}

@end

@interface CheckboxOutlineViewDataSource ()
@property(weak) IBOutlet PreferencesManager* preferencesManager;
@property(weak) IBOutlet XMLCompiler* xmlCompiler;
@property XMLCompilerTree* dataSource;
@property FilterCondition* filterCondition;
@end

@implementation CheckboxOutlineViewDataSource

- (void)load:(BOOL)force {
  if (force) {
    if (self.dataSource) {
      self.dataSource = nil;
      self.filterCondition = nil;
    }
  }

  if (!self.dataSource) {
    self.dataSource = self.xmlCompiler.preferencepane_checkbox;
    self.filterCondition = nil;
  }
}

- (XMLCompilerTree*)filterDataSource_core:(XMLCompilerTree*)tree isEnabledOnly:(BOOL)isEnabledOnly strings:(NSArray*)strings {
  // check strings
  BOOL stringsMatched = YES;
  if (strings) {
    // Remove matched strings from strings for children.
    //
    // For example:
    //   strings == @[@"Emacs", @"Mode", @"Tab"]
    //
    //   * Emacs Mode
    //     * Control+I to Tab
    //
    //   notMatchedStrings == @[@"Tab"] at "Emacs Mode".
    //   Then "Control+I to Tab" will be matched by strings == @[@"Tab"].

    NSMutableArray* notMatchedStrings = nil;
    for (NSString* s in strings) {
      CheckboxItem* checkboxItem = [tree castNodeToCheckboxItem];
      if (![checkboxItem isNameMatched:s]) {
        stringsMatched = NO;
      } else {
        if (!notMatchedStrings) {
          notMatchedStrings = [NSMutableArray arrayWithArray:strings];
        }
        [notMatchedStrings removeObject:s];
      }
    }

    if (notMatchedStrings) {
      strings = notMatchedStrings;
    }
  }

  // ------------------------------------------------------------
  // check children
  XMLCompilerTree* newtree = [XMLCompilerTree new];
  newtree.node = tree.node;

  if (tree.children) {
    NSMutableArray* newchildren = [NSMutableArray new];
    for (XMLCompilerTree* child in tree.children) {
      XMLCompilerTree* t = [self filterDataSource_core:child isEnabledOnly:isEnabledOnly strings:strings];
      if (t) {
        [newchildren addObject:t];
      }
    }

    if ([newchildren count] > 0) {
      newtree.children = newchildren;
      return newtree;
    }
  }

  // ------------------------------------------------------------
  // filter by isEnabledOnly
  if (isEnabledOnly) {
    CheckboxItem* checkboxItem = [tree castNodeToCheckboxItem];
    NSString* identifier = [checkboxItem getIdentifier];
    if ([identifier length] == 0) {
      return nil;
    }
    if (![self.preferencesManager value:identifier]) {
      return nil;
    }
  }

  // check strings
  if (!stringsMatched) {
    return nil;
  }

  return newtree;
}

// return YES if we need to call [NSOutlineView reloadData]
- (BOOL)filterDataSource:(BOOL)isEnabledOnly string:(NSString*)string {
  // Check filter condition is changed from previous filterDataSource.
  FilterCondition* filterCondition = [[FilterCondition alloc] init:isEnabledOnly string:string];
  if ([self.filterCondition isEqualToFilterCondition:filterCondition]) {
    return NO;
  }

  // ----------------------------------------
  [self load:YES];

  if (!self.dataSource) return NO;

  NSMutableArray* strings = [NSMutableArray new];
  if (string) {
    for (NSString* s in [string componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceCharacterSet]]) {
      if ([s length] == 0) continue;
      [strings addObject:s];
    }
  }

  if (isEnabledOnly || [strings count] > 0) {
    XMLCompilerTree* newdatasource = [self filterDataSource_core:self.dataSource isEnabledOnly:isEnabledOnly strings:strings];
    if (!newdatasource) {
      newdatasource = [XMLCompilerTree new];
    }
    self.dataSource = newdatasource;
  }

  self.filterCondition = filterCondition;
  return YES;
}

- (NSInteger)outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item {
  [self load:NO];

  XMLCompilerTree* tree = (XMLCompilerTree*)(item);
  return tree ? [tree.children count] : [self.dataSource.children count];
}

- (void)clearFilterCondition {
  self.filterCondition = nil;
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

@end
