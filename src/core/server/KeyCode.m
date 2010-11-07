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
  NSNumber* number = [self numberValue:name];
  return [number unsignedIntValue];
}

- (NSNumber*) numberValue:(NSString*)name
{
  NSNumber* number = [dict_ objectForKey:name];
  if (number) {
    return number;
  }

  NSLog(@"[ERROR] KeyRemap4MacBook_server unknown KeyCode name:%@", name);
  return [NSNumber numberWithUnsignedInt:0];
}

- (void) append:(NSString*)name newvalue:(unsigned int)newvalue
{
  [dict_ setObject:[NSNumber numberWithUnsignedInt:newvalue] forKey:name];
}

- (void) append:(NSString*)type name:(NSString*)name
{
  NSString* autoindexkey = [NSString stringWithFormat:@"%@::VK__AUTOINDEX__BEGIN__", type];
  unsigned int newvalue = [self unsignedIntValue:autoindexkey];
  [dict_ setObject:[NSNumber numberWithUnsignedInt:(newvalue+1)] forKey:autoindexkey];

  NSString* newname = [NSString stringWithFormat:@"%@::%@", type, name];
  [self append:newname newvalue:newvalue];
}

+ (NSString*) normalizeName:(NSString*)name
{
  NSString* trimed = [name stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
  NSString* replaced = [trimed stringByReplacingOccurrencesOfString:@"." withString:@"_"];
  return replaced;
}

@end
