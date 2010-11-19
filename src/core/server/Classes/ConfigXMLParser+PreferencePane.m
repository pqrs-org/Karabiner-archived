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

  for (NSString* xmlpath in paths) {
    @try {
      if ([xmlpath length] == 0) continue;

      NSURL* url = [NSURL fileURLWithPath:xmlpath];
      NSError* error = nil;
      NSXMLDocument* xmldocument = [[[NSXMLDocument alloc] initWithContentsOfURL:url options:0 error:&error] autorelease];
      if (! xmldocument) {
        @throw [NSException exceptionWithName :[NSString stringWithFormat:@"%@ is invalid", xmlpath] reason :[error localizedDescription] userInfo : nil];
      }

      // Set retval to YES if only one XML file is loaded successfully.
      // Unless we do it, all setting becomes disabled by one error.
      // (== If private.xml is invalid, system wide checkbox.xml is not loaded in kext.)
      retval = YES;

    } @catch (NSException* exception) {
      [self setErrorMessage:exception];
    }
  }

  return retval;
}

@end
