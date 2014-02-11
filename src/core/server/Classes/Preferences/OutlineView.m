#import "NotificationKeys.h"
#import "OutlineView.h"
#import "PreferencesManager.h"
#import "XMLCompiler.h"

@implementation OutlineView

- (void) observer_PreferencesChanged:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    [outlineview_ setNeedsDisplay:YES];
  });
}

- (void) observer_ConfigListChanged:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    [outlineview_ setNeedsDisplay:YES];
    if ([showEnabledOnly_ state] == NSOnState) {
      [self filter:self];
    }
  });
}

- (void) observer_ConfigXMLReloaded:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    [self load:YES];
    [outlineview_ reloadData];
    [self filter:self];
    if (! ischeckbox_) {
      [self expand:self];
    }
  });
}

- (id) init
{
  self = [super init];

  if (self) {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_PreferencesChanged:)
                                                 name:kPreferencesChangedNotification
                                               object:nil];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_ConfigListChanged:)
                                                 name:kConfigListChangedNotification
                                               object:nil];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_ConfigXMLReloaded:)
                                                 name:kConfigXMLReloadedNotification
                                               object:nil];
  }

  return self;
}

- (void) dealloc
{
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void) load:(BOOL)force
{
  if (force) {
    if (datasource_) {
      datasource_ = nil;
    }
  }

  if (datasource_) return;

  if (ischeckbox_) {
    datasource_ = [xmlCompiler_ preferencepane_checkbox];
  } else {
    datasource_ = [xmlCompiler_ preferencepane_number];
  }
}

/* ---------------------------------------------------------------------- */
- (NSUInteger) outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item
{
  [self load:NO];

  // ----------------------------------------
  NSArray* a = nil;

  // root object
  if (! item) {
    a = datasource_;

  } else {
    a = item[@"children"];
  }

  if (! a) return 0;
  return [a count];
}

- (id) outlineView:(NSOutlineView*)outlineView child:(NSUInteger)idx ofItem:(id)item
{
  [self load:NO];

  // ----------------------------------------
  NSArray* a = nil;

  // root object
  if (! item) {
    a = datasource_;

  } else {
    a = item[@"children"];
  }

  if (! a) return nil;
  if (idx >= [a count]) return nil;
  return a[idx];
}

- (BOOL) outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item
{
  NSArray* a = item[@"children"];
  return a ? YES : NO;
}

- (id) outlineView:(NSOutlineView*)outlineView objectValueForTableColumn:(NSTableColumn*)tableColumn byItem:(id)item
{
  NSString* identifier = item[@"identifier"];

  if (ischeckbox_) {
    NSButtonCell* cell = [tableColumn dataCell];
    if (! cell) return nil;

    NSDictionary* attributes = item[@"stringAttributes"];
    if (attributes) {
      [cell setAttributedTitle:[[NSAttributedString alloc] initWithString:item[@"name"]
                                                               attributes:attributes]];
    } else {
      [cell setTitle:item[@"name"]];
    }

    if (! identifier || [identifier hasPrefix:@"notsave."]) {
      [cell setImagePosition:NSNoImage];
      return nil;

    } else {
      [cell setImagePosition:NSImageLeft];
      return @([preferencesManager_ value:identifier]);
    }

  } else {
    NSString* columnIdentifier = [tableColumn identifier];

    if ([columnIdentifier isEqualToString:@"name"]) {
      return item[columnIdentifier];

    } else if ([columnIdentifier isEqualToString:@"baseunit"] ||
               [columnIdentifier isEqualToString:@"default"]) {
      if (! identifier) {
        return nil;
      }
      return item[columnIdentifier];

    } else if ([columnIdentifier isEqualToString:@"value"]) {
      NSTextFieldCell* cell = [tableColumn dataCell];
      if (! cell) return nil;

      if (! identifier) {
        [cell setEditable:NO];
        return nil;
      } else {
        [cell setEditable:YES];
        return @([preferencesManager_ value:identifier]);
      }

    } else if ([columnIdentifier isEqualToString:@"stepper"]) {
      NSStepperCell* cell = [tableColumn dataCell];
      if (! identifier) {
        [cell setEnabled:NO];
      } else {
        [cell setEnabled:YES];
      }
      return nil;
    }
  }

  return nil;
}

- (CGFloat) outlineView:(NSOutlineView*)outlineView heightOfRowByItem:(id)item
{
  NSNumber* number = item[@"height"];
  if (! number || [number intValue] == 0) {
    number = @([outlineView rowHeight]);
  } else {
    number = @([number intValue] * (CGFloat)([outlineView rowHeight]));
  }

  return [number floatValue];
}

- (void) outlineView:(NSOutlineView*)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn byItem:(id)item
{
  NSString* identifier = item[@"identifier"];

  if (identifier) {
    if (ischeckbox_) {
      if (! [identifier hasPrefix:@"notsave."]) {
        int value = [preferencesManager_ value:identifier];
        value = ! value;
        [preferencesManager_ setValue:value forName:identifier];
      }

    } else {
      NSString* columnIdentifier = [tableColumn identifier];
      if ([columnIdentifier isEqualToString:@"value"]) {
        [preferencesManager_ setValue:[object intValue] forName:identifier];

      } else if ([columnIdentifier isEqualToString:@"stepper"]) {
        int newvalue = [preferencesManager_ value:identifier];
        NSNumber* step = item[@"step"];
        newvalue += ([object intValue]* [step intValue]);

        // confirm range
        if (newvalue < 0) {
          newvalue = 0;
        } else if (newvalue > 1073741824) { // 2^30
          newvalue = 1073741824;
        }

        [preferencesManager_ setValue:newvalue forName:identifier];

        [outlineView reloadItem:item];
      }
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

/* ---------------------------------------------------------------------- */
- (NSDictionary*) filterDataSource_core:(NSDictionary*)dictionary isEnabledOnly:(BOOL)isEnabledOnly strings:(NSArray*)strings
{
  // ------------------------------------------------------------
  // check children
  NSArray* children = dictionary[@"children"];
  if (children) {
    NSMutableArray* newchildren = [NSMutableArray new];
    for (NSDictionary* dict in children) {
      NSDictionary* d = [self filterDataSource_core:dict isEnabledOnly:isEnabledOnly strings:strings];
      if (d) {
        [newchildren addObject:d];
      }
    }

    if ([newchildren count] > 0) {
      NSMutableDictionary* newdictionary = [NSMutableDictionary dictionaryWithDictionary:dictionary];
      newdictionary[@"children"] = newchildren;
      return newdictionary;
    }
  }

  // ------------------------------------------------------------
  // filter by isEnabledOnly
  if (isEnabledOnly) {
    NSString* identifier = dictionary[@"identifier"];
    if (! identifier) {
      return nil;
    }
    if (! [preferencesManager_ value:identifier]) {
      return nil;
    }
  }

  // check self name
  NSString* string_for_filter = dictionary[@"string_for_filter"];
  if (string_for_filter) {
    BOOL hit = YES;
    for (NSString* s in strings) {
      if ([string_for_filter rangeOfString:s].location == NSNotFound) hit = NO;
    }
    if (hit) {
      return dictionary;
    }
  }

  return nil;
}

- (void) filterDataSource:(BOOL)isEnabledOnly string:(NSString*)string
{
  [self load:YES];
  if (! datasource_) return;

  NSMutableArray* newdatasource = [NSMutableArray new];

  NSMutableArray* strings = [NSMutableArray new];
  if (string) {
    for (NSString* s in [string componentsSeparatedByCharactersInSet :[NSCharacterSet whitespaceCharacterSet]]) {
      if ([s length] == 0) continue;
      [strings addObject:[s lowercaseString]];
    }
  }

  for (NSDictionary* dict in datasource_) {
    NSDictionary* d = [self filterDataSource_core:dict isEnabledOnly:isEnabledOnly strings:strings];
    if (d) {
      [newdatasource addObject:d];
    }
  }

  datasource_ = newdatasource;
  [outlineview_ reloadData];

  if ([string length] == 0 && isEnabledOnly == NO) {
    [outlineview_ collapseItem:nil collapseChildren:YES];
  } else {
    [outlineview_ expandItem:nil expandChildren:YES];
  }
}

- (IBAction) filter:(id)sender
{
  if (ischeckbox_) {
    BOOL isEnabledOnly = ([showEnabledOnly_ state] == NSOnState);
    [self filterDataSource:isEnabledOnly string:[searchText_ stringValue]];
  }
}

- (IBAction) expand:(id)sender
{
  [outlineview_ expandItem:nil expandChildren:YES];
  [outlineview_ setNeedsDisplay:YES];
}

- (IBAction) collapse:(id)sender
{
  [outlineview_ collapseItem:nil collapseChildren:YES];
  [outlineview_ setNeedsDisplay:YES];
}

@end
