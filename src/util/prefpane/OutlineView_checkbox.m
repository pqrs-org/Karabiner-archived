/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "OutlineView_checkbox.h"

@implementation org_pqrs_KeyRemap4MacBook_OutlineView_checkbox

- (id) init
{
  [super init];

  ischeckbox_ = YES;

  return self;
}

- (IBAction) reloadXML:(id)sender
{
  // reload xml on server process.
  [[preferencesclient_ proxy] configxml_reload];

  // wait until xml reloaded.
  int trycount = 0;
  for (trycount = 0; trycount < 10; ++trycount) {
    [NSThread sleepForTimeInterval:0.5];
    if ([[preferencesclient_ proxy] configxml_initialized]) break;
  }

  // ----------------------------------------
  // update prefpane
  [self load:YES];

  // ----------------------------------------
  [outlineview_ reloadData];
}

/* ------------------------------------------------------------ */
#if 0
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
#endif

- (IBAction) filter:(id)sender
{
  [self filterByString:[searchText_ stringValue]];
}

@end
