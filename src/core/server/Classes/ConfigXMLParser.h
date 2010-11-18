// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "KeyCode.h"

@interface ConfigXMLParser : NSObject {
  unsigned int simultaneous_keycode_index_;
  NSMutableDictionary* dict_initialize_vector_;
  NSMutableDictionary* dict_config_name_;
  KeyCode* keycode_;
  BOOL initialized_;
}

- (BOOL) initialized;
- (void) reload;
- (NSUInteger) count;
- (NSUInteger) initialize_vector_size:(unsigned int)configindex;
- (NSArray*) initialize_vector:(unsigned int)configindex;
- (unsigned int) keycode:(NSString*)name;
- (NSString*) configname:(unsigned int)configindex;

@end
