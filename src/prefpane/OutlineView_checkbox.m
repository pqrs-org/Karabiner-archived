// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "OutlineView_checkbox.h"
#import "SysctlWrapper.h"
#import "Common.h"
#import "OutlineViewUtil.h"

@implementation OutlineView_checkbox

static XMLTreeWrapper *_xmlTreeWrapper;

- (id)init
{
  self = [super init];
  if (! self) return self;

  _xmlTreeWrapper = [[XMLTreeWrapper alloc] init];
  if (_xmlTreeWrapper == nil) return nil;
  if (! [_xmlTreeWrapper load:@"/Library/org.pqrs/KeyRemap4MacBook/prefpane/checkbox.xml"]) return nil;
  return self;
}

- (IBAction) intelligentExpand:(id)sender
{
  [OutlineViewUtil intelligentExpand:_outlineView_checkbox];
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
      NSNumber *value = [SysctlWrapper getInt:entry];
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
    return [SysctlWrapper getInt:entry];
  }

  return nil;
}

- (BOOL)outlineView:(NSOutlineView *)outlineView shouldCollapseItem:(id)item
{
  return ! [self checkAnyChildrenChecked:item];
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

    NSNumber *value = [SysctlWrapper getInt:entry];
    NSNumber *new = [[[NSNumber alloc] initWithBool:![value boolValue]] autorelease];
    [Common setSysctlInt:name value:new];
  }
}

@end
