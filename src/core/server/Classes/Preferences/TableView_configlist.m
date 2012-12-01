// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "PreferencesManager.h"
#import "TableView_configlist.h"

@implementation TableView_configlist

// ======================================================================
- (NSInteger) numberOfRowsInTableView:(NSTableView*)aTableView
{
  return (NSInteger)([preferencesManager_ configlist_count]);
}

- (id) tableView:(NSTableView*)aTableView objectValueForTableColumn:(NSTableColumn*)aTableColumn row:(NSInteger)rowIndex
{
  return [preferencesManager_ configlist_name:rowIndex];
}

- (void) tableView:(NSTableView*)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn*)aTableColumn row:(NSInteger)rowIndex
{
  [preferencesManager_ configlist_setName:rowIndex name:anObject];
  [view_ reloadData];
}

// ======================================================================
- (IBAction) add:(id)sender
{
  [preferencesManager_ configlist_append];
  [view_ reloadData];
}

- (IBAction) delete:(id)sender
{
  NSInteger idx = [view_ selectedRow];
  if (idx == -1) return;

  [preferencesManager_ configlist_delete:idx];
  [view_ reloadData];
}

@end
