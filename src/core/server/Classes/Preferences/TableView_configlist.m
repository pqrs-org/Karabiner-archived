// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "NotificationKeys.h"
#import "PreferencesManager.h"
#import "TableView_configlist.h"

@implementation TableView_configlist

- (void)observer_ConfigListChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [view_ setNeedsDisplay:YES];
  });
}

- (id)init {
  self = [super init];

  if (self) {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_ConfigListChanged:)
                                                 name:kConfigListChangedNotification
                                               object:nil];
  }

  return self;
}

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

// ======================================================================
- (NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView {
  return (NSInteger)([preferencesManager_ configlist_count]);
}

- (id)tableView:(NSTableView*)aTableView objectValueForTableColumn:(NSTableColumn*)aTableColumn row:(NSInteger)rowIndex {
  return [preferencesManager_ configlist_name:rowIndex];
}

- (void)tableView:(NSTableView*)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn*)aTableColumn row:(NSInteger)rowIndex {
  [preferencesManager_ configlist_setName:rowIndex name:anObject];
  [view_ reloadData];
}

// ======================================================================
- (IBAction)add:(id)sender {
  [preferencesManager_ configlist_append];
  [view_ reloadData];
}

- (IBAction) delete:(id)sender {
  NSInteger idx = [view_ selectedRow];
  if (idx == -1) return;

  if (idx == [preferencesManager_ configlist_selectedIndex]) {
    NSAlert* alert = [NSAlert new];
    [alert setMessageText:@"Karabiner Alert"];
    [alert addButtonWithTitle:@"Close"];
    [alert setInformativeText:@"You cannot delete an active profile.\n"];
    [alert runModal];
    return;
  }

  [preferencesManager_ configlist_delete:idx];
  [view_ reloadData];
}

- (IBAction)sortByName:(id)sender {
  [preferencesManager_ configlist_sortByName];
  [view_ reloadData];
}

- (IBAction)sortByCreated:(id)sender {
  [preferencesManager_ configlist_sortByAppendIndex];
  [view_ reloadData];
}

@end
