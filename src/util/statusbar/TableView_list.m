// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "TableView_list.h"
#import "ConfigList.h"

@implementation TableView_list

- (int) numberOfRowsInTableView:(NSTableView*)aTableView
{
  return [ConfigList getSize];
}

- (id) tableView:(NSTableView*)aTableView objectValueForTableColumn:(NSTableColumn*)aTableColumn row:(int)rowIndex
{
  return [ConfigList getName:rowIndex];
}

- (void) tableView:(NSTableView*)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn*)aTableColumn row:(int)rowIndex
{
  [ConfigList setName:rowIndex newName:anObject];
  [_tableView reloadData];
}

- (IBAction) add:(id)sender
{
  [ConfigList add];
  [_tableView reloadData];
}

- (IBAction) delete:(id)sender
{
  NSInteger idx = [_tableView selectedRow];
  if (idx == -1) return;

  [ConfigList delete:idx];
  [_tableView reloadData];
}

- (IBAction) refresh:(id)sender
{
  [ConfigList refresh];
  [_tableView reloadData];
}

@end
