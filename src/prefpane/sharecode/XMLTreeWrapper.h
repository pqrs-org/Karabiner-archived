// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@interface BUNDLEPREFIX_XMLTreeWrapper : NSObject
{
  NSXMLDocument *_XMLDocument;
}

- (BOOL) load:(NSString *)path;

- (id) normalizeItem:(id)item;
- (int) numberOfChildren:(id)item;
- (id) getChild:(id)item index:(int)index;
- (BOOL) isItemExpandable:(id)item;
- (NSXMLNode *) getNode:(NSXMLNode *)node xpath:(NSString *)xpath;

@end
