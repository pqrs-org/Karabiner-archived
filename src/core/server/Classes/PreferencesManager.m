#import "PreferencesManager.h"
#include <sys/time.h>

@implementation PreferencesManager

// ----------------------------------------
- (void) addToDefault:(NSXMLElement*)element
{
  for (NSXMLElement* e in [element elementsForName : @"identifier"]) {
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
- (id) init
{
  [super init];

  default_ = [[NSMutableDictionary alloc] initWithCapacity:0];
  [self setDefault];

  essential_config_index_ = [[NSArray alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"include.bridge_essential_config_index" ofType:@"plist"]];

  serverconnection_ = [NSConnection new];
  [serverconnection_ setRootObject:self];
  [serverconnection_ registerName:@"org.pqrs.KeyRemap4MacBook"];

  // ------------------------------------------------------------
  // scan config_* and detech notsave.*
  for (NSDictionary* dict in [self configlist_getConfigList]) {
    if (! dict) continue;

    NSString* identifier = [dict objectForKey:@"identify"];
    if (! identifier) continue;

    NSDictionary* d = [[NSUserDefaults standardUserDefaults] dictionaryForKey:identifier];
    if (! d) continue;

    NSMutableDictionary* md = [NSMutableDictionary dictionaryWithDictionary:d];

    for (NSString* name in [md allKeys]) {
      if ([name hasPrefix:@"notsave."]) {
        [md removeObjectForKey:name];
      }
    }

    [[NSUserDefaults standardUserDefaults] setObject:md forKey:identifier];
  }

  return self;
}

- (void) dealloc
{
  if (default_) {
    [default_ release];
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
  // user setting
  NSString* identifier = [self configlist_selectedIdentifier];
  if (identifier) {
    NSDictionary* dict = [[NSUserDefaults standardUserDefaults] dictionaryForKey:identifier];
    if (dict) {
      NSNumber* number = [dict objectForKey:name];
      if (number) {
        return [number intValue];
      }
    }
  }

  return [self defaultValue:name];
}

- (int) defaultValue:(NSString*)name
{
  NSNumber* number = [default_ objectForKey:name];
  if (number) {
    return [number intValue];
  } else {
    return 0;
  }
}

- (void) setValueForName:(int)newval forName:(NSString*)name
{
  NSString* identifier = [self configlist_selectedIdentifier];
  if (! identifier) return;

  NSDictionary* dict = [[NSUserDefaults standardUserDefaults] dictionaryForKey:identifier];
  if (! dict) return;

  NSMutableDictionary* md = [NSMutableDictionary dictionaryWithDictionary:dict];

  int defaultvalue = 0;
  NSNumber* defaultnumber = [default_ objectForKey:name];
  if (defaultnumber) {
    defaultvalue = [defaultnumber intValue];
  }

  if (newval == defaultvalue) {
    [md removeObjectForKey:name];
  } else {
    [md setObject:[NSNumber numberWithInt:newval] forKey:name];
  }

  [[NSUserDefaults standardUserDefaults] setObject:md forKey:identifier];
  //[[NSUserDefaults standardUserDefaults] synchronize];

  [[NSNotificationCenter defaultCenter] postNotificationName:@"PreferencesChanged" object:nil];
}

- (NSArray*) essential_config
{
  NSMutableArray* a = [[[NSMutableArray alloc] initWithCapacity:0] autorelease];

  if (essential_config_index_) {
    for (NSString* name in essential_config_index_) {
      [a addObject:[NSNumber numberWithInt:[self value:name]]];
    }
  }

  return a;
}

// ----------------------------------------------------------------------
- (NSInteger) configlist_selectedIndex
{
  return [[NSUserDefaults standardUserDefaults] integerForKey:@"selectedIndex"];
}

- (NSString*) configlist_selectedName
{
  return [self configlist_name:[self configlist_selectedIndex]];
}

- (NSString*) configlist_selectedIdentifier
{
  return [self configlist_identifier:[self configlist_selectedIndex]];
}

- (NSArray*) configlist_getConfigList
{
  return [[NSUserDefaults standardUserDefaults] arrayForKey:@"configList"];
}

- (NSUInteger) configlist_count
{
  NSArray* a = [self configlist_getConfigList];
  if (! a) return 0;
  return [a count];
}

- (NSDictionary*) configlist_dictionary:(NSInteger)rowIndex
{
  NSArray* list = [self configlist_getConfigList];
  if (! list) return nil;

  if (rowIndex < 0 || (NSUInteger)(rowIndex) >= [list count]) return nil;

  return [list objectAtIndex:rowIndex];
}

- (NSString*) configlist_name:(NSInteger)rowIndex
{
  NSDictionary* dict = [self configlist_dictionary:rowIndex];
  if (! dict) return nil;
  return [dict objectForKey:@"name"];
}

- (NSString*) configlist_identifier:(NSInteger)rowIndex
{
  NSDictionary* dict = [self configlist_dictionary:rowIndex];
  if (! dict) return nil;
  return [dict objectForKey:@"identify"];
}

- (void) configlist_select:(NSInteger)newindex
{
  if (newindex < 0) return;
  if (newindex == [self configlist_selectedIndex]) return;

  NSArray* list = [self configlist_getConfigList];
  if (! list) return;
  if ((NSUInteger)(newindex) >= [list count]) return;

  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
  [userdefaults setInteger:newindex forKey:@"selectedIndex"];

  [[NSNotificationCenter defaultCenter] postNotificationName:@"ConfigListChanged" object:nil];
  [[NSNotificationCenter defaultCenter] postNotificationName:@"PreferencesChanged" object:nil];
}

- (void) configlist_setName:(NSInteger)rowIndex name:(NSString*)name
{
  if ([name length] == 0) return;

  NSArray* a = [[NSUserDefaults standardUserDefaults] arrayForKey:@"configList"];
  if (! a) return;
  if (rowIndex < 0 || (NSUInteger)(rowIndex) >= [a count]) return;

  NSDictionary* d = [a objectAtIndex:rowIndex];
  if (! d) return;

  NSMutableDictionary* md = [NSMutableDictionary dictionaryWithDictionary:d];
  if (! md) return;
  [md setObject:name forKey:@"name"];

  NSMutableArray* ma = [NSMutableArray arrayWithArray:a];
  if (! ma) return;
  [ma replaceObjectAtIndex:rowIndex withObject:md];

  [[NSUserDefaults standardUserDefaults] setObject:ma forKey:@"configList"];

  [[NSNotificationCenter defaultCenter] postNotificationName:@"ConfigListChanged" object:nil];
}

- (void) configlist_append
{
  NSArray* a = [[NSUserDefaults standardUserDefaults] arrayForKey:@"configList"];
  if (! a) return;
  NSMutableArray* ma = [NSMutableArray arrayWithArray:a];
  if (! ma) return;

  struct timeval tm;
  gettimeofday(&tm, NULL);
  NSString* identifier = [NSString stringWithFormat:@"config_%d_%d", tm.tv_sec, tm.tv_usec];

  NSMutableDictionary* md = [NSMutableDictionary dictionaryWithCapacity:0];
  [md setObject:@"NewItem" forKey:@"name"];
  [md setObject:identifier forKey:@"identify"];

  [ma addObject:md];

  [[NSUserDefaults standardUserDefaults] setObject:ma forKey:@"configList"];

  [[NSNotificationCenter defaultCenter] postNotificationName:@"ConfigListChanged" object:nil];
}

- (void) configlist_delete:(NSInteger)rowIndex
{
  NSArray* a = [[NSUserDefaults standardUserDefaults] arrayForKey:@"configList"];
  if (! a) return;

  if (rowIndex < 0 || (NSUInteger)(rowIndex) >= [a count]) return;
  if (rowIndex == [self configlist_selectedIndex]) return;

  NSMutableArray* ma = [NSMutableArray arrayWithArray:a];
  if (! ma) return;

  [ma removeObjectAtIndex:(NSUInteger)(rowIndex)];

  [[NSUserDefaults standardUserDefaults] setObject:ma forKey:@"configList"];

  [[NSNotificationCenter defaultCenter] postNotificationName:@"ConfigListChanged" object:nil];
}

- (BOOL) isStatusbarEnable
{
  // If the key does not exist, treat as YES.
  id object = [[NSUserDefaults standardUserDefaults] objectForKey:@"isStatusbarEnable"];
  if (! object) return YES;

  NSInteger value = [[NSUserDefaults standardUserDefaults] integerForKey:@"isStatusbarEnable"];
  return value ? YES : NO;
}

- (BOOL) isShowSettingNameInStatusBar
{
  NSInteger value = [[NSUserDefaults standardUserDefaults] integerForKey:@"isShowSettingNameInStatusBar"];
  return value ? YES : NO;
}

- (void) toggleStatusbarEnable
{
  if ([self isStatusbarEnable]) {
    [[NSUserDefaults standardUserDefaults] setBool:NO forKey:@"isStatusbarEnable"];
  } else {
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"isStatusbarEnable"];
  }

  [[NSNotificationCenter defaultCenter] postNotificationName:@"ConfigListChanged" object:nil];
}

- (void) toggleShowSettingNameInStatusBar
{
  if ([self isShowSettingNameInStatusBar]) {
    [[NSUserDefaults standardUserDefaults] setBool:NO forKey:@"isShowSettingNameInStatusBar"];
  } else {
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"isShowSettingNameInStatusBar"];
  }

  [[NSNotificationCenter defaultCenter] postNotificationName:@"ConfigListChanged" object:nil];
}

// ----------------------------------------------------------------------
- (NSInteger) checkForUpdatesMode
{
  return [[NSUserDefaults standardUserDefaults] integerForKey:@"isCheckUpdate"];
}

- (void) setCheckForUpdatesMode:(NSInteger)newval
{
  [[NSUserDefaults standardUserDefaults] setInteger:newval forKey:@"isCheckUpdate"];
  //[[NSUserDefaults standardUserDefaults] synchronize];
}

@end
