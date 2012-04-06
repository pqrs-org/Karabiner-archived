#import "KeyRemap4MacBookKeys.h"
#import "KeyRemap4MacBookNSDistributedNotificationCenter.h"
#import "XMLCompiler.h"
#include "pqrs/xml_compiler_bindings_clang.h"

static XMLCompiler* global_instance = nil;

@implementation XMLCompiler

+ (XMLCompiler*) getInstance
{
  @synchronized(self) {
    if (! global_instance) {
      global_instance = [XMLCompiler new];
    }
  }
  return global_instance;
}

+ (NSString*) get_private_xml_path
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

    // copyItemAtPath does not change ctime and mtime of file.
    // (For example, mtime of destination file == mtime of source file.)
    // Therefore, we need to set mtime to current time.
    NSDictionary* attr = [NSDictionary dictionaryWithObjectsAndKeys:
                          [NSDate date], NSFileCreationDate,
                          [NSDate date], NSFileModificationDate,
                          nil];
    [filemanager setAttributes:attr ofItemAtPath:path error:NULL];
  }

  BOOL isDirectory;
  if ([filemanager fileExistsAtPath:path isDirectory:&isDirectory] && ! isDirectory) {
    return path;
  }

  return @"";
}

// ------------------------------------------------------------
- (id) init
{
  self = [super init];

  if (self) {
    pqrs_xml_compiler_initialize(&pqrs_xml_compiler_,
                                 [[[NSBundle mainBundle] resourcePath] UTF8String],
                                 [[[XMLCompiler get_private_xml_path] stringByDeletingLastPathComponent] UTF8String]);
    [self reload];
  }

  return self;
}

- (void) dealloc
{
  pqrs_xml_compiler_terminate(&pqrs_xml_compiler_);

  [super dealloc];
}

- (BOOL) initialized {
  return initialized_;
}

- (BOOL) reload {
  @synchronized(self) {
    initialized_ = NO;

    pqrs_xml_compiler_reload(pqrs_xml_compiler_);
    if (! pqrs_xml_compiler_get_error_count(pqrs_xml_compiler_)) {
      initialized_ = YES;
    }
  }

  // We need to send a notification outside synchronized block to prevent lock.
  if (initialized_) {
    [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter postNotificationName:kKeyRemap4MacBookConfigXMLReloadedNotification userInfo:nil];
  }

  return initialized_;
}

- (size_t) remapclasses_initialize_vector_size
{
  NSUInteger v = 0;
  @synchronized(self) {
    if (initialized_) {
      v = pqrs_xml_compiler_get_remapclasses_initialize_vector_size(pqrs_xml_compiler_);
    }
  }
  return v;
}

- (const uint32_t*) remapclasses_initialize_vector_data
{
  const uint32_t* v = nil;
  @synchronized(self) {
    if (initialized_) {
      v = pqrs_xml_compiler_get_remapclasses_initialize_vector_data(pqrs_xml_compiler_);
    }
  }
  return v;
}

- (uint32_t) keycode:(NSString*)name
{
  uint32_t v = 0;
  @synchronized(self) {
    if (initialized_) {
      v = pqrs_xml_compiler_get_symbol_map_value(pqrs_xml_compiler_, [name UTF8String]);
    }
  }
  return v;
}

- (NSString*) identifier:(uint32_t)config_index
{
  NSString* v = nil;
  @synchronized(self) {
    if (initialized_) {
      const char* p = pqrs_xml_compiler_get_identifier(pqrs_xml_compiler_, config_index);
      if (p) {
        v = [NSString stringWithUTF8String:p];
      }
    }
  }
  return v;
}

- (uint32_t) appid:(NSString*)bundleIdentifier
{
  uint32_t v = 0;
  @synchronized(self) {
    if (initialized_) {
      v = pqrs_xml_compiler_get_appid(pqrs_xml_compiler_, [bundleIdentifier UTF8String]);
    }
  }
  return v;
}

- (NSArray*) preferencepane_checkbox
{
  return nil;
}

- (NSArray*) preferencepane_number;
{
  return nil;
}

- (NSString*) preferencepane_error_message;
{
  return nil;
}

- (NSString*) preferencepane_get_private_xml_path;
{
  return nil;
}

@end
