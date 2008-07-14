// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "OutlineView_checkbox.h"
#import "sharecode.h"

@implementation org_pqrs_KeyRemap4MacBook_OutlineView_checkbox

static BUNDLEPREFIX_XMLTreeWrapper *_xmlTreeWrapper;
static NSString *sysctl_set = @"/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set";
static NSString *sysctl_ctl = @"/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_ctl";
static NSString *xmlpath = @"/Library/org.pqrs/KeyRemap4MacBook/prefpane/checkbox.xml";

- (id)init
{
  self = [super init];
  if (! self) return self;

  _xmlTreeWrapper = [[BUNDLEPREFIX_XMLTreeWrapper alloc] init];
  if (_xmlTreeWrapper == nil) return nil;
  if (! [_xmlTreeWrapper load:xmlpath]) return nil;
  return self;
}

- (IBAction) expandALL:(id)sender
{
  [_outlineView_checkbox reloadData];
  [BUNDLEPREFIX_OutlineViewUtil expandALL:_outlineView_checkbox];
}

- (IBAction) collapseALL:(id)sender
{
  [_outlineView_checkbox reloadData];
  [BUNDLEPREFIX_OutlineViewUtil collapseALL:_outlineView_checkbox];
}

// ------------------------------------------------------------
- (BOOL) checkNodeChecked:(NSXMLNode *)node
{
  NSXMLNode *sysctl = [_xmlTreeWrapper getNode:node xpath:@"sysctl"];

  if (sysctl) {
    NSString *entry = [NSString stringWithFormat:@"keyremap4macbook.%@", [sysctl stringValue]];
    NSNumber *value = [BUNDLEPREFIX_SysctlWrapper getInt:entry];
    if ([value boolValue]) return TRUE;
  }

  return FALSE;
}

- (BOOL) checkAnyChildrenChecked:(NSXMLNode *)node
{
  NSArray *a = [node nodesForXPath:@"list/item" error:NULL];
  if (a == nil) return FALSE;
  if ([a count] == 0) return FALSE;

  NSEnumerator *enumerator = [a objectEnumerator];
  NSXMLNode *n;
  while (n = [enumerator nextObject]) {
    if ([self checkAnyChildrenChecked:n]) return TRUE;
    if ([self checkNodeChecked:n]) return TRUE;
  }

  return FALSE;
}

- (void) showEnabledOnly:(NSXMLElement *)node
{
  NSArray *a = [node nodesForXPath:@"list/item" error:NULL];

  NSEnumerator *enumerator = [a objectEnumerator];
  NSXMLElement *n;
  while (n = [enumerator nextObject]) {
    if ([self checkNodeChecked:n] || [self checkAnyChildrenChecked:n]) {
      [self showEnabledOnly:n];
    } else {
      [n detach];
    }
  }
}

- (IBAction) toggleShowEnabled:(id)sender
{
  [_xmlTreeWrapper load:xmlpath];

  static BOOL showEnabledOnly = FALSE;
  showEnabledOnly = ! showEnabledOnly;
  if (! showEnabledOnly) {
    [_outlineView_checkbox reloadData];
    return;
  }

  [self showEnabledOnly:[_xmlTreeWrapper getRoot]];
  [_outlineView_checkbox reloadData];
  [BUNDLEPREFIX_OutlineViewUtil expandALL:_outlineView_checkbox];
}

// ----------------------------------------
- (BOOL) checkNodeSearchHit:(NSXMLNode *)node search:(NSString *)search
{
  NSXMLNode *text = [_xmlTreeWrapper getNode:node xpath:@"name"];
  if (text && [[[text stringValue] lowercaseString] rangeOfString:search].location != NSNotFound) return TRUE;

  text = [_xmlTreeWrapper getNode:node xpath:@"appendix"];
  if (text && [[[text stringValue] lowercaseString] rangeOfString:search].location != NSNotFound) return TRUE;

  return FALSE;
}

- (BOOL) checkAnyChildrenSearchHit:(NSXMLNode *)node search:(NSString *)search
{
  NSArray *a = [node nodesForXPath:@"list/item" error:NULL];
  if (a == nil) return FALSE;
  if ([a count] == 0) return FALSE;

  NSEnumerator *enumerator = [a objectEnumerator];
  NSXMLNode *n;
  while (n = [enumerator nextObject]) {
    if ([self checkAnyChildrenSearchHit:n search:search]) return TRUE;
    if ([self checkNodeSearchHit:n search:search]) return TRUE;
  }

  return FALSE;
}


- (IBAction) showSearchHitOnly:(NSXMLElement *)node search:(NSString *)search
{
  NSArray *a = [node nodesForXPath:@"list/item" error:NULL];

  NSEnumerator *enumerator = [a objectEnumerator];
  NSXMLElement *n;
  while (n = [enumerator nextObject]) {
    if ([self checkNodeSearchHit:n search:search] || [self checkAnyChildrenSearchHit:n search:search]) {
      [self showSearchHitOnly:n search:search];
    } else {
      [n detach];
    }
  }
}

- (IBAction) search:(id)sender
{
  [_xmlTreeWrapper load:xmlpath];

  NSArray *a = [[_searchText stringValue] componentsSeparatedByString:@" "];

  NSEnumerator *enumerator = [a objectEnumerator];
  NSString *str;
  while (str = [enumerator nextObject]) {
    str = [str lowercaseString];
    if (! [str isEqual:@""]) {
      [self showSearchHitOnly:[_xmlTreeWrapper getRoot] search:str];
    }
  }
  [_outlineView_checkbox reloadData];
  [BUNDLEPREFIX_OutlineViewUtil expandALL:_outlineView_checkbox];
}

// ----------------------------------------------------------------------
- (int) outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item
{
  return [_xmlTreeWrapper numberOfChildren:item];
}

- (id) outlineView:(NSOutlineView*)outlineView child:(int)index ofItem:(id)item
{
  return [_xmlTreeWrapper getChild:item index:index];
}

- (BOOL) outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item
{
  return [_xmlTreeWrapper isItemExpandable:item];
}

- (id)outlineView:(NSOutlineView*)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
  NSButtonCell *cell = [tableColumn dataCell];
  if (! cell) return nil;

  NSXMLNode *title = [_xmlTreeWrapper getNode:item xpath:@"name"];
  if (! title) return nil;

  NSXMLNode *appendix = [_xmlTreeWrapper getNode:item xpath:@"appendix"];
  if (appendix) {
    [cell setTitle:[NSString stringWithFormat:@"%@\n  %@", [title stringValue], [appendix stringValue]]];
  } else {
    [cell setTitle:[title stringValue]];
  }

  NSXMLNode *sysctl = [_xmlTreeWrapper getNode:item xpath:@"sysctl"];
  if (! sysctl) {
    [cell setImagePosition:NSNoImage];
    return nil;

  } else {
    [cell setImagePosition:NSImageLeft];
    NSString *entry = [NSString stringWithFormat:@"keyremap4macbook.%@", [sysctl stringValue]];
    return [BUNDLEPREFIX_SysctlWrapper getInt:entry];
  }

  return nil;
}

- (float)outlineView:(NSOutlineView *)outlineView heightOfRowByItem:(id)item
{
  NSXMLNode *appendix = [_xmlTreeWrapper getNode:item xpath:@"appendix"];
  if (appendix) {
    return [outlineView rowHeight] * 2;
  } else {
    return [outlineView rowHeight];
  }
}

- (void)outlineView:(NSOutlineView *)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
  NSXMLNode *sysctl = [_xmlTreeWrapper getNode:item xpath:@"sysctl"];
  if (sysctl) {
    NSString *name = [sysctl stringValue];
    NSString *entry = [NSString stringWithFormat:@"keyremap4macbook.%@", name];

    NSNumber *value = [BUNDLEPREFIX_SysctlWrapper getInt:entry];
    NSNumber *new = [[[NSNumber alloc] initWithBool:![value boolValue]] autorelease];
    [BUNDLEPREFIX_Common setSysctlInt:@"keyremap4macbook" name:name value:new sysctl_set:sysctl_set sysctl_ctl:sysctl_ctl];
  }
}

@end
