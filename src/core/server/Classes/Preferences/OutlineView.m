#import "NotificationKeys.h"
#import "OutlineView.h"
#import "PreferencesManager.h"
#import "XMLCompiler.h"

@interface OutlineView () {
  NSMutableDictionary* textsHeightCache_;
  dispatch_queue_t textsHeightQueue_;
}
@end

@implementation OutlineView

- (void)observer_PreferencesChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [outlineview_ setNeedsDisplay:YES];
  });
}

- (void)observer_ConfigListChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [outlineview_ setNeedsDisplay:YES];
    if ([showEnabledOnly_ state] == NSOnState) {
      [self filter:self];
    }
  });
}

- (void)observer_ConfigXMLReloaded:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self load:YES];
    [outlineview_ reloadData];
    [self filter:self];
    if (!ischeckbox_) {
      [self expand:self];
    }
  });
}

+ (NSFont*)font {
  return [NSFont systemFontOfSize:[NSFont smallSystemFontSize]];
}

+ (CGFloat)textsHeight:(NSUInteger)lineCount {
  if (lineCount == 0) return 0.0f;

  NSString* line = @"gM\n";
  NSUInteger length = [line length] * lineCount - 1; // skip last '\n'
  NSString* texts = [[NSString string] stringByPaddingToLength:length withString:line startingAtIndex:0];
  NSDictionary* attributes = @{NSFontAttributeName : [OutlineView font]};
  NSSize size = [texts sizeWithAttributes:attributes];
  return size.height;
}

- (id)init {
  self = [super init];

  if (self) {
    textsHeightCache_ = [NSMutableDictionary new];
    textsHeightQueue_ = dispatch_queue_create("org.pqrs.Karabiner.OutlineView.textsHeightQueue_", NULL);

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

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)load:(BOOL)force {
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
- (NSUInteger)outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item {
  [self load:NO];

  // ----------------------------------------
  NSArray* a = nil;

  // root object
  if (!item) {
    a = datasource_;

  } else {
    a = item[@"children"];
  }

  return [a count];
}

- (id)outlineView:(NSOutlineView*)outlineView child:(NSUInteger)idx ofItem:(id)item {
  [self load:NO];

  // ----------------------------------------
  NSArray* a = nil;

  // root object
  if (!item) {
    a = datasource_;

  } else {
    a = item[@"children"];
  }

  if (!a) return nil;
  if (idx >= [a count]) return nil;
  return a[idx];
}

- (BOOL)outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item {
  NSArray* a = item[@"children"];
  return a ? YES : NO;
}

- (BOOL)isTextCell:(NSTableColumn*)tableColumn item:(id)item {
  if (ischeckbox_) {
    NSString* identifier = item[@"identifier"];
    return (!identifier || [identifier hasPrefix:@"notsave."]);
  } else {
    return NO;
  }
}

- (NSCell*)outlineView:(NSOutlineView*)outlineView dataCellForTableColumn:(NSTableColumn*)tableColumn item:(id)item {
  if (!tableColumn) return nil;

  if (ischeckbox_) {
    NSButtonCell* cell = [NSButtonCell new];
    [cell setFont:[OutlineView font]];
    [cell setButtonType:NSSwitchButton];
    [cell setTitle:item[@"name"]];

    {
      NSColor* backgroundColor = nil;
      NSString* style = item[@"style"];
      if ([style isEqualToString:@"caution"]) {
        backgroundColor = [NSColor greenColor];
      } else if ([style isEqualToString:@"important"]) {
        backgroundColor = [NSColor orangeColor];
      } else if ([style isEqualToString:@"slight"]) {
        backgroundColor = [NSColor lightGrayColor];
      }

      if (backgroundColor) {
        [cell setBackgroundColor:backgroundColor];
      }
    }

    if ([self isTextCell:tableColumn item:item]) {
      [cell setImagePosition:NSNoImage];
    }

    return cell;

  } else {
    NSString* identifier = item[@"identifier"];
    NSString* columnIdentifier = [tableColumn identifier];

    if (identifier) {
      if ([columnIdentifier isEqualToString:@"value"]) {
        NSTextFieldCell* cell = [NSTextFieldCell new];
        [cell setFont:[OutlineView font]];
        [cell setEditable:YES];
        return cell;

      } else if ([columnIdentifier isEqualToString:@"stepper"]) {
        NSStepperCell* cell = [NSStepperCell new];
        [cell setFont:[OutlineView font]];

        [cell setMinValue:0];
        [cell setMaxValue:1073741824]; // 2^30
        [cell setIncrement:[item[@"step"] intValue]];
        [cell setAutorepeat:NO];
        [cell setValueWraps:NO];
        return cell;
      }
    }

    // ----------------------------------------
    NSCell* cell = [NSCell new];
    [cell setFont:[OutlineView font]];

    if ([columnIdentifier isEqualToString:@"default"]) {
      [cell setEnabled:NO];
    }

    return cell;
  }
}

- (id)outlineView:(NSOutlineView*)outlineView objectValueForTableColumn:(NSTableColumn*)tableColumn byItem:(id)item {
  NSString* identifier = item[@"identifier"];

  if (ischeckbox_) {
    if ([self isTextCell:tableColumn item:item]) {
      return nil;
    } else {
      return @([preferencesManager_ value:identifier]);
    }

  } else {
    NSString* columnIdentifier = [tableColumn identifier];

    if ([columnIdentifier isEqualToString:@"name"]) {
      return item[columnIdentifier];

    } else if ([columnIdentifier isEqualToString:@"baseunit"] ||
               [columnIdentifier isEqualToString:@"default"]) {
      if (!identifier) return nil;
      return item[columnIdentifier];

    } else if ([columnIdentifier isEqualToString:@"value"] ||
               [columnIdentifier isEqualToString:@"stepper"]) {
      if (!identifier) return nil;

      // @(12345) will be @"12,345" in view.
      // So we return NSString to show numbers without comma.
      return [[NSString alloc] initWithFormat:@"%d", [preferencesManager_ value:identifier]];
    }
  }

  return nil;
}

- (CGFloat)outlineView:(NSOutlineView*)outlineView heightOfRowByItem:(id)item {
  NSNumber* lineCount = item[@"height"];
  __block NSNumber* height = @([outlineView rowHeight]);

  if ([lineCount integerValue] > 1) {
    dispatch_sync(textsHeightQueue_, ^{
      if (textsHeightCache_[lineCount] == nil) {
        textsHeightCache_[lineCount] = @([OutlineView textsHeight:[lineCount unsignedIntegerValue]]);
      }
      // We add [outlineView rowHeight] as vertical margin.
      height = @([textsHeightCache_[lineCount] floatValue] + [outlineView rowHeight]);
    });
  }

  return [height floatValue];
}

- (void)outlineView:(NSOutlineView*)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn byItem:(id)item {
  NSString* identifier = item[@"identifier"];

  if (identifier) {
    if (ischeckbox_) {
      if (![self isTextCell:tableColumn item:item]) {
        int value = [preferencesManager_ value:identifier];
        value = !value;
        [preferencesManager_ setValue:value forName:identifier];
      }

    } else {
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

        [preferencesManager_ setValue:(int)(newvalue) forName:identifier];
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
- (NSDictionary*)filterDataSource_core:(NSDictionary*)dictionary isEnabledOnly:(BOOL)isEnabledOnly strings:(NSArray*)strings {
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
    if (!identifier) {
      return nil;
    }
    if (![preferencesManager_ value:identifier]) {
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

- (void)filterDataSource:(BOOL)isEnabledOnly string:(NSString*)string {
  [self load:YES];
  if (!datasource_) return;

  NSMutableArray* newdatasource = [NSMutableArray new];

  NSMutableArray* strings = [NSMutableArray new];
  if (string) {
    for (NSString* s in [string componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceCharacterSet]]) {
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

- (IBAction)filter:(id)sender {
  if (ischeckbox_) {
    BOOL isEnabledOnly = ([showEnabledOnly_ state] == NSOnState);
    [self filterDataSource:isEnabledOnly string:[searchText_ stringValue]];
  }
}

- (IBAction)expand:(id)sender {
  [outlineview_ expandItem:nil expandChildren:YES];
  [outlineview_ setNeedsDisplay:YES];
}

- (IBAction)collapse:(id)sender {
  [outlineview_ collapseItem:nil collapseChildren:YES];
  [outlineview_ setNeedsDisplay:YES];
}

@end
