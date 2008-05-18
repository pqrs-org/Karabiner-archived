// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "OutlineView_checkbox.h"
#import "sharecode.h"

@implementation org_pqrs_KeyRemap4MacBook_OutlineView_checkbox

static BUNDLEPREFIX_XMLTreeWrapper *_xmlTreeWrapper;
static NSString *sysctl_set = @"/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set";
static NSString *sysctl_ctl = @"/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_ctl";
static BOOL showEnabledOnly = FALSE;

- (id)init
{
  self = [super init];
  if (! self) return self;

  _xmlTreeWrapper = [[BUNDLEPREFIX_XMLTreeWrapper alloc] init];
  if (_xmlTreeWrapper == nil) return nil;
  if (! [_xmlTreeWrapper load:@"/Library/org.pqrs/KeyRemap4MacBook/prefpane/checkbox.xml"]) return nil;
  return self;
}

- (IBAction) expandALL:(id)sender
{
  [BUNDLEPREFIX_OutlineViewUtil expandALL:_outlineView_checkbox];
}

- (IBAction) collapseALL:(id)sender
{
  [BUNDLEPREFIX_OutlineViewUtil collapseALL:_outlineView_checkbox delegater:self];
}

- (IBAction) showEnabled:(id)sender
{
  showEnabledOnly = TRUE;
  [BUNDLEPREFIX_OutlineViewUtil expandALL:_outlineView_checkbox];
  [BUNDLEPREFIX_OutlineViewUtil collapseALL:_outlineView_checkbox delegater:self];
  showEnabledOnly = FALSE;
}

// ------------------------------------------------------------
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

- (BOOL) checkAnyChildrenChecked:(NSXMLNode *)node
{
  NSArray *a = [node nodesForXPath:@"list/item" error:NULL];
  if (a == nil) return FALSE;
  if ([a count] == 0) return FALSE;

  NSEnumerator *enumerator = [a objectEnumerator];
  NSXMLNode *n;
  while (n = [enumerator nextObject]) {
    if ([self checkAnyChildrenChecked:n]) return TRUE;

    NSXMLNode *sysctl = [_xmlTreeWrapper getNode:n xpath:@"sysctl"];
    if (sysctl) {
      NSString *entry = [NSString stringWithFormat:@"keyremap4macbook.%@", [sysctl stringValue]];
      NSNumber *value = [BUNDLEPREFIX_SysctlWrapper getInt:entry];
      if ([value boolValue]) return TRUE;
    }
  }

  return FALSE;
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

- (BOOL)outlineView:(NSOutlineView *)outlineView shouldCollapseItem:(id)item
{
  if (showEnabledOnly) {
    return ! [self checkAnyChildrenChecked:item];
  } else {
    return TRUE;
  }
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
