// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "KeyCode.h"

@interface ConfigXMLParser : NSObject {
  unsigned int simultaneous_keycode_index_;
  NSMutableArray* array_initialize_vector_;
  NSMutableArray* array_config_name_;
  KeyCode* keycode_;
  BOOL initialized_;
}

- (NSUInteger) count;
- (NSUInteger) initialize_vector_size:(unsigned int)configindex;
- (NSArray*) initialize_vector:(unsigned int)configindex;
- (unsigned int) keycode:(NSString*)name;
- (NSString*) configname:(unsigned int)configindex;

@end
