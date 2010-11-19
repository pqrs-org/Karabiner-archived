#import "ConfigXMLParser.h"
#import "bridge.h"

@implementation ConfigXMLParser (PreferencePane)

- (BOOL) reload_preferencepane {
  BOOL retval = NO;

  if (preferencepane_checkbox_) {
    [preferencepane_checkbox_ release];
  }
  preferencepane_checkbox_ = [[NSMutableArray alloc] initWithCapacity:0];

  if (preferencepane_number_) {
    [preferencepane_number_ release];
  }
  preferencepane_number_ = [[NSMutableArray alloc] initWithCapacity:0];

#if 0
  NSArray* paths = [NSArray arrayWithObjects:
                    [self get_private_xml_path],
                    @"/Library/org.pqrs/KeyRemap4MacBook/prefpane/checkbox.xml",
                    nil];
#endif

  retval = YES;

  return retval;
}

@end
