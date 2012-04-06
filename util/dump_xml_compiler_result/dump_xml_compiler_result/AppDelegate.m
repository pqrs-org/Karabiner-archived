#import "AppDelegate.h"
#include "pqrs/xml_compiler_bindings_clang.h"

@implementation AppDelegate

@synthesize window;

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification
{
  NSArray* paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
  NSString* path = [paths objectAtIndex:0];
  path = [path stringByAppendingPathComponent:@"KeyRemap4MacBook"];

  pqrs_xml_compiler* p = NULL;
  int error = pqrs_xml_compiler_initialize(&p,
                                           "/Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook.app/Contents/Resources",
                                           [path UTF8String]);
  if (error) {
    NSLog(@"Error %d", error);
    exit(1);
  }
  pqrs_xml_compiler_reload(p);

  size_t size = pqrs_xml_compiler_get_remapclasses_initialize_vector_size(p);
  const uint32_t* v = pqrs_xml_compiler_get_remapclasses_initialize_vector_data(p);
  for (size_t i = 0; i < size; ++i) {
    printf("%d\n", v[i]);
  }
  fflush(stdout);

  pqrs_xml_compiler_terminate(&p);

  [[NSApplication sharedApplication] terminate:nil];
}

@end
