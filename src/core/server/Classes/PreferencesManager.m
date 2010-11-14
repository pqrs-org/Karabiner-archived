#import "PreferencesManager.h"
#import "KeyCode.h"

@implementation PreferencesManager

- (void) addToDefault:(NSXMLElement*)element
{
  for (NSXMLElement* e in [element elementsForName : @"sysctl"]) {
    NSXMLNode* attr_default = [e attributeForName:@"default"];
    if (! attr_default) continue;

    [default_ setObject:[NSNumber numberWithInt:[[attr_default stringValue] intValue]] forKey:[KeyCode normalizeName:[e stringValue]]];
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

- (id) init
{
  [super init];

  default_ = [[NSMutableDictionary alloc] initWithCapacity:0];
  [self setDefault];

  return self;
}

- (void) dealloc
{
  if (default_) {
    [default_ release];
  }
  [super dealloc];
}

// ----------------------------------------------------------------------
- (unsigned int) value:(NSString*)name
{
  [[NSUserDefaults standardUserDefaults] integerForKey:@"selectedIndex"];
  return 0;
}

@end
