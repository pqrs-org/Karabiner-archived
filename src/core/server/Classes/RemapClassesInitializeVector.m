#import "RemapClassesInitializeVector.h"
#include "bridge.h"

@implementation RemapClassesInitializeVector

enum {
  CAPACITY_UNIT_COUNT = 1024 * 32,
  INDEX_OF_FORMAT_VERSION = 0,
  INDEX_OF_COUNT = 1,
};

- (id) init
{
  self = [super init];

  if (self) {
    capacity_ = CAPACITY_UNIT_COUNT;
    data_ = (uint32_t*)(malloc(capacity_ * sizeof(uint32_t)));
    if (! data_) {
      @throw [NSException
            exceptionWithName: @"malloc is failed"
            reason:[NSString stringWithFormat:@"capacity_:%d", capacity_]
            userInfo: nil];
    }

    size_ = 0;
    freezed_ = NO;
    dict_configindex_ = [NSMutableDictionary new];
    max_configindex_ = 0;

    if (capacity_ <= INDEX_OF_COUNT) {
      @throw [NSException
            exceptionWithName: @"RemapClassesInitializeVector init error"
            reason: @"capacity_ is too small"
            userInfo: nil];
    }
    data_[INDEX_OF_FORMAT_VERSION] = BRIDGE_REMAPCLASS_INITIALIZE_VECTOR_FORMAT_VERSION;
    ++size_;
    data_[INDEX_OF_COUNT] = 0;
    ++size_;
  }

  return self;
}

- (void) dealloc
{
  if (data_) {
    free(data_);
    data_ = NULL;
  }

  [dict_configindex_ release];

  [super dealloc];
}

- (void) addVector:(NSArray*)vector configindex:(uint32_t)configindex
{
  // ----------------------------------------
  // realloc if needed
  NSUInteger newcount = [vector count] + 2; // +2 == size,configindex

  while (size_ + newcount > capacity_) {
    data_ = (uint32_t*)(realloc(data_, (capacity_ + CAPACITY_UNIT_COUNT) * sizeof(uint32_t)));
    if (! data_) {
      @throw [NSException
            exceptionWithName: @"realloc is failed"
            reason:[NSString stringWithFormat:@"capacity_:%d, newcount:%d", capacity_, newcount]
            userInfo: nil];
    }
    capacity_ += CAPACITY_UNIT_COUNT;
  }

  // ----------------------------------------
  // increase count
  ++(data_[INDEX_OF_COUNT]);

  // ----------------------------------------
  // append to data_
  uint32_t* p = data_ + size_;

  *p++ = (uint32_t)([vector count] + 1); // +1 == configindex
  *p++ = configindex;

  for (NSNumber* number in vector) {
    *p++ = [number unsignedIntValue];
  }

  size_ += newcount;

  // ----------------------------------------
  if (configindex > max_configindex_) {
    max_configindex_ = configindex;
  }
  [dict_configindex_ setObject:[NSNumber numberWithBool:YES] forKey:[NSNumber numberWithUnsignedInt:configindex]];
}

- (void) setFreezed
{
  // ----------------------------------------
  // fill configindex
  for (uint32_t i = 0; i <= max_configindex_; ++i) {
    if (! [dict_configindex_ objectForKey:[NSNumber numberWithUnsignedInt:i]]) {
      [self addVector:[[NSArray new] autorelease] configindex:i];
    }
  }

  // ----------------------------------------
  freezed_ = YES;
}

- (BOOL) freezed
{
  return freezed_;
}

- (uint32_t*) rawValue
{
  if (! freezed_) return NULL;
  return data_;
}

- (size_t) size
{
  if (! freezed_) {
    @throw [NSException
          exceptionWithName: @"[RemapClassesInitializeVector size] is failed"
          reason: @"[RemapClassesInitializeVector size] is called before freeze"
          userInfo: nil];
  }
  return size_;
}

- (uint32_t) countOfVectors
{
  if (! freezed_) {
    @throw [NSException
          exceptionWithName: @"[RemapClassesInitializeVector countOfVectors] is failed"
          reason: @"[RemapClassesInitializeVector countOfVectors] is called before freeze"
          userInfo: nil];
  }
  return data_[INDEX_OF_COUNT];
}

@end
