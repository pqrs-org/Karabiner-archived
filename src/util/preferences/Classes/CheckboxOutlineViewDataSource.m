#import "CheckboxOutlineViewDataSource.h"
#import "CheckboxTree.h"
#import "PreferencesWindowController.h"
#import "ServerClient.h"

@interface FilterCondition : NSObject

@property BOOL isEnabledOnly;
@property(copy) NSString* string;

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

@property(weak) IBOutlet PreferencesWindowController* preferencesWindowController;
@property(weak) IBOutlet ServerClient* client;
@property(readwrite) CheckboxTree* dataSource;
@property(readwrite) CheckboxTree* fullDataSource;
@property FilterCondition* filterCondition;

@end

@implementation CheckboxOutlineViewDataSource

- (void)setup {
  self.dataSource = [self.client.proxy checkboxTree];
  self.fullDataSource = self.dataSource;
  self.filterCondition = nil;
}

// return YES if we need to call [NSOutlineView reloadData]
- (BOOL)filterDataSource:(BOOL)isEnabledOnly string:(NSString*)string {
#if 0
  // Check filter condition is changed from previous filterDataSource.
  FilterCondition* filterCondition = [[FilterCondition alloc] init:isEnabledOnly string:string];
  if ([self.filterCondition isEqualToFilterCondition:filterCondition]) {
    return NO;
  }

  // ----------------------------------------
  NSMutableArray* strings = [NSMutableArray new];
  if (string) {
    for (NSString* s in [string componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceCharacterSet]]) {
      if ([s length] == 0) continue;
      [strings addObject:s];
    }
  }

  if (isEnabledOnly || [strings count] > 0) {
    self.dataSource = [self.client.proxy narrowedSharedCheckboxTree:isEnabledOnly strings:strings];
  } else {
    self.dataSource = [self.client.proxy sharedCheckboxTree];
  }

  self.filterCondition = filterCondition;
  return YES;
#else
  return NO;
#endif
}

#if 0
- (SharedXMLCompilerTree*)narrowedSharedCheckboxTree:(XMLCompilerTree*)tree isEnabledOnly:(BOOL)isEnabledOnly strings:(NSArray*)strings {
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
      CheckboxItem* checkboxItem = [tree.node castToCheckboxItem];
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
  NSMutableArray* newchildren = [NSMutableArray new];
  for (XMLCompilerTree* child in tree.children) {
    SharedXMLCompilerTree* t = [self narrowedSharedCheckboxTree:child isEnabledOnly:isEnabledOnly strings:strings];
    if (t) {
      [newchildren addObject:t];
    }
  }

  if ([newchildren count] > 0) {
    return [[SharedXMLCompilerTree alloc] initWithId:tree.node.id children:newchildren];
  }

  // ------------------------------------------------------------
  // filter by isEnabledOnly
  if (isEnabledOnly) {
    CheckboxItem* checkboxItem = [tree.node castToCheckboxItem];
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

  return [[SharedXMLCompilerTree alloc] initWithId:tree.node.id children:nil];
}

- (SharedXMLCompilerTree*)narrowedSharedCheckboxTree:(BOOL)isEnabledOnly strings:(NSArray*)strings {
  __block SharedXMLCompilerTree* result = nil;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    result = [self narrowedSharedCheckboxTree:self.preferencepane_checkbox isEnabledOnly:isEnabledOnly strings:strings];
  });
  if (!result) {
    result = [[SharedXMLCompilerTree alloc] initWithId:nil children:nil];
  }
  return result;
}
#endif

- (NSInteger)outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item {
  CheckboxTree* tree = (CheckboxTree*)(item);
  return tree ? [tree.children count] : [self.dataSource.children count];
}

- (void)clearFilterCondition {
  self.filterCondition = nil;
}

- (id)outlineView:(NSOutlineView*)outlineView child:(NSInteger)index ofItem:(id)item {
  CheckboxTree* tree = (CheckboxTree*)(item);
  NSArray* a = tree ? tree.children : self.dataSource.children;

  if ((NSUInteger)(index) >= [a count]) return nil;
  return a[index];
}

- (BOOL)outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item {
  CheckboxTree* tree = (CheckboxTree*)(item);
  NSArray* a = tree ? tree.children : self.dataSource.children;
  return [a count] > 0;
}

@end
