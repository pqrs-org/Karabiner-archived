// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>

@interface RemapClassesInitializeVector : NSObject {
  // We store the vector as uint32_t raw value for memory usage and performance.
  // (We don't use NSMutableArray.)
  uint32_t* data_;
  size_t capacity_;
  size_t size_;
  BOOL freezed_;
}

- (void) addVector:(NSArray*)vector;

- (void) setFreezed;
- (BOOL) freezed;
- (uint32_t*) rawValue;
- (size_t) size;

@end
