#import "PreferencesManager.h"
#import "XMLCompiler.h"
#import "KeyRemap4MacBookKeys.h"
#import "KeyRemap4MacBookNSDistributedNotificationCenter.h"
#import "UserClient_userspace.h"
#include <sys/time.h>

static PreferencesManager* global_instance = nil;

@implementation PreferencesManager

+ (PreferencesManager*) getInstance
{
  @synchronized(self) {
    if (! global_instance) {
      global_instance = [PreferencesManager new];
    }
  }
  return global_instance;
}

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
  NSURL* xmlurl = [[NSBundle mainBundle] URLForResource:@"number" withExtension:@"xml"];
  NSXMLDocument* xmldocument = [[[NSXMLDocument alloc] initWithContentsOfURL:xmlurl options:0 error:NULL] autorelease];
  if (xmldocument) {
    [self addToDefault:[xmldocument rootElement]];
  }
}

// ----------------------------------------
- (id) init
{
  self = [super init];

  if (self) {
    default_ = [NSMutableDictionary new];
    [self setDefault];

    essential_configuration_identifiers_ = [[NSArray arrayWithObjects:
#include "../../../bridge/output/include.bridge_essential_configuration_identifiers.m"
                                            ] retain];

    // ------------------------------------------------------------
    // initialize
    if (! [self configlist_selectedIdentifier]) {
      [self configlist_select:0];

      if (! [self configlist_selectedIdentifier]) {
        NSLog(@"initialize configlist");
        // add new item

        [self configlist_append];
        [self configlist_setName:0 name:@"Default"];
        [self configlist_select:0];
      }
    }

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

    // ------------------------------------------------------------
    serverconnection_ = [NSConnection new];
    [serverconnection_ setRootObject:self];
    if (! [serverconnection_ registerName:kKeyRemap4MacBookConnectionName]) {
      // Quit when registerName is failed.
      // We wait 2 second before quit to avoid consecutive restarting from launchd.
      NSLog(@"[NSConnection registerName] is failed. Restarting process.");
      [NSThread sleepForTimeInterval:2];
      [NSApp terminate: nil];
    }

    [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter postNotificationName:kKeyRemap4MacBookServerLaunchedNotification userInfo:nil];
  }

  return self;
}

- (void) dealloc
{
  [default_ release];
  [essential_configuration_identifiers_ release];
  [serverconnection_ release];

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
  if (! identifier) {
    NSLog(@"[ERROR] %s identifier == nil", __FUNCTION__);
    return;
  }

  NSMutableDictionary* md = nil;

  NSDictionary* dict = [[NSUserDefaults standardUserDefaults] dictionaryForKey:identifier];
  if (dict) {
    md = [NSMutableDictionary dictionaryWithDictionary:dict];
  } else {
    md = [[NSMutableDictionary new] autorelease];
  }
  if (! md) {
    NSLog(@"[ERROR] %s md == nil", __FUNCTION__);
    return;
  }

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
  // [[NSUserDefaults standardUserDefaults] synchronize];

  [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter postNotificationName:kKeyRemap4MacBookPreferencesChangedNotification userInfo:nil];
}

- (NSArray*) essential_config
{
  NSMutableArray* a = [[NSMutableArray new] autorelease];

  if (essential_configuration_identifiers_) {
    for (NSString* identifier in essential_configuration_identifiers_) {
      [a addObject:[NSNumber numberWithInt:[self value:identifier]]];
    }
  }

  return a;
}

- (NSDictionary*) changed
{
  NSString* identifier = [self configlist_selectedIdentifier];
  if (! identifier) return nil;

  return [[NSUserDefaults standardUserDefaults] dictionaryForKey:identifier];
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

  [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter postNotificationName:kKeyRemap4MacBookPreferencesChangedNotification userInfo:nil];
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
  NSMutableArray* ma = nil;

  NSArray* a = [[NSUserDefaults standardUserDefaults] arrayForKey:@"configList"];
  if (a) {
    ma = [NSMutableArray arrayWithArray:a];
  } else {
    ma = [[NSMutableArray new] autorelease];
  }
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

  NSInteger selectedIndex = [self configlist_selectedIndex];
  if (rowIndex == selectedIndex) return;

  NSMutableArray* ma = [NSMutableArray arrayWithArray:a];
  if (! ma) return;

  [ma removeObjectAtIndex:(NSUInteger)(rowIndex)];

  [[NSUserDefaults standardUserDefaults] setObject:ma forKey:@"configList"];

  // When Item2 is deleted in the following condition,
  // we need to decrease selected index 2->1.
  //
  // - Item1
  // - Item2
  // - Item3 [selected]
  //
  if (rowIndex < selectedIndex) {
    [self configlist_select:(selectedIndex - 1)];
  }

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
  // If the key does not exist, treat as "The stable release only".
  if (! [[NSUserDefaults standardUserDefaults] objectForKey:@"isCheckUpdate"]) {
    return 1;
  }
  return [[NSUserDefaults standardUserDefaults] integerForKey:@"isCheckUpdate"];
}

- (void) setCheckForUpdatesMode:(NSInteger)newval
{
  [[NSUserDefaults standardUserDefaults] setInteger:newval forKey:@"isCheckUpdate"];
  // [[NSUserDefaults standardUserDefaults] synchronize];
}

// ----------------------------------------------------------------------
- (void) configxml_reload
{
  [[XMLCompiler getInstance] reload];
}

- (NSArray*) preferencepane_checkbox
{
  return [[XMLCompiler getInstance] preferencepane_checkbox];
}

- (NSArray*) preferencepane_number
{
  return [[XMLCompiler getInstance] preferencepane_number];
}

- (int) enabled_count:(NSArray*)checkbox changed:(NSDictionary*)changed
{
  int count = 0;

  if (checkbox) {
    for (NSDictionary* dict in checkbox) {
      NSString* identifier = [dict objectForKey:@"identifier"];
      if (identifier) {
        if ([[changed objectForKey:identifier] intValue] != 0) {
          ++count;
        }
      }

      count += [self enabled_count:[dict objectForKey:@"children"] changed:changed];
    }
  }

  return count;
}

- (int) preferencepane_enabled_count
{
  NSArray* checkbox = [[XMLCompiler getInstance] preferencepane_checkbox];
  NSDictionary* changed = [self changed];
  return [self enabled_count:checkbox changed:changed];
}

- (NSString*) preferencepane_error_message
{
  return [[XMLCompiler getInstance] preferencepane_error_message];
}

- (NSString*) preferencepane_get_private_xml_path
{
  return [XMLCompiler get_private_xml_path];
}

- (NSString*) preferencepane_version
{
  return [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"];
}

- (NSArray*) device_information:(NSInteger)type
{
  NSMutableArray* information = [[NSMutableArray new] autorelease];

  for (size_t i = 0;; ++i) {
    struct BridgeDeviceInformation deviceInformation;

    struct BridgeUserClientStruct bridgestruct;
    bridgestruct.type   = (uint32_t)(type);
    bridgestruct.option = i;
    bridgestruct.data   = (uintptr_t)(&deviceInformation);
    bridgestruct.size   = sizeof(deviceInformation);

    if (! [UserClient_userspace synchronized_communication:&bridgestruct]) break;

    if (! deviceInformation.isFound) break;

    NSDictionary* newdict = [NSDictionary dictionaryWithObjectsAndKeys:
                             [NSString stringWithUTF8String:deviceInformation.manufacturer], @"manufacturer",
                             [NSString stringWithUTF8String:deviceInformation.product], @"product",
                             [NSString stringWithFormat:@"0x%x", deviceInformation.vendorID], @"vendorID",
                             [NSString stringWithFormat:@"0x%x", deviceInformation.productID], @"productID",
                             [NSString stringWithFormat:@"0x%x", deviceInformation.locationID], @"locationID",
                             nil];

    // skip if newdict is already exists.
    BOOL found = NO;
    for (NSDictionary* d in information) {
      if ([newdict isEqualToDictionary:d]) {
        found = YES;
      }
    }
    if (! found) {
      [information addObject:newdict];
    }
  }

  return information;
}

@end
