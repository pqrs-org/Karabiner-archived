#import "DataSource.h"

@implementation org_pqrs_KeyRemap4MacBook_DataSource

- initWithSelector:(SEL)sel preferencesclient:(org_pqrs_KeyRemap4MacBook_PreferencesClient*)client;
{
  [super init];
  selector_ = sel;
  preferencesclient_ = client;

  return self;
}

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

  datasource_ = [[preferencesclient_ proxy] performSelector:selector_];
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
  NSButtonCell* cell = [tableColumn dataCell];
  if (! cell) return nil;

  [cell setTitle:[item objectForKey:@"name"]];
  NSString* identifier = [item objectForKey:@"identifier"];

  if (! identifier || [identifier hasPrefix:@"notsave."]) {
    [cell setImagePosition:NSNoImage];
    return nil;

  } else {
    [cell setImagePosition:NSImageLeft];
    return [NSNumber numberWithInt:[[preferencesclient_ proxy] value:identifier]];
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
}

@end
