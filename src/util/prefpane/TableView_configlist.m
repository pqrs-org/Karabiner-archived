// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "TableView_configlist.h"
#import "ConfigControl.h"

@implementation TableView_configlist

- (id) init
{
  self = [super init];
  if (! self) return self;

  cache_ = nil;
  return self;
}

- (void) fillcache
{
  if (cache_) return;

  cache_ = [[NSMutableArray alloc] init];
  NSArray* list = [ConfigControl getConfigList];
  for (id name in list) {
    if ([name length] == 0) continue;
    NSString* title = [name substringFromIndex:1];
    [cache_ addObject:title];
  }
}

- (void) refresh
{
  cache_ = nil;
  [self fillcache];
  [view_ reloadData];
}

// ======================================================================
- (NSInteger) numberOfRowsInTableView:(NSTableView*)aTableView
{
  [self fillcache];
  return [cache_ count];
}

- (id) tableView:(NSTableView*)aTableView objectValueForTableColumn:(NSTableColumn*)aTableColumn row:(int)rowIndex
{
  [self fillcache];
  return [cache_ objectAtIndex:rowIndex];
}

- (void) tableView:(NSTableView*)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn*)aTableColumn row:(int)rowIndex
{
  [ConfigControl setName:rowIndex newName:anObject];
  [self refresh];
}

// ======================================================================
- (IBAction) add:(id)sender
{
  [ConfigControl add];
  [self refresh];
}

- (IBAction) delete:(id)sender
{
  NSInteger idx = [view_ selectedRow];
  if (idx == -1) return;

  [ConfigControl delete:idx];
  [self refresh];
}

@end
