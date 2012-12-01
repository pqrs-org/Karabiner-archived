/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "OutlineView_checkbox.h"
#import "XMLCompiler.h"

@implementation OutlineView_checkbox

- (id) init
{
  self = [super init];

  if (self) {
    ischeckbox_ = YES;
  }

  return self;
}

- (void) observer_ConfigXMLReloaded:(NSNotification*)notification
{
  [super observer_ConfigXMLReloaded:notification];
  [self filter:nil];
}

- (IBAction) reloadXML:(id)sender
{
  [xmlCompiler_ reload];
}

- (IBAction) filter:(id)sender
{
  BOOL isEnabledOnly = ([showEnabledOnly_ state] == NSOnState);
  [self filterDataSource:isEnabledOnly string:[searchText_ stringValue]];
}

@end
