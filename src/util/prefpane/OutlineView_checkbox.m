/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "OutlineView_checkbox.h"
#import "XMLTreeWrapper.h"

@implementation org_pqrs_KeyRemap4MacBook_OutlineView_checkbox

static BUNDLEPREFIX(XMLTreeWrapper) * _xmlTreeWrapper;
static NSString* xmlpath = @"/Library/org.pqrs/KeyRemap4MacBook/prefpane/checkbox.xml";

- (id) init
{
  self = [super init];
  if (! self) return self;

  _xmlTreeWrapper = [[BUNDLEPREFIX (XMLTreeWrapper) alloc] init];
  if (_xmlTreeWrapper == nil) return nil;
  if (! [_xmlTreeWrapper load:xmlpath]) return nil;
  return self;
}

- (void) dealloc
{
  if (_xmlTreeWrapper) {
    [_xmlTreeWrapper release];
  }
  [super dealloc];
}

- (IBAction) reloadXML:(id)sender
{
  // ----------------------------------------
  // restart server process
  for (NSRunningApplication* application in [NSRunningApplication runningApplicationsWithBundleIdentifier:@"org.pqrs.KeyRemap4MacBook"]) {
    [application terminate];

    // wait until server is terminated.
    int trycount = 0;
    for (trycount = 0; trycount < 10; ++trycount) {
      [NSThread sleepForTimeInterval:0.5];
      if (application.terminated) break;
    }

    // server will be restarted by launchd.
  }

  // ----------------------------------------
  // update prefpane
  if (_xmlTreeWrapper) {
    [_xmlTreeWrapper release];
  }
  _xmlTreeWrapper = [[BUNDLEPREFIX (XMLTreeWrapper) alloc] init];
  if (_xmlTreeWrapper) {
    [_xmlTreeWrapper load:xmlpath];
  }

  // wait until server will be alive.
  int trycount = 0;
  for (trycount = 0; trycount < 10; ++trycount) {
    if ([preferencesclient_ proxy]) break;
    [NSThread sleepForTimeInterval:0.5];
  }

  // ----------------------------------------
  [_outlineView_checkbox reloadData];
}

/* ------------------------------------------------------------ */
- (BOOL) filter_identifier:(NSXMLNode*)node
{
  NSXMLNode* identifier = [_xmlTreeWrapper getNode:node xpath:@"identifier"];

  if (identifier) {
    int value = [[preferencesclient_ proxy] value:[identifier stringValue]];
    if (value) return TRUE;
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

- (BOOL) filter_checkChildren:(NSXMLNode*)node identifier:(BOOL)identifier search:(NSString*)search
{
  NSArray* a = [node nodesForXPath:@"list/item" error:NULL];
  if (a == nil) return FALSE;

  for (NSXMLNode* n in a) {
    if (! n) break;

    if ([self filter_checkChildren:n identifier:identifier search:search]) return TRUE;
    if (identifier) {
      if ([self filter_identifier:n]) return TRUE;
    }
    if (search != nil) {
      if ([self filter_search:n search:search]) return TRUE;
    }
  }

  return FALSE;
}

- (void) filter_core:(NSXMLElement*)node identifier:(BOOL)identifier search:(NSString*)search
{
  NSArray* a = [node nodesForXPath:@"list/item" error:NULL];
  if (a == nil) return;

  for (NSXMLElement* n in a) {
    if (! n) break;

    if (identifier) {
      if ([self filter_identifier:n] || [self filter_checkChildren:n identifier:identifier search:search]) {
        [self filter_core:n identifier:identifier search:search];
      } else {
        [n detach];
      }
    } else if (search) {
      if ([self filter_search:n search:search]) continue;
      if ([self filter_checkChildren:n identifier:identifier search:search]) {
        [self filter_core:n identifier:identifier search:search];
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
    [self filter_core:[_xmlTreeWrapper getRoot] identifier:TRUE search:nil];
    expand = TRUE;
  }
  if (! [[_searchText stringValue] isEqual:@""]) {
    NSArray* a = [[_searchText stringValue] componentsSeparatedByString:@" "];
    for (NSString* str in a) {
      if (! str) break;

      str = [str lowercaseString];
      if (! [str isEqual:@""]) {
        [self filter_core:[_xmlTreeWrapper getRoot] identifier:FALSE search:str];
      }
    }
    expand = TRUE;
  }

  [_outlineView_checkbox reloadData];
  if (expand) {
    [_outlineView_checkbox expandItem:nil expandChildren:YES];
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
  // setTitle
  NSXMLNode* attr_title = [item attributeForName:@"cache_title"];
  if (attr_title != nil) {
    [cell setTitle:[attr_title stringValue]];

  } else {
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

    [item addAttribute:[NSXMLNode attributeWithName:@"cache_title" stringValue:titlestring]];

    [cell setTitle:titlestring];
  }

  // ------------------------------------------------------------
  // check identifier, then setImagePosition or return value.
  NSXMLNode* attr_identifier = [item attributeForName:@"cache_identifier"];
  NSString* identifier = nil;
  if (attr_identifier != nil) {
    identifier = [attr_identifier stringValue];

  } else {
    NSXMLNode* node_identifier = [_xmlTreeWrapper getNode:item xpath:@"identifier"];
    if (node_identifier) {
      identifier = [node_identifier stringValue];
    }
  }

  if (! identifier || [identifier hasPrefix:@"notsave."]) {
    [cell setImagePosition:NSNoImage];
    return nil;

  } else {
    [cell setImagePosition:NSImageLeft];
    return [NSNumber numberWithInt:[[preferencesclient_ proxy] value:identifier]];
  }

  return nil;
}

- (CGFloat) outlineView:(NSOutlineView*)outlineView heightOfRowByItem:(id)item
{
  NSXMLNode* attr = [item attributeForName:@"cache_height"];
  if (attr != nil) {
    return [[attr stringValue] floatValue];
  }

  NSUInteger appendixnum = 0;
  NSArray* a = [item nodesForXPath:@"appendix" error:NULL];
  if (a) {
    appendixnum = [a count];
  }

  CGFloat height = [outlineView rowHeight] * (1 + appendixnum);
  NSString* heightstring = [NSString stringWithFormat:@"%f", height];
  [item addAttribute:[NSXMLNode attributeWithName:@"cache_height" stringValue:heightstring]];

  return height;
}

- (void) outlineView:(NSOutlineView*)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn byItem:(id)item
{
  NSXMLNode* identifier = [_xmlTreeWrapper getNode:item xpath:@"identifier"];
  if (identifier) {
    NSString* name = [identifier stringValue];
    if (! [name hasPrefix:@"notsave."]) {
      int value = [[preferencesclient_ proxy] value:name];
      value = ! value;
      [[preferencesclient_ proxy] setValueForName:value forName:name];
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
