// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>

@interface RemapClassesInitializeVector : NSObject {
  // We store the vector as uint32_t raw value for memory usage and performance.
  // (We don't use NSMutableArray.)
  uint32_t* data_;

  // capacity_ and size_ are count of items. (== not allocated memory size.)
  size_t capacity_;
  size_t size_;
  BOOL freezed_;

  // hash of "configindex => BOOL"
  NSMutableDictionary* dict_configindex_;
  uint32_t max_configindex_;
}

- (void) addVector:(NSArray*)vector configindex:(uint32_t)configindex;

- (void) setFreezed;
- (BOOL) freezed;
- (uint32_t*) rawValue;
- (size_t) size;
- (uint32_t) countOfVectors;

@end
