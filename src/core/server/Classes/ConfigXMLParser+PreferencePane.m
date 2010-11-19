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

  NSArray* paths = [NSArray arrayWithObjects:
                    [self get_private_xml_path],
                    @"/Library/org.pqrs/KeyRemap4MacBook/prefpane/checkbox.xml",
                    nil];

  @try {
    for (NSString* xmlpath in paths) {
      if ([xmlpath length] == 0) continue;

      NSURL* url = [NSURL fileURLWithPath:xmlpath];
      NSError* error = nil;
      NSXMLDocument* xmldocument = [[[NSXMLDocument alloc] initWithContentsOfURL:url options:0 error:&error] autorelease];
      if (! xmldocument) {
        [self setErrorMessageFromNSError:error];
      }
    }

    retval = YES;

  } @catch (NSException* exception) {
    NSLog(@"[ERROR] KeyRemap4MacBook_server %@ %@", [exception name], [exception reason]);
  }

  return retval;
}

@end
