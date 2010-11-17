/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "OutlineView_number.h"
#import "XMLTreeWrapper.h"

@implementation org_pqrs_KeyRemap4MacBook_OutlineView_number

static BUNDLEPREFIX(XMLTreeWrapper) * _xmlTreeWrapper;

- (id) init
{
  self = [super init];
  if (self) {
    _xmlTreeWrapper = [[BUNDLEPREFIX (XMLTreeWrapper) alloc] init];
    if (_xmlTreeWrapper == nil) return nil;
    if (! [_xmlTreeWrapper load:@"/Library/org.pqrs/KeyRemap4MacBook/prefpane/number.xml"]) return nil;
  }
  return self;
}

/* ------------------------------------------------------------ */
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
  id identifier = [tableColumn identifier];

  if ([identifier isEqualToString:@"name"] ||
      [identifier isEqualToString:@"baseunit"] ||
      [identifier isEqualToString:@"default"]) {
    NSXMLNode* node = [_xmlTreeWrapper getNode:item xpath:identifier];
    if (! node) return nil;

    return [node stringValue];

  } else if ([identifier isEqualToString:@"value"]) {
    NSXMLNode* identifier = [_xmlTreeWrapper getNode:item xpath:@"identifier"];
    if (! identifier) return nil;

    return [NSNumber numberWithInt:[[preferencesclient_ proxy] value:[identifier stringValue]]];
  }

  return nil;
}

- (BOOL) outlineView:(NSOutlineView*)outlineView shouldCollapseItem:(id)item
{
  return false;
}

- (void) outlineView:(NSOutlineView*)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn byItem:(id)item
{
  NSXMLNode* identifier = [_xmlTreeWrapper getNode:item xpath:@"identifier"];
  if (identifier) {
    NSString* name = [identifier stringValue];
    [[preferencesclient_ proxy] setValueForName:[object intValue] forName:name];
  }
}

@end
