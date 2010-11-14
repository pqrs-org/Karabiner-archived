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
    if (value_) {
      [value_ release];
    }
    value_ = [[NSMutableDictionary alloc] initWithCapacity:0];

    NSArray* configList = [self getConfigList];
    if (! configList) return;

    NSUInteger selectedIndex = (NSUInteger)[self selectedIndex];
    if (selectedIndex >= [configList count]) return;

    NSDictionary* configListItem = [configList objectAtIndex:selectedIndex];
    if (! configListItem) return;

    NSString* identifier = [configListItem objectForKey:@"identify"];
    if (! identifier) return;

    NSDictionary* dict = [[NSUserDefaults standardUserDefaults] dictionaryForKey:identifier];
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

  serverconnection_ = [NSConnection new];
  [serverconnection_ setRootObject:self];
  [serverconnection_ registerName:@"org.pqrs.KeyRemap4MacBook"];

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
  if (serverconnection_) {
    [serverconnection_ release];
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

// ----------------------------------------------------------------------
- (NSInteger) selectedIndex
{
  return [[NSUserDefaults standardUserDefaults] integerForKey:@"selectedIndex"];
}

- (NSArray*) getConfigList
{
  return [[NSUserDefaults standardUserDefaults] arrayForKey:@"configList"];
}

- (void) select:(NSInteger)newindex
{
  if (newindex < 0) return;
  if (newindex == [self selectedIndex]) return;

  NSArray* list = [self getConfigList];
  if ((NSUInteger)(newindex) >= [list count]) return;

  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];

  [userdefaults setInteger:newindex forKey:@"selectedIndex"];
  [userdefaults synchronize];
  [self loadSelectedDictionary];
}

- (BOOL) isStatusbarEnable
{
  id object = [[NSUserDefaults standardUserDefaults] objectForKey:@"isStatusbarEnable"];
  if (! object) return YES;

  NSInteger value = [[NSUserDefaults standardUserDefaults] integerForKey:@"isStatusbarEnable"];
  return value ? YES : NO;
}

@end
