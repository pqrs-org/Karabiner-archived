#import "AppDelegate.h"
#include "pqrs/xml_compiler_bindings_clang.h"

@implementation AppDelegate

@synthesize window;

static int total_identifier_count_ = 0;

- (NSString*) escapeHTML:(NSString*)string
{
  string = [string stringByReplacingOccurrencesOfString:@"&"  withString:@"&amp;"];
  string = [string stringByReplacingOccurrencesOfString:@"<"  withString:@"&lt;"];
  string = [string stringByReplacingOccurrencesOfString:@">"  withString:@"&gt;"];
  return string;
}

- (void) dump_tree:(const pqrs_xml_compiler_preferences_checkbox_node_tree*)node_tree
{
  if (node_tree) {
    size_t size = pqrs_xml_compiler_get_preferences_checkbox_node_tree_children_count(node_tree);
    if (size > 0) {
      printf("<ul>\n");
      for (size_t i = 0; i < size; ++i) {
        const pqrs_xml_compiler_preferences_checkbox_node_tree* child =
          pqrs_xml_compiler_get_preferences_checkbox_node_tree_child(node_tree, i);
        NSString* name = [NSString stringWithUTF8String:pqrs_xml_compiler_get_preferences_checkbox_node_tree_name(child)];
        printf("<li>%s</li>\n", [[self escapeHTML:name] UTF8String]);

        [self dump_tree:child];

        if (pqrs_xml_compiler_get_preferences_checkbox_node_tree_identifier(child)) {
          printf("<identifier>%s</identifier>\n", pqrs_xml_compiler_get_preferences_checkbox_node_tree_identifier(child));
          ++total_identifier_count_;
        }
      }
      printf("</ul>\n");
    }
  }
}

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification
{
  pqrs_xml_compiler* pqrs_xml_compiler = NULL;
  int error = pqrs_xml_compiler_initialize(&pqrs_xml_compiler,
                                           "/Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook.app/Contents/Resources",
                                           "/noexistspath");
  if (error) {
    NSLog(@"Error %d", error);
    exit(1);
  }
  pqrs_xml_compiler_reload(pqrs_xml_compiler);

  NSArray* arguments = [[NSProcessInfo processInfo] arguments];
  if ([arguments count] == 1 ||
      [[arguments objectAtIndex:1] isEqualToString:@"dump_data"]) {
    size_t size = pqrs_xml_compiler_get_remapclasses_initialize_vector_size(pqrs_xml_compiler);
    const uint32_t* v = pqrs_xml_compiler_get_remapclasses_initialize_vector_data(pqrs_xml_compiler);
    for (size_t i = 0; i < size; ++i) {
      printf("%d\n", v[i]);
    }

  } else if ([[arguments objectAtIndex:1] isEqualToString:@"dump_tree"]) {
    const pqrs_xml_compiler_preferences_checkbox_node_tree* node_tree =
      pqrs_xml_compiler_get_preferences_checkbox_node_tree_root(pqrs_xml_compiler);
    [self dump_tree:node_tree];

    printf("Total items: %d\n", total_identifier_count_);
  }

  fflush(stdout);
  pqrs_xml_compiler_terminate(&pqrs_xml_compiler);

  [[NSApplication sharedApplication] terminate:nil];
}

@end
