#import "OutlineView.h"

@implementation org_pqrs_KeyRemap4MacBook_OutlineView

- (void) dealloc
{
  if (datasource_) {
    [datasource_ release];
  }

  [super dealloc];
}

- (void) load:(BOOL)force
{
  if (force) {
    if (datasource_) {
      [datasource_ release];
      datasource_ = nil;
    }
  }

  if (datasource_) return;

  if (ischeckbox_) {
    datasource_ = [[preferencesclient_ proxy] preferencepane_checkbox];
  } else {
    datasource_ = [[preferencesclient_ proxy] preferencepane_number];
  }
  if (datasource_) {
    [datasource_ retain];
  }
}

/* ---------------------------------------------------------------------- */
- (NSUInteger) outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item
{
  NSArray* a = nil;

  // root object
  if (! item) {
    [self load:NO];
    a = datasource_;

  } else {
    a = [item objectForKey:@"children"];
  }

  if (! a) return 0;
  return [a count];
}

- (id) outlineView:(NSOutlineView*)outlineView child:(NSUInteger)idx ofItem:(id)item
{
  NSArray* a = nil;

  // root object
  if (! item) {
    [self load:NO];
    a = datasource_;

  } else {
    a = [item objectForKey:@"children"];
  }

  if (! a) return nil;
  if (idx >= [a count]) return nil;
  return [a objectAtIndex:idx];
}

- (BOOL) outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item
{
  NSArray* a = [item objectForKey:@"children"];
  return a ? YES : NO;
}

- (id) outlineView:(NSOutlineView*)outlineView objectValueForTableColumn:(NSTableColumn*)tableColumn byItem:(id)item
{
  NSString* identifier = [item objectForKey:@"identifier"];

  if (ischeckbox_) {
    NSButtonCell* cell = [tableColumn dataCell];
    if (! cell) return nil;

    [cell setTitle:[item objectForKey:@"name"]];

    if (! identifier || [identifier hasPrefix:@"notsave."]) {
      [cell setImagePosition:NSNoImage];
      return nil;

    } else {
      [cell setImagePosition:NSImageLeft];
      return [NSNumber numberWithInt:[[preferencesclient_ proxy] value:identifier]];
    }

  } else {
    NSString* columnIdentifier = [tableColumn identifier];

    if ([columnIdentifier isEqualToString:@"name"]) {
      return [item objectForKey:columnIdentifier];

    } else if ([columnIdentifier isEqualToString:@"baseunit"] ||
               [columnIdentifier isEqualToString:@"default"]) {
      if (! identifier) {
        return nil;
      }
      return [item objectForKey:columnIdentifier];

    } else if ([columnIdentifier isEqualToString:@"value"]) {
      NSTextFieldCell* cell = [tableColumn dataCell];
      if (! cell) return nil;

      if (! identifier) {
        [cell setEditable:NO];
        return nil;
      } else {
        [cell setEditable:YES];
        return [NSNumber numberWithInt:[[preferencesclient_ proxy] value:identifier]];
      }
    }
  }

  return nil;
}

- (CGFloat) outlineView:(NSOutlineView*)outlineView heightOfRowByItem:(id)item
{
  NSNumber* number = [item objectForKey:@"cached_height"];
  if (number) return [number floatValue];

  number = [item objectForKey:@"height"];
  if (! number) {
    number = [NSNumber numberWithFloat:0];
  } else {
    number = [NSNumber numberWithDouble:([number intValue] * (CGFloat)([outlineView rowHeight]))];
  }
  return [number floatValue];
}

- (void) outlineView:(NSOutlineView*)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn byItem:(id)item
{
  NSString* identifier = [item objectForKey:@"identifier"];

  if (ischeckbox_) {
    if (identifier) {
      if (! [identifier hasPrefix:@"notsave."]) {
        int value = [[preferencesclient_ proxy] value:identifier];
        value = ! value;
        [[preferencesclient_ proxy] setValueForName:value forName:identifier];
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

  } else {
    if (identifier) {
      NSString* columnIdentifier = [tableColumn identifier];
      if ([columnIdentifier isEqualToString:@"value"]) {
        [[preferencesclient_ proxy] setValueForName:[object intValue] forName:identifier];

      } else if ([columnIdentifier isEqualToString:@"stepper"]) {
        int newvalue = [[preferencesclient_ proxy] value:identifier];
        NSNumber* step = [item objectForKey:@"step"];
        newvalue += ([object intValue] * [step intValue]);
        [[preferencesclient_ proxy] setValueForName:newvalue forName:identifier];

        [outlineView reloadItem:item];
      }
    }
  }
}

/* ---------------------------------------------------------------------- */
- (NSDictionary*) filterDataSource_core:(NSDictionary*)dictionary isEnabledOnly:(BOOL)isEnabledOnly strings:(NSArray*)strings
{
  // ------------------------------------------------------------
  // check children
  NSArray* children = [dictionary objectForKey:@"children"];
  if (children) {
    NSMutableArray* newchildren = [[NSMutableArray new] autorelease];
    for (NSDictionary* dict in children) {
      NSDictionary* d = [self filterDataSource_core:dict isEnabledOnly:isEnabledOnly strings:strings];
      if (d) {
        [newchildren addObject:d];
      }
    }

    if ([newchildren count] > 0) {
      NSMutableDictionary* newdictionary = [NSMutableDictionary dictionaryWithDictionary:dictionary];
      [newdictionary setObject:newchildren forKey:@"children"];
      return newdictionary;
    }
  }

  // ------------------------------------------------------------
  // filter by isEnabledOnly
  if (isEnabledOnly) {
    NSString* identifier = [dictionary objectForKey:@"identifier"];
    if (! identifier) {
      return nil;
    }
    if (! [[preferencesclient_ proxy] value:identifier]) {
      return nil;
    }
  }

  // check self name
  NSString* string_for_filter = [dictionary objectForKey:@"string_for_filter"];
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

  NSMutableArray* newdatasource = [[NSMutableArray new] autorelease];

  NSMutableArray* strings = [[NSMutableArray new] autorelease];
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

  [datasource_ release];
  datasource_ = [newdatasource retain];
  [outlineview_ reloadData];
  [outlineview_ expandItem:nil expandChildren:YES];
}

@end
