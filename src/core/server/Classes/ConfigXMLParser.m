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
  self = [super init];

  if (self) {
    [self reload];
  }

  return self;
}

- (void) dealloc
{
  [dict_initialize_vector_ release];
  [dict_config_name_ release];
  [preferencepane_checkbox_ release];
  [preferencepane_number_ release];
  [keycode_ release];
  [error_message_ release];

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

- (NSString*) preferencepane_error_message
{
  return error_message_;
}

- (NSString*) preferencepane_get_private_xml_path
{
  NSFileManager* filemanager = [NSFileManager defaultManager];
  NSArray* paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
  NSString* path = [paths objectAtIndex:0];
  path = [path stringByAppendingPathComponent:@"KeyRemap4MacBook"];
  if (! [filemanager fileExistsAtPath:path]) {
    [filemanager createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:NULL];
  }

  path = [path stringByAppendingPathComponent:@"private.xml"];
  if (! [filemanager fileExistsAtPath:path]) {
    [filemanager copyItemAtPath:[[NSBundle mainBundle] pathForResource:@"private" ofType:@"xml"] toPath:path error:NULL];
  }

  BOOL isDirectory;
  if ([filemanager fileExistsAtPath:path isDirectory:&isDirectory] && ! isDirectory) {
    return path;
  }

  return @"";
}

@end
