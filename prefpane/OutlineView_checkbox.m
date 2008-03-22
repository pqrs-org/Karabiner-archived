// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "OutlineView_checkbox.h"
#import "SysctlWrapper.h"
#import "Common.h"

@implementation OutlineView_checkbox

- (id)init
{
  self = [super init];
  if (self) {
    _xmlTreeWrapper = [[XMLTreeWrapper alloc] init];
    if (_xmlTreeWrapper == nil) return nil;
    if (! [_xmlTreeWrapper load:@"/Library/org.pqrs/KeyRemap4MacBook/prefpane/checkbox.xml"]) return nil;
    [_outlineView reloadData];
  }
  return self;
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
      NSNumber *value = [SysctlWrapper getInt:[sysctl stringValue]];
      if ([value boolValue]) return TRUE;
    }
  }

  return FALSE;
}

- (id)outlineView:(NSOutlineView*)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
  // ----------------------------------------
  // autoexpand
  if ([outlineView isExpandable:item]) {
    if (! [outlineView isItemExpanded:item]) {
      if ([self checkAnyChildrenChecked:item]) {
        [outlineView expandItem:item];
      }
    }
  }

  // ----------------------------------------
  NSButtonCell *cell = [tableColumn dataCell];
  if (! cell) return nil;

  NSXMLNode *title = [_xmlTreeWrapper getNode:item xpath:@"name"];
  if (! title) return nil;

  [cell setTitle:[title stringValue]];

  NSXMLNode *sysctl = [_xmlTreeWrapper getNode:item xpath:@"sysctl"];
  if (! sysctl) {
    [cell setImagePosition:NSNoImage];
    return nil;

  } else {
    [cell setImagePosition:NSImageLeft];
    return [SysctlWrapper getInt:[sysctl stringValue]];
  }

  return nil;
}

- (BOOL)outlineView:(NSOutlineView *)outlineView shouldCollapseItem:(id)item
{
  return ! [self checkAnyChildrenChecked:item];
}

- (void)outlineView:(NSOutlineView *)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
  NSXMLNode *sysctl = [_xmlTreeWrapper getNode:item xpath:@"sysctl"];
  if (sysctl) {
    NSString *name = [sysctl stringValue];
    NSNumber *value = [SysctlWrapper getInt:name];
    NSNumber *new = [[[NSNumber alloc] initWithBool:![value boolValue]] autorelease];
    [Common setSysctlInt:name value:new];
  }
}

@end
