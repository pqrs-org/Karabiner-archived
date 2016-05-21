#import "CheckboxOutlineViewDataSource.h"
#import "CheckboxTree.h"
#import "PreferencesClient.h"
#import "PreferencesModel.h"
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

@property(weak) IBOutlet PreferencesClient* preferencesClient;
@property(weak) IBOutlet ServerClient* client;
@property(readwrite) CheckboxTree* dataSource;
@property(readwrite) CheckboxTree* fullDataSource;
@property FilterCondition* filterCondition;

@end

@implementation CheckboxOutlineViewDataSource

- (void)setup {
  self.dataSource = [self.client checkboxTree];
  self.fullDataSource = self.dataSource;
  self.filterCondition = nil;
}

// return YES if we need to call [NSOutlineView reloadData]
- (BOOL)filterDataSource:(BOOL)isEnabledOnly string:(NSString*)string {
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
    self.dataSource = [self narrowedSharedCheckboxTree:isEnabledOnly strings:strings];
  } else {
    self.dataSource = self.fullDataSource;
  }

  self.filterCondition = filterCondition;
  return YES;
}

- (CheckboxTree*)narrowedSharedCheckboxTree:(BOOL)isEnabledOnly strings:(NSArray*)strings {
  CheckboxTree* result = [self narrowedSharedCheckboxTree:self.fullDataSource isEnabledOnly:isEnabledOnly strings:strings];
  if (!result) {
    result = [[CheckboxTree alloc] initWithItem:nil children:nil];
  }
  return result;
}

- (CheckboxTree*)narrowedSharedCheckboxTree:(CheckboxTree*)tree isEnabledOnly:(BOOL)isEnabledOnly strings:(NSArray*)strings {
  if (!tree) return nil;

  // check strings
  BOOL stringsMatched = YES;
  if (!tree.node) {
    stringsMatched = NO;
  } else {
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
        if ([tree.node.name rangeOfString:s options:NSCaseInsensitiveSearch].location == NSNotFound) {
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
  }

  // ------------------------------------------------------------
  // check children
  NSMutableArray* newchildren = [NSMutableArray new];
  for (CheckboxTree* child in tree.children) {
    CheckboxTree* t = [self narrowedSharedCheckboxTree:child isEnabledOnly:isEnabledOnly strings:strings];
    if (t) {
      [newchildren addObject:t];
    }
  }

  if ([newchildren count] > 0) {
    return [[CheckboxTree alloc] initWithItem:tree.node children:newchildren];
  }

  // ------------------------------------------------------------
  // filter by isEnabledOnly
  if (isEnabledOnly) {
    if (tree.node) {
      if ([tree.node.identifier length] == 0) {
        return nil;
      }
      if (![self.preferencesClient.pm value:tree.node.identifier]) {
        return nil;
      }
    }
  }

  // check strings
  if (!stringsMatched) {
    return nil;
  }

  return [[CheckboxTree alloc] initWithItem:tree.node children:nil];
}

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
