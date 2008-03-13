// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "KeyRemap4MacBookPref.h"

@implementation KeyRemap4MacBookPref

// ----------------------------------------------------------------------
- (void) loadXML
{
  NSString *path = @"/Library/org.pqrs/KeyRemap4MacBook/prefpane/sysctl.xml";
  NSURL *url = [NSURL fileURLWithPath:path];
  _XMLDocument = [[NSXMLDocument alloc] initWithContentsOfURL:url options:0 error:NULL];
}

- (void) drawVersion
{
  NSString *version = [_sysctlWrapper getString:@"keyremap4macbook.version"];
  if (! version) {
    version = @"-.-.-";
  }
  [_versionText setStringValue:version];
}

- (void) loadKeyRepeat
{
  NSNumber *value = [_sysctlWrapper getInt:@"keyremap4macbook.repeat.initial_wait"];
  [_keyrepeat_initialWait setStringValue:[value stringValue]];

  value = [_sysctlWrapper getInt:@"keyremap4macbook.repeat.wait"];
  [_keyrepeat_wait setStringValue:[value stringValue]];
}


// ----------------------------------------------------------------------
- (void) saveSetting
{
  char command[] = "/Library/org.pqrs/KeyRemap4MacBook/scripts/save.sh";
  [_adminAction execCommand:command];
}

- (void) setSysctlInt:(NSString *)name value:(NSNumber *)value
{
  NSNumber *old = [_sysctlWrapper getInt:name];
  if ([value isEqualToNumber:old]) return;

  char command[512];
  snprintf(command, sizeof(command), "/usr/sbin/sysctl -w '%s=%d'",
           [name cStringUsingEncoding:NSUTF8StringEncoding],
           [value intValue]);
  if (! [_adminAction execCommand:command]) return;

  [self saveSetting];
}


// ----------------------------------------------------------------------
- (void) mainViewDidLoad
{
  _XMLDocument = nil;
  _sysctlWrapper = [[SysctlWrapper alloc] init];
  _adminAction = [[AdminAction alloc] init];

  [self loadXML];
  if (! _XMLDocument) return;

  [self drawVersion];
  [self loadKeyRepeat];
  [_outlineView reloadData];
}


// ----------------------------------------------------------------------
// for NSOutlineView
- (id) normalizeItem:(id) item
{
  if (! _XMLDocument) return nil;

  if (! item) {
    return [_XMLDocument rootElement];
  }
  return item;
}

- (int) numberOfChildren:(id) item
{
  item = [self normalizeItem:item];
  if (! item) return 0;

  NSArray *nodes = [item nodesForXPath:@"list/item" error:NULL];
  return [nodes count];
}

- (int) outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item
{
  return [self numberOfChildren:item];
}

- (id) outlineView:(NSOutlineView*)outlineView child:(int)index ofItem:(id)item
{
  item = [self normalizeItem:item];
  if (! item) return 0;

  NSArray *nodes = [item nodesForXPath:@"list/item" error:NULL];
  return [nodes objectAtIndex:index];
}

- (BOOL) outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item
{
  item = [self normalizeItem:item];
  if (! item) return 0;

  return [self numberOfChildren:item] > 0;
}

- (NSXMLNode *) getNode:(NSXMLNode *)node xpath:(NSString *)xpath
{
  NSArray *a = [node nodesForXPath:xpath error:NULL];
  if (a == nil) return nil;
  if ([a count] == 0) return nil;

  return [a objectAtIndex:0];
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

    NSXMLNode *sysctl = [self getNode:n xpath:@"sysctl"];
    if (sysctl) {
      NSNumber *value = [_sysctlWrapper getInt:[sysctl stringValue]];
      if ([value boolValue]) return TRUE;
    }
  }

  return FALSE;
}

- (id)outlineView:(NSOutlineView*)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
  // ----------------------------------------
  // autoexpand
  if ([_outlineView isExpandable:item]) {
    if (! [_outlineView isItemExpanded:item]) {
      if ([self checkAnyChildrenChecked:item]) {
        [_outlineView expandItem:item];
      }
    }
  }

  // ----------------------------------------
  NSButtonCell *cell = [tableColumn dataCell];
  if (! cell) return nil;

  NSXMLNode *title = [self getNode:item xpath:@"name"];
  if (! title) return nil;

  [cell setTitle:[title stringValue]];

  NSXMLNode *sysctl = [self getNode:item xpath:@"sysctl"];
  if (! sysctl) {
    [cell setImagePosition:NSNoImage];
    return nil;

  } else {
    [cell setImagePosition:NSImageLeft];
    return [_sysctlWrapper getInt:[sysctl stringValue]];
  }

  return nil;
}

- (BOOL)outlineView:(NSOutlineView *)outlineView shouldCollapseItem:(id)item
{
  return ! [self checkAnyChildrenChecked:item];
}

- (void)outlineView:(NSOutlineView *)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
  NSXMLNode *sysctl = [self getNode:item xpath:@"sysctl"];
  if (sysctl) {
    NSString *name = [sysctl stringValue];
    NSNumber *value = [_sysctlWrapper getInt:name];
    NSNumber *new = [[[NSNumber alloc] initWithBool:![value boolValue]] autorelease];
    [self setSysctlInt:name value:new];
  }
}

// ----------------------------------------------------------------------
- (IBAction)setKeyRepeat_initialWait:(id)sender {
  NSNumber *new = [[[NSNumber alloc] initWithInt:[[sender stringValue] intValue]] autorelease];
  [self setSysctlInt:@"keyremap4macbook.repeat.initial_wait" value:new];
}

- (IBAction)setKeyRepeat_wait:(id)sender {
  NSNumber *new = [[[NSNumber alloc] initWithInt:[[sender stringValue] intValue]] autorelease];
  [self setSysctlInt:@"keyremap4macbook.repeat.wait" value:new];
}

@end
