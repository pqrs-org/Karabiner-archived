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

- (IBAction) reloadXML:(id)sender
{
  [xmlCompiler_ reload];
}

@end
