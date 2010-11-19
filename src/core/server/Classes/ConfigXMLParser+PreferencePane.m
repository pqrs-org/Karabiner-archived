#import "ConfigXMLParser.h"
#import "bridge.h"

@implementation ConfigXMLParser (PreferencePane)

- (NSMutableArray*) traverse_item:(NSXMLElement*)element
{
  NSMutableArray* array = [[NSMutableArray new] autorelease];

  for (NSXMLElement* element_item in [element elementsForName : @"item"]) {
    NSMutableDictionary* dict = [[NSMutableDictionary new] autorelease];

    // ----------------------------------------
    NSString* title = @"";
    NSUInteger height = 0;
    for (NSXMLElement* element_name in [element_item elementsForName : @"name"]) {
      title = [title stringByAppendingString:[NSString stringWithFormat:@"%@\n", [element_name stringValue]]];
      ++height;
    }
    for (NSXMLElement* element_appendix in [element_item elementsForName : @"appendix"]) {
      title = [title stringByAppendingString:[NSString stringWithFormat:@"  %@\n", [element_appendix stringValue]]];
      ++height;
    }
    title = [title stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    [dict setObject:title forKey:@"title"];
    [dict setObject:[NSNumber numberWithUnsignedInteger:height] forKey:@"height"];

    // ----------------------------------------
    NSString* identifier = @"";
    NSArray* elements_identifier = [element_item elementsForName:@"identifier"];
    if ([elements_identifier count] == 1) {
      identifier = [[elements_identifier objectAtIndex:0] stringValue];
    } else if ([elements_identifier count] >= 2) {
      @throw [NSException exceptionWithName : @"<item> is invalid" reason :[NSString stringWithFormat:@"multiple <identifier> in one <item>.\n%@", [element_item stringValue]] userInfo : nil];
    }
    [dict setObject:identifier forKey:@"identifier"];

    // ----------------------------------------
    [dict setObject:[self traverse_item:element_item] forKey:@"children"];

    // ----------------------------------------
    [array addObject:dict];
  }

  for (NSXMLElement* e in [element elementsForName : @"list"]) {
    [array addObject:[self traverse_item:e]];
  }

  return array;
}

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

      NSMutableArray* list = [self traverse_item:[xmldocument rootElement]];
      NSLog(@"%@", list);

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
