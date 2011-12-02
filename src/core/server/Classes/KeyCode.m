// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "KeyCode.h"

@implementation KeyCode

- (id) init
{
  self = [super init];

  if (self) {
    dict_ = [[NSMutableDictionary alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"include.keycode" ofType:@"plist"]];
    [dict_ setObject:[NSNumber numberWithUnsignedInt:0] forKey:@"ConfigIndex::VK__AUTOINDEX__BEGIN__"];
  }

  return self;
}

- (void) dealloc
{
  [dict_ release];

  [super dealloc];
}

- (NSDictionary*) dictionary
{
  return dict_;
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

  @throw [NSException exceptionWithName :[NSString stringWithFormat:@"Unknown data: %@", name] reason : @"" userInfo : nil];
}

- (BOOL) isExists:(NSString*)name
{
  if ([dict_ objectForKey:name]) {
    return YES;
  } else {
    return NO;
  }
}

- (void) append:(NSString*)name newvalue:(unsigned int)newvalue
{
  if ([dict_ objectForKey:name]) {
    @throw [NSException exceptionWithName :[NSString stringWithFormat:@"Repetition addition: %@", name] reason : @"" userInfo : nil];
  }
  [dict_ setObject:[NSNumber numberWithUnsignedInt:newvalue] forKey:name];
}

- (void) append:(NSString*)type name:(NSString*)name
{
  NSString* autoindexkey = [NSString stringWithFormat:@"%@::VK__AUTOINDEX__BEGIN__", type];
  unsigned int newvalue = [self unsignedIntValue:autoindexkey];
  [dict_ setObject:[NSNumber numberWithUnsignedInt:(newvalue + 1)] forKey:autoindexkey];

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
