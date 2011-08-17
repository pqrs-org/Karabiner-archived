/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "OutlineView_checkbox.h"

@implementation org_pqrs_KeyRemap4MacBook_OutlineView_checkbox

- (id) init
{
  self = [super init];

  if (self) {
    ischeckbox_ = YES;
  }

  return self;
}

- (void) distributedObserver_configXMLReloaded:(NSNotification*)notification
{
  // [NSAutoreleasePool drain] is never called from NSDistributedNotificationCenter.
  // Therefore, we need to make own NSAutoreleasePool.
  NSAutoreleasePool* pool = [NSAutoreleasePool new];
  {
    [super distributedObserver_configXMLReloaded:notification];

    [self filter:nil];
  }
  [pool drain];
}

- (IBAction) reloadXML:(id)sender
{
  [[client_ proxy] configxml_reload];
}

- (IBAction) filter:(id)sender
{
  BOOL isEnabledOnly = ([showEnabledOnly_ state] == NSOnState);
  [self filterDataSource:isEnabledOnly string:[searchText_ stringValue]];
}

@end
