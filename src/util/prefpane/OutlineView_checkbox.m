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

  // ----------------------------------------
  [self filter:nil];
}

- (IBAction) filter:(id)sender
{
  BOOL isEnabledOnly = ([showEnabledOnly_ state] == NSOnState);
  [self filterDataSource:isEnabledOnly string:[searchText_ stringValue]];
}

@end
