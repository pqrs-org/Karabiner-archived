// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#include "pqrs/xml_compiler_bindings_clang.h"

@interface XMLCompiler : NSObject
{
  pqrs_xml_compiler* pqrs_xml_compiler_;
  BOOL initialized_;

  NSMutableArray* preferencepane_checkbox_;
  NSMutableArray* preferencepane_number_;
}

+ (XMLCompiler*) getInstance;
+ (NSString*) get_private_xml_path;

- (BOOL) initialized;
- (BOOL) reload;

- (size_t) remapclasses_initialize_vector_size;
- (const uint32_t*) remapclasses_initialize_vector_data;

- (uint32_t) keycode:(NSString*)name;
- (NSString*) identifier:(uint32_t)config_index;
- (uint32_t) appid:(NSString*)bundleIdentifier;

- (NSArray*) preferencepane_checkbox;
- (NSArray*) preferencepane_number;
- (NSString*) preferencepane_error_message;
- (NSString*) preferencepane_get_private_xml_path;

@end
