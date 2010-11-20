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
    NSArray* elements_identifier = [element_item elementsForName:@"identifier"];
    if ([elements_identifier count] == 1) {
      NSXMLElement* element_identifier = [elements_identifier objectAtIndex:0];
      [dict setObject:[element_identifier stringValue] forKey:@"identifier"];

      NSXMLNode* attr_default = [element_identifier attributeForName:@"default"];
      if (attr_default) {
        [dict setObject:[NSNumber numberWithInt:[[attr_default stringValue] intValue]] forKey:@"default"];
      }

      NSXMLNode* attr_baseunit = [element_identifier attributeForName:@"baseunit"];
      if (attr_baseunit) {
        [dict setObject:[attr_baseunit stringValue] forKey:@"baseunit"];
      }

    } else if ([elements_identifier count] >= 2) {
      @throw [NSException exceptionWithName : @"<item> is invalid" reason :[NSString stringWithFormat:@"multiple <identifier> in one <item>.\n%@", [element_item stringValue]] userInfo : nil];
    }

    // ----------------------------------------
    NSArray* a = [self traverse_item:element_item];
    if ([a count] > 0) {
      [dict setObject:a forKey:@"children"];
    }

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
  preferencepane_checkbox_ = [NSMutableArray new];

  if (preferencepane_number_) {
    [preferencepane_number_ release];
  }
  preferencepane_number_ = [NSMutableArray new];

  enum {
    XML_TYPE_CHECKBOX,
    XML_TYPE_NUMBER,
  };
  NSArray* paths = [NSArray arrayWithObjects:
                    [NSArray arrayWithObjects:[self get_private_xml_path],                                 [NSNumber numberWithInt:XML_TYPE_CHECKBOX], nil],
                    [NSArray arrayWithObjects:@"/Library/org.pqrs/KeyRemap4MacBook/prefpane/checkbox.xml", [NSNumber numberWithInt:XML_TYPE_CHECKBOX], nil],
                    [NSArray arrayWithObjects:@"/Library/org.pqrs/KeyRemap4MacBook/prefpane/number.xml",   [NSNumber numberWithInt:XML_TYPE_NUMBER],   nil],
                    nil];

  for (NSArray* pathinfo in paths) {
    @try {
      NSString* xmlpath = [pathinfo objectAtIndex:0];
      NSNumber* xmltype = [pathinfo objectAtIndex:1];

      if ([xmlpath length] == 0) continue;

      NSURL* url = [NSURL fileURLWithPath:xmlpath];
      NSError* error = nil;
      NSXMLDocument* xmldocument = [[[NSXMLDocument alloc] initWithContentsOfURL:url options:0 error:&error] autorelease];
      if (! xmldocument) {
        @throw [NSException exceptionWithName :[NSString stringWithFormat:@"%@ is invalid", xmlpath] reason :[error localizedDescription] userInfo : nil];
      }

      if ([xmltype intValue] == XML_TYPE_CHECKBOX) {
        [preferencepane_checkbox_ addObjectsFromArray:[self traverse_item:[xmldocument rootElement]]];
      } else if ([xmltype intValue] == XML_TYPE_NUMBER) {
        [preferencepane_number_   addObjectsFromArray:[self traverse_item:[xmldocument rootElement]]];
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
