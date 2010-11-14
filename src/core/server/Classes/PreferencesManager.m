#import "PreferencesManager.h"
#import "KeyCode.h"

@implementation PreferencesManager

// ----------------------------------------
- (void) addToDefault:(NSXMLElement*)element
{
  for (NSXMLElement* e in [element elementsForName : @"sysctl"]) {
    NSXMLNode* attr_default = [e attributeForName:@"default"];
    if (! attr_default) continue;

    [default_ setObject:[NSNumber numberWithInt:[[attr_default stringValue] intValue]] forKey:[e stringValue]];
  }

  for (NSXMLElement* e in [element elementsForName : @"list"]) {
    [self addToDefault:e];
  }
  for (NSXMLElement* e in [element elementsForName : @"item"]) {
    [self addToDefault:e];
  }
}

- (void) setDefault
{
  NSString* xmlpath = @"/Library/org.pqrs/KeyRemap4MacBook/prefpane/number.xml";
  NSURL* xmlurl = [NSURL fileURLWithPath:xmlpath];
  NSXMLDocument* xmldocument = [[[NSXMLDocument alloc] initWithContentsOfURL:xmlurl options:0 error:NULL] autorelease];
  if (xmldocument) {
    [self addToDefault:[xmldocument rootElement]];
  }
}

// ----------------------------------------
- (void) loadSelectedDictionary
{
  @synchronized(self) {
    NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];

    if (value_) {
      [value_ release];
    }
    value_ = [[NSMutableDictionary alloc] initWithCapacity:0];

    NSArray* configList = [userdefaults arrayForKey:@"configList"];
    if (! configList) return;

    NSUInteger selectedIndex = (NSUInteger)[userdefaults integerForKey:@"selectedIndex"];
    if (selectedIndex >= [configList count]) return;

    NSDictionary* configListItem = [configList objectAtIndex:selectedIndex];
    if (! configListItem) return;

    NSString* identifier = [configListItem objectForKey:@"identify"];
    if (! identifier) return;

    NSDictionary* dict = [userdefaults dictionaryForKey:identifier];
    if (! dict) return;

    [value_ addEntriesFromDictionary:dict];
  }
}

- (id) init
{
  [super init];

  default_ = [[NSMutableDictionary alloc] initWithCapacity:0];
  [self setDefault];
  [self loadSelectedDictionary];

  essential_config_index_ = [[NSArray alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"include.bridge_essential_config_index" ofType:@"plist"]];

  return self;
}

- (void) dealloc
{
  if (default_) {
    [default_ release];
  }
  if (value_) {
    [value_ release];
  }
  if (essential_config_index_) {
    [essential_config_index_ release];
  }
  [super dealloc];
}

// ----------------------------------------------------------------------
- (int) value:(NSString*)name
{
  int v = 0;
  @synchronized(self) {
    NSNumber* number = [value_ objectForKey:name];
    if (! number) {
      number = [default_ objectForKey:name];
    }

    if (number) {
      v = [number intValue];
    }
  }
  return v;
}

- (NSArray*) essential_config
{
  NSMutableArray* a = [[[NSMutableArray alloc] initWithCapacity:0] autorelease];
  @synchronized(self) {
    if (essential_config_index_) {
      for (NSString* name in essential_config_index_) {
        [a addObject:[NSNumber numberWithInt:[self value:name]]];
      }
    }
  }
  return a;
}

@end
