#import "ConfigXMLParser.h"

@implementation ConfigXMLParserAppDefData

@synthesize name = name_;

- (id) init
{
  self = [super init];

  if (self) {
    rules_equal_ = [NSMutableArray new];
    rules_prefix_ = [NSMutableArray new];
  }

  return self;
}

- (void) dealloc
{
  [name_ release];
  [rules_equal_ release];
  [rules_prefix_ release];

  [super dealloc];
}

- (void) setName:(NSString*)newname;
{
  [name_ release];
  name_ = [newname retain];
}

- (void) addEqualRule:(NSString*)identifier
{
  [rules_equal_ addObject:identifier];
}

- (void) addPrefixRule:(NSString*)identifier
{
  [rules_prefix_ addObject:identifier];
}

- (NSString*) getApplicationType:(NSString*)bundleIdentifier
{
  if (! name_) return nil;

  for (NSString* identifier in rules_equal_) {
    if ([bundleIdentifier isEqualToString:identifier]) {
      goto found;
    }
  }
  for (NSString* identifier in rules_prefix_) {
    if ([bundleIdentifier hasPrefix:identifier]) {
      goto found;
    }
  }

  return nil;

found:
  return [NSString stringWithFormat:@"ApplicationType::%@", name_];
}

@end

@implementation ConfigXMLParser (AppDef)

- (ConfigXMLParserAppDefData*) parse_appdef:(NSXMLElement*)appdefElement
{
  ConfigXMLParserAppDefData* newdata = [[ConfigXMLParserAppDefData new] autorelease];

  NSUInteger count = [appdefElement childCount];
  for (NSUInteger i = 0; i < count; ++i) {
    NSXMLElement* e = [self castToNSXMLElement:[appdefElement childAtIndex:i]];
    if (! e) continue;

    NSString* name = [e name];

    if ([name isEqualToString:@"appname"]) {
      NSString* appname = [self trim:[e stringValue]];

      // Skip if appname is already registered.
      if (appname) {
        for (ConfigXMLParserAppDefData* data in appdefdata_) {
          if ([data.name isEqualToString:appname]) {
            NSLog(@"[INFO] AppDef:%@ is already registered. Skipped it.", appname);
            return nil;
          }
        }
      }

      [newdata setName:appname];

    } else if ([name isEqualToString:@"equal"]) {
      [newdata addEqualRule:[self trim:[e stringValue]]];

    } else if ([name isEqualToString:@"prefix"]) {
      [newdata addPrefixRule:[self trim:[e stringValue]]];
    }
  }

  if (! newdata.name) return nil;

  return newdata;
}

- (void) traverse_appdef:(NSXMLElement*)element
{
  NSUInteger count = [element childCount];
  for (NSUInteger i = 0; i < count; ++i) {
    NSXMLElement* e = [self castToNSXMLElement:[element childAtIndex:i]];
    if (! e) continue;

    if (! [[e name] isEqualToString:@"appdef"]) {
      [self traverse_appdef:e];

    } else {
      ConfigXMLParserAppDefData* newdata = [self parse_appdef:e];
      if (newdata) {
        [appdefdata_ addObject:newdata];

        // Adding to keycode_ if needed.
        if (! [keycode_ isExists:[NSString stringWithFormat:@"ApplicationType::%@", newdata.name]]) {
          [keycode_ append:@"ApplicationType" name:newdata.name];
        }
      }
    }
  }
}

- (BOOL) reload_appdef {
  BOOL retval = NO;

  [appdefdata_ release];
  appdefdata_ = [NSMutableArray new];

  NSArray* paths = [NSArray arrayWithObjects:
                    [self preferencepane_get_private_xml_path],
                    [[NSBundle mainBundle] pathForResource:@"appdef" ofType:@"xml"],
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

      [self traverse_appdef:[xmldocument rootElement]];

      // Set retval to YES if only one XML file is loaded successfully.
      // Unless we do it, all setting becomes disabled by one error.
      // (== If private.xml is invalid, system wide appdef.xml is not loaded.)
      retval = YES;

    } @catch (NSException* exception) {
      [self setErrorMessage:exception xmlpath:xmlpath];
    }
  }

  return retval;
}

@end
