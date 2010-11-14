// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "TableView_configlist.h"

@implementation TableView_configlist

// ======================================================================
- (NSInteger) numberOfRowsInTableView:(NSTableView*)aTableView
{
  return (NSInteger)([[preferencesclient_ proxy] configlist_count]);
}

- (id) tableView:(NSTableView*)aTableView objectValueForTableColumn:(NSTableColumn*)aTableColumn row:(NSInteger)rowIndex
{
  return [[preferencesclient_ proxy] configlist_name:rowIndex];
}

- (void) tableView:(NSTableView*)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn*)aTableColumn row:(NSInteger)rowIndex
{
  [[preferencesclient_ proxy] configlist_setName:rowIndex name:anObject];
  [view_ reloadData];
}

// ======================================================================
- (IBAction) add:(id)sender
{
  [[preferencesclient_ proxy] configlist_append];
  [view_ reloadData];
}

- (IBAction) delete:(id)sender
{
  NSInteger idx = [view_ selectedRow];
  if (idx == -1) return;

  [[preferencesclient_ proxy] configlist_delete:idx];
  [view_ reloadData];
}

@end
