#import "KeyRemap4MacBookPref.h"

@implementation KeyRemap4MacBookPref

- (void) loadXML
{
  NSString *path = @"/Applications/KeyRemap4MacBook/prefpane/sysctl.xml";
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

- (id)outlineView:(NSOutlineView*)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
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

- (void)outlineView:(NSOutlineView *)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
  NSXMLNode *sysctl = [self getNode:item xpath:@"sysctl"];
  if (sysctl) {
    NSString *name = [sysctl stringValue];
    NSNumber *value = [_sysctlWrapper getInt:name];
    NSNumber *new = [[[NSNumber alloc] initWithBool:![value boolValue]] autorelease];
    [_adminAction setSysctlInt:name value:new];
  }
}

// ----------------------------------------------------------------------
- (IBAction)setKeyRepeat_initialWait:(id)sender {
  NSNumber *new = [[[NSNumber alloc] initWithInt:[[sender stringValue] intValue]] autorelease];
  [_adminAction setSysctlInt:@"keyremap4macbook.repeat.initial_wait" value:new];
}

- (IBAction)setKeyRepeat_wait:(id)sender {
  NSNumber *new = [[[NSNumber alloc] initWithInt:[[sender stringValue] intValue]] autorelease];
  [_adminAction setSysctlInt:@"keyremap4macbook.repeat.wait" value:new];
}

@end
