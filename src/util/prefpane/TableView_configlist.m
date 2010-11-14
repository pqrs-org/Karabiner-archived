// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "TableView_configlist.h"
#import "ConfigControl.h"

@implementation TableView_configlist

// ======================================================================
- (NSInteger) numberOfRowsInTableView:(NSTableView*)aTableView
{
  return (NSInteger)([[preferencesclient_ proxy] count]);
}

- (id) tableView:(NSTableView*)aTableView objectValueForTableColumn:(NSTableColumn*)aTableColumn row:(NSInteger)rowIndex
{
  return [[preferencesclient_ proxy] name:rowIndex];
}

- (void) tableView:(NSTableView*)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn*)aTableColumn row:(NSInteger)rowIndex
{
  [[preferencesclient_ proxy] setName:rowIndex name:anObject];
  [view_ reloadData];
}

// ======================================================================
- (IBAction) add:(id)sender
{
  [ConfigControl add];
  [view_ reloadData];
}

- (IBAction) delete:(id)sender
{
  NSInteger idx = [view_ selectedRow];
  if (idx == -1) return;

  [ConfigControl delete:idx];
  [view_ reloadData];
}

@end
