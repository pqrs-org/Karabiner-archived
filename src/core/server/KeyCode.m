// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "KeyCode.h"

@implementation KeyCode

- (id) init
{
  [super init];

  dict_ = [[NSMutableDictionary alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"include.keycode" ofType:@"plist"]];
  return self;
}

- (void) dealloc
{
  if (dict_) {
    [dict_ release];
  }
  [super dealloc];
}

- (unsigned int) unsignedIntValue:(NSString*)name
{
  NSNumber* number = [dict_ objectForKey:name];
  if (number) {
    return [number unsignedIntValue];
  }

  NSLog(@"[ERROR] KeyRemap4MacBook_server unknown KeyCode name:%@", name);
  return 0;
}

@end
