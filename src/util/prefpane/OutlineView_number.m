// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "OutlineView_number.h"
#import "sharecode.h"

@implementation org_pqrs_KeyRemap4MacBook_OutlineView_number

static BUNDLEPREFIX_XMLTreeWrapper *_xmlTreeWrapper;
static NSString *sysctl_set = @"/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set";
static NSString *sysctl_ctl = @"/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_ctl";

- (id)init
{
  self = [super init];
  if (self) {
    _xmlTreeWrapper = [[BUNDLEPREFIX_XMLTreeWrapper alloc] init];
    if (_xmlTreeWrapper == nil) return nil;
    if (! [_xmlTreeWrapper load:@"/Library/org.pqrs/KeyRemap4MacBook/prefpane/number.xml"]) return nil;
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

- (id)outlineView:(NSOutlineView*)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
  id identifier = [tableColumn identifier];

  if ([identifier isEqualToString:@"name"] ||
      [identifier isEqualToString:@"baseunit"] ||
      [identifier isEqualToString:@"default"]) {
    NSXMLNode *node = [_xmlTreeWrapper getNode:item xpath:identifier];
    if (! node) return nil;

    return [node stringValue];

  } else if ([identifier isEqualToString:@"value"]) {
    NSXMLNode *sysctl = [_xmlTreeWrapper getNode:item xpath:@"sysctl"];
    if (! sysctl) return nil;

    NSString *entry = [NSString stringWithFormat:@"keyremap4macbook.%@", [sysctl stringValue]];
    return [BUNDLEPREFIX_SysctlWrapper getInt:entry];
  }

  return nil;
}

- (BOOL)outlineView:(NSOutlineView *)outlineView shouldCollapseItem:(id)item
{
  return false;
}

- (void)outlineView:(NSOutlineView *)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
  NSXMLNode *sysctl = [_xmlTreeWrapper getNode:item xpath:@"sysctl"];
  if (sysctl) {
    NSString *name = [sysctl stringValue];
    NSNumber *new = [[[NSNumber alloc] initWithInt:[object intValue]] autorelease];
    [BUNDLEPREFIX_Common setSysctlInt:@"keyremap4macbook" name:name value:new sysctl_set:sysctl_set sysctl_ctl:sysctl_ctl];
  }
}

@end
