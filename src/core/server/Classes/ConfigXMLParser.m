#import "ConfigXMLParser.h"
#import "bridge.h"

@interface ConfigXMLParser (AutoGen)
- (BOOL) reload_autogen;
@end

@interface ConfigXMLParser (PreferencePane)
- (BOOL) reload_preferencepane;
@end

@implementation ConfigXMLParser

- (id) init
{
  [super init];
  [self reload];

  return self;
}

- (void) dealloc
{
  if (dict_initialize_vector_) {
    [dict_initialize_vector_ release];
  }
  if (dict_config_name_) {
    [dict_config_name_ release];
  }
  if (preferencepane_checkbox_) {
    [preferencepane_checkbox_ release];
  }
  if (preferencepane_number_) {
    [preferencepane_number_ release];
  }
  if (keycode_) {
    [keycode_ release];
  }

  [super dealloc];
}

// ------------------------------------------------------------
- (BOOL) initialized {
  return initialized_;
}

- (BOOL) reload {
  @synchronized(self) {
    initialized_ = NO;
    [self removeErrorMessage];

    if ([self reload_autogen] &&
        [self reload_preferencepane]) {
      initialized_ = YES;
    }
  }

  // We need to send a notification outside synchronized block to prevent lock.
  if (initialized_) {
    [[NSNotificationCenter defaultCenter] postNotificationName:@"ConfigXMLReloaded" object:nil];
  }

  return initialized_;
}

- (NSUInteger) count
{
  NSUInteger v = 0;
  @synchronized(self) {
    if (initialized_) {
      v = [dict_initialize_vector_ count];
    }
  }
  return v;
}

- (NSArray*) initialize_vector:(unsigned int)configindex
{
  NSArray* a = nil;
  @synchronized(self) {
    if (initialized_) {
      a = [dict_initialize_vector_ objectForKey:[NSNumber numberWithUnsignedInt:configindex]];
    }
  }
  return a;
}

- (NSUInteger) initialize_vector_size:(unsigned int)configindex
{
  NSUInteger v = 0;
  @synchronized(self) {
    NSArray* a = [self initialize_vector:configindex];
    if (a) {
      v = [a count];
    }
  }
  return v;
}

- (unsigned int) keycode:(NSString*)name
{
  unsigned int v = 0;
  @synchronized(self) {
    if (initialized_) {
      v = [keycode_ unsignedIntValue:name];
    }
  }
  return v;
}

- (NSString*) configname:(unsigned int)configindex
{
  NSString* name = nil;
  @synchronized(self) {
    if (initialized_) {
      name = [dict_config_name_ objectForKey:[NSNumber numberWithUnsignedInt:configindex]];
    }
  }
  return name;
}

- (NSArray*) preferencepane_checkbox
{
  NSArray* a = nil;
  @synchronized(self) {
    if (initialized_) {
      a = preferencepane_checkbox_;
    }
  }
  return a;
}

- (NSArray*) preferencepane_number
{
  NSArray* a = nil;
  @synchronized(self) {
    if (initialized_) {
      a = preferencepane_number_;
    }
  }
  return a;
}

@end
