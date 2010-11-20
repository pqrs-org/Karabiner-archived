/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "OutlineView_checkbox.h"

@implementation org_pqrs_KeyRemap4MacBook_OutlineView_checkbox

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

  datasource_ = [[preferencesclient_ proxy] preferencepane_checkbox];
  if (datasource_) {
    [datasource_ retain];
  }
}

- (IBAction) reloadXML:(id)sender
{
  // reload xml on server process.
  [[preferencesclient_ proxy] configxml_reload];

  // wait until xml reloaded.
  int trycount = 0;
  for (trycount = 0; trycount < 10; ++trycount) {
    [NSThread sleepForTimeInterval:0.5];
    if ([[preferencesclient_ proxy] configxml_initialized]) break;
  }

  // ----------------------------------------
  // update prefpane
  [self load:YES];

  // ----------------------------------------
  [outlineview_checkbox_ reloadData];
}

/* ------------------------------------------------------------ */
#if 0
- (BOOL) filter_identifier:(NSXMLNode*)node
{
  NSXMLNode* identifier = [_xmlTreeWrapper getNode:node xpath:@"identifier"];

  if (identifier) {
    int value = [[preferencesclient_ proxy] value:[identifier stringValue]];
    if (value) return TRUE;
  }

  return FALSE;
}

- (BOOL) filter_search:(NSXMLNode*)node search:(NSString*)search
{
  NSXMLNode* text = [_xmlTreeWrapper getNode:node xpath:@"name"];
  if (text && [[[text stringValue] lowercaseString] rangeOfString:search].location != NSNotFound) return TRUE;

  NSArray* a = [node nodesForXPath:@"appendix" error:NULL];
  if (a) {
    for (NSXMLNode* appendix in a) {
      if (! appendix) continue;

      if ([[[appendix stringValue] lowercaseString] rangeOfString:search].location != NSNotFound) return TRUE;
    }
  }

  return FALSE;
}

- (BOOL) filter_checkChildren:(NSXMLNode*)node identifier:(BOOL)identifier search:(NSString*)search
{
  NSArray* a = [node nodesForXPath:@"list/item" error:NULL];
  if (a == nil) return FALSE;

  for (NSXMLNode* n in a) {
    if (! n) break;

    if ([self filter_checkChildren:n identifier:identifier search:search]) return TRUE;
    if (identifier) {
      if ([self filter_identifier:n]) return TRUE;
    }
    if (search != nil) {
      if ([self filter_search:n search:search]) return TRUE;
    }
  }

  return FALSE;
}

- (void) filter_core:(NSXMLElement*)node identifier:(BOOL)identifier search:(NSString*)search
{
  NSArray* a = [node nodesForXPath:@"list/item" error:NULL];
  if (a == nil) return;

  for (NSXMLElement* n in a) {
    if (! n) break;

    if (identifier) {
      if ([self filter_identifier:n] || [self filter_checkChildren:n identifier:identifier search:search]) {
        [self filter_core:n identifier:identifier search:search];
      } else {
        [n detach];
      }
    } else if (search) {
      if ([self filter_search:n search:search]) continue;
      if ([self filter_checkChildren:n identifier:identifier search:search]) {
        [self filter_core:n identifier:identifier search:search];
      } else {
        [n detach];
      }
    }
  }
}
#endif

- (IBAction) filter:(id)sender
{
  return;
#if 0
  [_xmlTreeWrapper load:xmlpath];

  BOOL expand = FALSE;

  if ([_showEnabledOnly state] == NSOnState) {
    [self filter_core:[_xmlTreeWrapper getRoot] identifier:TRUE search:nil];
    expand = TRUE;
  }
  if (! [[_searchText stringValue] isEqual:@""]) {
    NSArray* a = [[_searchText stringValue] componentsSeparatedByString:@" "];
    for (NSString* str in a) {
      if (! str) break;

      str = [str lowercaseString];
      if (! [str isEqual:@""]) {
        [self filter_core:[_xmlTreeWrapper getRoot] identifier:FALSE search:str];
      }
    }
    expand = TRUE;
  }

  [_outlineView_checkbox reloadData];
  if (expand) {
    [_outlineView_checkbox expandItem:nil expandChildren:YES];
  }
#endif
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
