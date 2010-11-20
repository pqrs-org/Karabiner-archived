/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "OutlineView_number.h"

@implementation org_pqrs_KeyRemap4MacBook_OutlineView_number

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

  datasource_ = [[preferencesclient_ proxy] preferencepane_number];
  if (datasource_) {
    [datasource_ retain];
  }
}

/* ------------------------------------------------------------ */
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
  id columnIdentifier = [tableColumn identifier];

  if ([columnIdentifier isEqualToString:@"name"] ||
      [columnIdentifier isEqualToString:@"baseunit"] ||
      [columnIdentifier isEqualToString:@"default"]) {
    return [item objectForKey:columnIdentifier];

  } else if ([columnIdentifier isEqualToString:@"value"]) {
    NSString* identifier = [item objectForKey:@"identifier"];
    if (! identifier) return nil;

    return [NSNumber numberWithInt:[[preferencesclient_ proxy] value:identifier]];
  }

  return nil;
}

- (void) outlineView:(NSOutlineView*)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn byItem:(id)item
{
  NSString* identifier = [item objectForKey:@"identifier"];
  if (! identifier) return;

  [[preferencesclient_ proxy] setValueForName:[object intValue] forName:identifier];
}

@end
