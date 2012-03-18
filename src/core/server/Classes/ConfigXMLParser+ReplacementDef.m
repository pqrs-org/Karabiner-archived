#import "ConfigXMLParser.h"
#import "NSString+HashBraces.h"

@interface ConfigXMLParserReplacementDefData : NSObject {
  NSString* name;
  NSString* value;
}
@property (assign, readwrite) NSString* name;
@property (assign, readwrite) NSString* value;

@end

@implementation ConfigXMLParserReplacementDefData

@synthesize name;
@synthesize value;

@end

@implementation ConfigXMLParser (ReplacementDef)

- (ConfigXMLParserReplacementDefData*) parse_replacementdef:(NSXMLElement*)replacementdefElement
{
  ConfigXMLParserReplacementDefData* newdata = [[ConfigXMLParserReplacementDefData new] autorelease];

  NSUInteger count = [replacementdefElement childCount];
  for (NSUInteger i = 0; i < count; ++i) {
    NSXMLElement* e = [self castToNSXMLElement:[replacementdefElement childAtIndex:i]];
    if (! e) continue;

    NSString* name = [e name];
    NSString* stringValue = [self trim:[e stringValue]];

    if ([name isEqualToString:@"replacementname"]) {
      newdata.name = [NSString stringWithFormat:@"{{%@}}", stringValue];
    } else if ([name isEqualToString:@"replacementvalue"]) {
      newdata.value = stringValue;
    }
  }

  if (! newdata.name) return nil;
  if (! newdata.value) return nil;

  return newdata;
}

- (void) traverse_replacementdef:(NSXMLElement*)element
{
  NSUInteger count = [element childCount];
  for (NSUInteger i = 0; i < count; ++i) {
    NSXMLElement* e = [self castToNSXMLElement:[element childAtIndex:i]];
    if (! e) continue;

    if (! [[e name] isEqualToString:@"replacementdef"]) {
      [self traverse_replacementdef:e];

    } else {
      ConfigXMLParserReplacementDefData* newdata = [self parse_replacementdef:e];
      if (newdata) {
        // Adding to replacement_ if needed.
        if (! [replacement_ objectForKey:newdata.name]) {
          [replacement_ setObject:newdata.value forKey:newdata.name];
        }
      }
    }
  }
}

- (BOOL) reload_replacementdef {
  BOOL retval = NO;

  [replacement_ release];
  replacement_ = [NSMutableDictionary new];

  NSArray* paths = [NSArray arrayWithObjects:
                    [self preferencepane_get_private_xml_path],
                    [[NSBundle mainBundle] pathForResource:@"replacementdef" ofType:@"xml"],
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

      [self traverse_replacementdef:[xmldocument rootElement]];

      // Set retval to YES if only one XML file is loaded successfully.
      // Unless we do it, all setting becomes disabled by one error.
      // (== If private.xml is invalid, system wide replacementdef.xml is not loaded.)
      retval = YES;

    } @catch (NSException* exception) {
      [self setErrorMessage:exception xmlpath:xmlpath];
    }
  }

  return retval;
}

- (NSXMLDocument*) documentWithPathApplyingReplacement:(NSString*)filepath error:(NSError**)error {
  NSString* xmlstring = [NSString stringWithContentsOfFile:filepath
                                                  encoding:NSUTF8StringEncoding
                                                     error:error];
  if (! xmlstring) return nil;

  xmlstring = [xmlstring stringByReplacingHashBracesOccurrencesOfDictionary:replacement_];

  return [[[NSXMLDocument alloc] initWithXMLString:xmlstring options:0 error:error] autorelease];
}

@end
