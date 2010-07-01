/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "OutlineView_checkbox.h"
#import "SysctlWrapper.h"
#import "XMLTreeWrapper.h"

@implementation org_pqrs_KeyRemap4MacBook_OutlineView_checkbox

static BUNDLEPREFIX(XMLTreeWrapper) * _xmlTreeWrapper;
static NSString* sysctl_set = @"/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set";
static NSString* sysctl_ctl = @"/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_ctl";
static NSString* xmlpath = @"/Library/org.pqrs/KeyRemap4MacBook/prefpane/checkbox.xml";

- (id) init
{
  self = [super init];
  if (! self) return self;

  _xmlTreeWrapper = [[BUNDLEPREFIX(XMLTreeWrapper) alloc] init];
  if (_xmlTreeWrapper == nil) return nil;
  if (! [_xmlTreeWrapper load:xmlpath]) return nil;
  return self;
}

- (IBAction) expandALL:(id)sender
{
  [_outlineView_checkbox expandItem:nil expandChildren:YES];
}

- (IBAction) collapseALL:(id)sender
{
  [_outlineView_checkbox collapseItem:nil collapseChildren:YES];
}

/* ------------------------------------------------------------ */
- (BOOL) filter_sysctl:(NSXMLNode*)node
{
  NSXMLNode* sysctl = [_xmlTreeWrapper getNode:node xpath:@"sysctl"];

  if (sysctl) {
    NSString* entry = [NSString stringWithFormat:@"keyremap4macbook.%@", [sysctl stringValue]];
    NSNumber* value = [BUNDLEPREFIX(SysctlWrapper) getInt:entry];
    if ([value boolValue]) return TRUE;
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

- (BOOL) filter_checkChildren:(NSXMLNode*)node sysctl:(BOOL)sysctl search:(NSString*)search
{
  NSArray* a = [node nodesForXPath:@"list/item" error:NULL];
  if (a == nil) return FALSE;

  for (NSXMLNode* n in a) {
    if (! n) break;

    if ([self filter_checkChildren:n sysctl:sysctl search:search]) return TRUE;
    if (sysctl) {
      if ([self filter_sysctl:n]) return TRUE;
    }
    if (search != nil) {
      if ([self filter_search:n search:search]) return TRUE;
    }
  }

  return FALSE;
}

- (void) filter_core:(NSXMLElement*)node sysctl:(BOOL)sysctl search:(NSString*)search
{
  NSArray* a = [node nodesForXPath:@"list/item" error:NULL];
  if (a == nil) return;

  for (NSXMLElement* n in a) {
    if (! n) break;

    if (sysctl) {
      if ([self filter_sysctl:n] || [self filter_checkChildren:n sysctl:sysctl search:search]) {
        [self filter_core:n sysctl:sysctl search:search];
      } else {
        [n detach];
      }
    } else if (search) {
      if ([self filter_search:n search:search]) continue;
      if ([self filter_checkChildren:n sysctl:sysctl search:search]) {
        [self filter_core:n sysctl:sysctl search:search];
      } else {
        [n detach];
      }
    }
  }
}

- (IBAction) filter:(id)sender
{
  [_xmlTreeWrapper load:xmlpath];

  BOOL expand = FALSE;

  if ([_showEnabledOnly state] == NSOnState) {
    [self filter_core:[_xmlTreeWrapper getRoot] sysctl:TRUE search:nil];
    expand = TRUE;
  }
  if (! [[_searchText stringValue] isEqual:@""]) {
    NSArray* a = [[_searchText stringValue] componentsSeparatedByString:@" "];
    for (NSString* str in a) {
      if (! str) break;

      str = [str lowercaseString];
      if (! [str isEqual:@""]) {
        [self filter_core:[_xmlTreeWrapper getRoot] sysctl:FALSE search:str];
      }
    }
    expand = TRUE;
  }

  [_outlineView_checkbox reloadData];
  if (expand) {
    [self expandALL:nil];
  }
}

/* ---------------------------------------------------------------------- */
- (NSUInteger) outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item
{
  return [_xmlTreeWrapper numberOfChildren:item];
}

- (id) outlineView:(NSOutlineView*)outlineView child:(NSUInteger)idx ofItem:(id)item
{
  return [_xmlTreeWrapper getChild:item index:idx];
}

- (BOOL) outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item
{
  return [_xmlTreeWrapper isItemExpandable:item];
}

- (id) outlineView:(NSOutlineView*)outlineView objectValueForTableColumn:(NSTableColumn*)tableColumn byItem:(id)item
{
  NSButtonCell* cell = [tableColumn dataCell];
  if (! cell) return nil;

  // ------------------------------------------------------------
  NSXMLNode* title = [_xmlTreeWrapper getNode:item xpath:@"name"];
  if (! title) return nil;

  NSMutableString* titlestring = [NSMutableString stringWithString:[title stringValue]];

  NSArray* a = [item nodesForXPath:@"appendix" error:NULL];
  if (a) {
    for (NSXMLNode* appendix in a) {
      if (! appendix) break;

      [titlestring appendString:@"\n  "];
      [titlestring appendString:[appendix stringValue]];
    }
  }

  [cell setTitle:titlestring];

  // ------------------------------------------------------------
  NSXMLNode* sysctl = [_xmlTreeWrapper getNode:item xpath:@"sysctl"];
  if (! sysctl || [[sysctl stringValue] hasPrefix:@"notsave."]) {
    [cell setImagePosition:NSNoImage];
    return nil;

  } else {
    [cell setImagePosition:NSImageLeft];
    NSString* entry = [NSString stringWithFormat:@"keyremap4macbook.%@", [sysctl stringValue]];
    return [BUNDLEPREFIX(SysctlWrapper) getInt:entry];
  }

  return nil;
}

- (CGFloat) outlineView:(NSOutlineView*)outlineView heightOfRowByItem:(id)item
{
  NSUInteger appendixnum = 0;
  NSArray* a = [item nodesForXPath:@"appendix" error:NULL];
  if (a) {
    appendixnum = [a count];
  }
  return [outlineView rowHeight] * (1 + appendixnum);
}

- (void) outlineView:(NSOutlineView*)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn byItem:(id)item
{
  NSXMLNode* sysctl = [_xmlTreeWrapper getNode:item xpath:@"sysctl"];
  if (sysctl) {
    NSString* name = [sysctl stringValue];
    if (! [name hasPrefix:@"notsave."]) {
      NSString* entry = [NSString stringWithFormat:@"keyremap4macbook.%@", name];

      NSNumber* value = [BUNDLEPREFIX(SysctlWrapper) getInt:entry];
      NSNumber* new = [[[NSNumber alloc] initWithBool:! [value boolValue]] autorelease];
      [BUNDLEPREFIX(SysctlWrapper) setSysctlInt:@"keyremap4macbook" name:name value:new sysctl_set:sysctl_set sysctl_ctl:sysctl_ctl];
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
