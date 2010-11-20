#import "ConfigXMLParser.h"
#import "bridge.h"

@implementation ConfigXMLParser (PreferencePane)

- (NSMutableArray*) traverse_item:(NSXMLElement*)element_list identifier_prefix:(NSString*)identifier_prefix
{
  NSMutableArray* array = [[NSMutableArray new] autorelease];

  for (NSXMLElement* element_item in [element_list elementsForName : @"item"]) {
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
    [dict setObject:title forKey:@"name"];
    [dict setObject:[NSNumber numberWithUnsignedInteger:height] forKey:@"height"];

    // ----------------------------------------
    NSArray* elements_identifier = [element_item elementsForName:@"identifier"];
    if ([elements_identifier count] == 1) {
      NSXMLElement* element_identifier = [elements_identifier objectAtIndex:0];
      [dict setObject:[element_identifier stringValue] forKey:@"identifier"];

      NSXMLNode* attr_default = [element_identifier attributeForName:@"default"];
      if (attr_default) {
        [dict setObject:[attr_default stringValue] forKey:@"default"];
      } else {
        [dict setObject:@"0" forKey:@"default"];
      }

      NSXMLNode* attr_baseunit = [element_identifier attributeForName:@"baseunit"];
      if (attr_baseunit) {
        [dict setObject:[attr_baseunit stringValue] forKey:@"baseunit"];
      }

    } else if ([elements_identifier count] >= 2) {
      @throw [NSException exceptionWithName : @"<item> is invalid" reason :[NSString stringWithFormat:@"multiple <identifier> in one <item>.\n%@", [element_item stringValue]] userInfo : nil];
    }

    // ----------------------------------------
    NSMutableArray* a = [[NSMutableArray new] autorelease];
    for (NSXMLElement* child_list in [element_item elementsForName : @"list"]) {
      [a addObjectsFromArray:[self traverse_item:child_list identifier_prefix:identifier_prefix]];
    }
    if ([a count] > 0) {
      [dict setObject:a forKey:@"children"];
    }

    // ----------------------------------------
    [array addObject:dict];
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
                    [NSArray arrayWithObjects:[self get_private_xml_path],                                 [NSNumber numberWithInt:XML_TYPE_CHECKBOX], @"private.", nil],
                    [NSArray arrayWithObjects:@"/Library/org.pqrs/KeyRemap4MacBook/prefpane/checkbox.xml", [NSNumber numberWithInt:XML_TYPE_CHECKBOX], @"",         nil],
                    [NSArray arrayWithObjects:@"/Library/org.pqrs/KeyRemap4MacBook/prefpane/number.xml",   [NSNumber numberWithInt:XML_TYPE_NUMBER],   @"",         nil],
                    nil];

  for (NSArray* pathinfo in paths) {
    @try {
      NSString* xmlpath           = [pathinfo objectAtIndex:0];
      NSNumber* xmltype           = [pathinfo objectAtIndex:1];
      NSString* identifier_prefix = [pathinfo objectAtIndex:2];

      if ([xmlpath length] == 0) continue;

      NSURL* url = [NSURL fileURLWithPath:xmlpath];
      NSError* error = nil;
      NSXMLDocument* xmldocument = [[[NSXMLDocument alloc] initWithContentsOfURL:url options:0 error:&error] autorelease];
      if (! xmldocument) {
        @throw [NSException exceptionWithName :[NSString stringWithFormat:@"%@ is invalid", xmlpath] reason :[error localizedDescription] userInfo : nil];
      }

      for (NSXMLElement* element_list in [[xmldocument rootElement] elementsForName : @"list"]) {
        NSMutableArray* targetarray = nil;
        if ([xmltype intValue] == XML_TYPE_CHECKBOX) {
          targetarray = preferencepane_checkbox_;
        } else if ([xmltype intValue] == XML_TYPE_NUMBER) {
          targetarray = preferencepane_number_;
        }
        if (targetarray) {
          [targetarray addObjectsFromArray:[self traverse_item:element_list identifier_prefix:identifier_prefix]];
        }
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
