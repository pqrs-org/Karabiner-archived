// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "WorkSpaceData.h"

@implementation WorkSpaceData

- (unsigned int)getUIElementRole:(NSString *)name {
  if (!name) return 0;

  return [xmlCompiler_ uielementroleid:name];
}

@end
