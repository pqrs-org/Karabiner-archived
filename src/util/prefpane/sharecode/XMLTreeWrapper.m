/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "XMLTreeWrapper.h"

@implementation BUNDLEPREFIX (XMLTreeWrapper)

- (id) init
{
  [super init];

  _XMLDocument = nil;
  return self;
}

- (BOOL) load:(NSString*)path
{
  NSURL* url = [NSURL fileURLWithPath:path];

  if (_XMLDocument) [_XMLDocument release];

  _XMLDocument = [[NSXMLDocument alloc] initWithContentsOfURL:url options:0 error:NULL];
  if (_XMLDocument == nil) return FALSE;
  return TRUE;
}

/* ---------------------------------------------------------------------- */
/* for NSOutlineView */
- (id) normalizeItem:(id)item
{
  if (! _XMLDocument) return nil;

  if (! item) {
    return [_XMLDocument rootElement];
  }
  return item;
}

- (NSUInteger) numberOfChildren:(id)item
{
  item = [self normalizeItem:item];
  if (! item) return 0;

  NSArray* nodes = [item nodesForXPath:@"list/item" error:NULL];
  return [nodes count];
}

- (id) getChild:(id)item index:(NSUInteger)idx
{
  item = [self normalizeItem:item];
  if (! item) return nil;

  NSArray* nodes = [item nodesForXPath:@"list/item" error:NULL];
  return [nodes objectAtIndex:idx];
}

- (BOOL) isItemExpandable:(id)item
{
  item = [self normalizeItem:item];
  if (! item) return FALSE;

  return [self numberOfChildren:item] > 0;
}

- (NSXMLNode*) getNode:(NSXMLNode*)node xpath:(NSString*)xpath
{
  NSArray* a = [node nodesForXPath:xpath error:NULL];
  if (a == nil) return nil;
  if ([a count] == 0) return nil;

  return [a objectAtIndex:0];
}

- (NSXMLElement*) getRoot
{
  return [_XMLDocument rootElement];
}

@end
