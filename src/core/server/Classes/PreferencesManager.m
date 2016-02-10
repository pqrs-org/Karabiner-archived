#import "ClientForKernelspace.h"
#import "GlobalDomainKeyRepeatObserver.h"
#import "NotificationKeys.h"
#import "PreferencesKeys.h"
#import "PreferencesManager.h"
#import "XMLCompiler.h"
#include <sys/time.h>

@interface PreferencesManager () {
  NSMutableDictionary* default_;
  NSArray* essential_configuration_identifiers_;
  GlobalDomainKeyRepeatObserver* globalDomainKeyRepeatObserver_;
}
@end

@implementation PreferencesManager

// ----------------------------------------
+ (void)initialize {
  NSDictionary* dict = @{
    kIsStatusBarEnabled : @YES,
    kIsShowSettingNameInStatusBar : @NO,
    kConfigListSelectedIndex : @0,
    kCheckForUpdates : @YES,
    kIsStatusWindowEnabled : @YES,
    kIsStatusWindowShowCapsLock : @NO,
    kIsStatusWindowShowStickyModifier : @NO,
    kIsStatusWindowShowPointingButtonLock : @YES,
    kStatusWindowType : @0,  // Normal
    kStatusWindowTheme : @0, // White
    kStatusWindowOpacity : @80,
    kStatusWindowFontSize : @0, // Small
    kStatusWindowPosition : @3, // Bottom right
    kIsAXNotifierEnabled : @YES,
    kAXNotifierDisabledInJavaApps : @YES,
    kAXNotifierDisabledInQtApps : @YES,
    kAXNotifierDisabledInPreview : @YES,
    kAXNotifierDisabledInMicrosoftOffice : @YES,
    kUsePreparedSettings : @YES,
    kIsMigratedIsOverwriteKeyRepeat : @NO,
    kIsOverwriteKeyRepeat : @NO,
    kShowIconInDock : @NO,
    kKarabinerPreferencesCheckboxFont : @0,
  };
  [[NSUserDefaults standardUserDefaults] registerDefaults:dict];
}

// ----------------------------------------
- (void)addToDefault:(NSXMLElement*)element {
  for (NSXMLElement* e in [element elementsForName:@"identifier"]) {
    NSXMLNode* attr_default = [e attributeForName:@"default"];
    if (!attr_default) continue;

    default_[[e stringValue]] = @([[attr_default stringValue] intValue]);
  }

  for (NSXMLElement* e in [element elementsForName:@"list"]) {
    [self addToDefault:e];
  }
  for (NSXMLElement* e in [element elementsForName:@"item"]) {
    [self addToDefault:e];
  }
}

- (void)setDefault {
  NSURL* xmlurl = [[NSBundle mainBundle] URLForResource:@"number" withExtension:@"xml"];
  NSXMLDocument* xmldocument = [[NSXMLDocument alloc] initWithContentsOfURL:xmlurl options:0 error:NULL];
  if (xmldocument) {
    [self addToDefault:[xmldocument rootElement]];
  }
}

// ----------------------------------------
- (id)init {
  self = [super init];

  if (self) {
    default_ = [NSMutableDictionary new];
    [self setDefault];

    globalDomainKeyRepeatObserver_ = [[GlobalDomainKeyRepeatObserver alloc] initWithPreferencesManager:self];

    essential_configuration_identifiers_ = [NSArray arrayWithObjects:
#include "../../../bridge/output/include.bridge_essential_configuration_identifiers.m"
    ];
  }

  return self;
}

- (void)load {
  // ------------------------------------------------------------
  // initialize
  if (![self configlist_selectedIdentifier]) {
    [self configlist_select:0];

    if (![self configlist_selectedIdentifier]) {
      NSLog(@"initialize configlist");

      // add new item
      [self configlist_append];
      [self configlist_setName:0 name:@"Default"];
      [self configlist_select:0];
    }
  }

  // ------------------------------------------------------------
  // set AppendIndex if needed.
  {
    NSMutableArray* ma = [NSMutableArray new];
    NSInteger appendIndex = [self configlist_maxAppendIndex] + 1;

    for (NSDictionary* d in [[NSUserDefaults standardUserDefaults] arrayForKey:@"configList"]) {
      NSMutableDictionary* md = [NSMutableDictionary dictionaryWithDictionary:d];

      if (!md[@"appendIndex"]) {
        md[@"appendIndex"] = @(appendIndex);
        ++appendIndex;
      }

      [ma addObject:md];
    }

    [[NSUserDefaults standardUserDefaults] setObject:ma forKey:@"configList"];
  }

  // ------------------------------------------------------------
  // scan config_* and detech notsave.*
  for (NSDictionary* dict in [self configlist_getConfigList]) {
    if (!dict) continue;

    NSString* identifier = dict[@"identify"];
    if (!identifier) continue;

    NSDictionary* d = [[NSUserDefaults standardUserDefaults] dictionaryForKey:identifier];
    if (!d) continue;

    NSMutableDictionary* md = [NSMutableDictionary dictionaryWithDictionary:d];

    for (NSString* name in [md allKeys]) {
      if ([name hasPrefix:@"notsave."]) {
        [md removeObjectForKey:name];
      }
    }

    [[NSUserDefaults standardUserDefaults] setObject:md forKey:identifier];
  }

  // ------------------------------------------------------------
  // migration
  if (![[NSUserDefaults standardUserDefaults] boolForKey:kIsMigratedIsOverwriteKeyRepeat]) {
    [[NSUserDefaults standardUserDefaults] setObject:@YES forKey:kIsMigratedIsOverwriteKeyRepeat];

    if ([self valueAsNumber:@"repeat.initial_wait"] || [self valueAsNumber:@"repeat.wait"]) {
      [[NSUserDefaults standardUserDefaults] setObject:@YES forKey:kIsOverwriteKeyRepeat];

      if (![self valueAsNumber:@"repeat.initial_wait"]) {
        // old defalut value is 500.
        [self setValue:500 forName:@"repeat.initial_wait"];
      }
      if (![self valueAsNumber:@"repeat.wait"]) {
        // old defalut value is 83.
        [self setValue:83 forName:@"repeat.wait"];
      }
    } else {
      [[NSUserDefaults standardUserDefaults] setObject:@NO forKey:kIsOverwriteKeyRepeat];
    }

    NSLog(@"Migration: Set kIsOverwriteKeyRepeat:%d", [[NSUserDefaults standardUserDefaults] boolForKey:kIsOverwriteKeyRepeat]);
  }

  // ------------------------------------------------------------
  [globalDomainKeyRepeatObserver_ start];
}

// ----------------------------------------------------------------------
- (NSNumber*)valueAsNumber:(NSString*)name {
  // user setting
  NSString* identifier = [self configlist_selectedIdentifier];
  if (identifier) {
    NSDictionary* dict = [[NSUserDefaults standardUserDefaults] dictionaryForKey:identifier];
    if (dict) {
      return dict[name];
    }
  }

  return nil;
}

- (int)value:(NSString*)name {
  NSNumber* number = [self valueAsNumber:name];
  if (number) {
    return [number intValue];
  } else {
    return [self defaultValue:name];
  }
}

- (int)defaultValue:(NSString*)name {
  NSNumber* number = default_[name];
  if (number) {
    return [number intValue];
  } else {
    return 0;
  }
}

- (void)setValue:(int)newval forName:(NSString*)name {
  [self setValue:newval forName:name tellToKext:YES];
}

- (void)setValue:(int)newval forName:(NSString*)name tellToKext:(BOOL)tellToKext {
  [self setValue:newval forName:name tellToKext:tellToKext notificationUserInfo:nil];
}

- (void)setValue:(int)newval forName:(NSString*)name tellToKext:(BOOL)tellToKext notificationUserInfo:(NSDictionary*)notificationUserInfo {
  @synchronized(self) {
    int oldval = [self value:name];

    NSString* identifier = [self configlist_selectedIdentifier];
    if (!identifier) {
      NSLog(@"[ERROR] %s identifier == nil", __FUNCTION__);
      return;
    }

    NSMutableDictionary* md = nil;

    NSDictionary* dict = [[NSUserDefaults standardUserDefaults] dictionaryForKey:identifier];
    if (dict) {
      md = [NSMutableDictionary dictionaryWithDictionary:dict];
    } else {
      md = [NSMutableDictionary new];
    }
    if (!md) {
      NSLog(@"[ERROR] %s md == nil", __FUNCTION__);
      return;
    }

    int defaultvalue = 0;
    NSNumber* defaultnumber = default_[name];
    if (defaultnumber) {
      defaultvalue = [defaultnumber intValue];
    }

    if (newval == defaultvalue) {
      [md removeObjectForKey:name];
    } else {
      md[name] = @(newval);
    }

    [[NSUserDefaults standardUserDefaults] setObject:md forKey:identifier];
    // [[NSUserDefaults standardUserDefaults] synchronize];

    if (oldval != newval) {
      // We post notification only when newval is different from oldval in order to avoid infinity loop.
      // (`setValue` might be called in observer.)
      [[NSNotificationCenter defaultCenter] postNotificationName:kPreferencesChangedNotification object:nil userInfo:notificationUserInfo];
      if (tellToKext) {
        [self callSetConfigOne:name value:newval];
      }
    }
  }
}

- (void)callSetConfigOne:(NSString*)name value:(int)value {
  struct BridgeSetConfigOne bridgeSetConfigOne;

  for (NSUInteger i = 0, count = [essential_configuration_identifiers_ count]; i < count; ++i) {
    if ([essential_configuration_identifiers_[i] isEqualToString:name]) {
      bridgeSetConfigOne.isEssentialConfig = 1;
      bridgeSetConfigOne.index = (uint32_t)(i);
      bridgeSetConfigOne.value = (int32_t)(value);
      [clientForKernelspace_ set_config_one:&bridgeSetConfigOne];
      return;
    }
  }

  int config_index = [xmlCompiler_ config_index:name];
  if (config_index >= 0) {
    bridgeSetConfigOne.isEssentialConfig = 0;
    bridgeSetConfigOne.index = (uint32_t)(config_index);
    bridgeSetConfigOne.value = (int32_t)(value);
    [clientForKernelspace_ set_config_one:&bridgeSetConfigOne];
    return;
  }
}

- (void)clearNotSave {
  @synchronized(self) {
    // user setting
    NSString* identifier = [self configlist_selectedIdentifier];
    if (identifier) {
      NSDictionary* dict = [[NSUserDefaults standardUserDefaults] dictionaryForKey:identifier];
      if (dict) {
        NSMutableDictionary* md = [NSMutableDictionary dictionaryWithDictionary:dict];

        BOOL changed = NO;

        for (NSString* key in dict) {
          if ([key hasPrefix:@"notsave."]) {
            [md removeObjectForKey:key];
            changed = YES;
          }
        }

        [[NSUserDefaults standardUserDefaults] setObject:md forKey:identifier];

        if (changed) {
          [[NSNotificationCenter defaultCenter] postNotificationName:kPreferencesChangedNotification object:nil];
          [clientForKernelspace_ send_config_to_kext];
        }
      }
    }
  }
}

- (NSArray*)essential_config {
  NSMutableArray* a = [NSMutableArray new];

  if (essential_configuration_identifiers_) {
    for (NSString* identifier in essential_configuration_identifiers_) {
      [a addObject:@([self value:identifier])];
    }
  }

  return a;
}

- (NSDictionary*)changed {
  NSString* identifier = [self configlist_selectedIdentifier];
  if (!identifier) return nil;

  return [[NSUserDefaults standardUserDefaults] dictionaryForKey:identifier];
}

// ----------------------------------------------------------------------
- (NSInteger)configlist_selectedIndex {
  return [[NSUserDefaults standardUserDefaults] integerForKey:@"selectedIndex"];
}

- (NSString*)configlist_selectedName {
  return [self configlist_name:[self configlist_selectedIndex]];
}

- (NSString*)configlist_selectedIdentifier {
  return [self configlist_identifier:[self configlist_selectedIndex]];
}

- (NSArray*)configlist_getConfigList {
  return [[NSUserDefaults standardUserDefaults] arrayForKey:@"configList"];
}

- (NSUInteger)configlist_count {
  NSArray* a = [self configlist_getConfigList];
  if (!a) return 0;
  return [a count];
}

- (NSDictionary*)configlist_dictionary:(NSInteger)rowIndex {
  NSArray* list = [self configlist_getConfigList];
  if (!list) return nil;

  if (rowIndex < 0 || (NSUInteger)(rowIndex) >= [list count]) return nil;

  return list[rowIndex];
}

- (NSString*)configlist_name:(NSInteger)rowIndex {
  NSDictionary* dict = [self configlist_dictionary:rowIndex];
  if (!dict) return nil;
  return dict[@"name"];
}

- (NSString*)configlist_identifier:(NSInteger)rowIndex {
  NSDictionary* dict = [self configlist_dictionary:rowIndex];
  if (!dict) return nil;
  return dict[@"identify"];
}

- (void)configlist_select:(NSInteger)newindex {
  if (newindex < 0) return;
  if (newindex == [self configlist_selectedIndex]) return;

  NSArray* list = [self configlist_getConfigList];
  if (!list) return;
  if ((NSUInteger)(newindex) >= [list count]) return;

  NSUserDefaults* userdefaults = [NSUserDefaults standardUserDefaults];
  [userdefaults setInteger:newindex forKey:@"selectedIndex"];

  [[NSNotificationCenter defaultCenter] postNotificationName:kConfigListChangedNotification object:nil];
  [[NSNotificationCenter defaultCenter] postNotificationName:kPreferencesChangedNotification object:nil];
  [clientForKernelspace_ send_config_to_kext];
}

- (void)configlist_selectByIdentifier:(NSString*)identifier {
  NSInteger count = (NSInteger)([self configlist_count]);
  for (NSInteger i = 0; i < count; ++i) {
    if ([identifier isEqualToString:[self configlist_identifier:i]]) {
      [self configlist_select:i];
      return;
    }
  }
}

- (void)configlist_setName:(NSInteger)rowIndex name:(NSString*)name {
  if ([name length] == 0) return;

  NSArray* a = [[NSUserDefaults standardUserDefaults] arrayForKey:@"configList"];
  if (!a) return;
  if (rowIndex < 0 || (NSUInteger)(rowIndex) >= [a count]) return;

  NSDictionary* d = a[rowIndex];
  if (!d) return;

  NSMutableDictionary* md = [NSMutableDictionary dictionaryWithDictionary:d];
  if (!md) return;
  md[@"name"] = name;

  NSMutableArray* ma = [NSMutableArray arrayWithArray:a];
  if (!ma) return;
  ma[rowIndex] = md;

  [[NSUserDefaults standardUserDefaults] setObject:ma forKey:@"configList"];

  [[NSNotificationCenter defaultCenter] postNotificationName:kConfigListChangedNotification object:nil];
}

- (void)configlist_append {
  NSMutableArray* ma = nil;

  NSArray* a = [[NSUserDefaults standardUserDefaults] arrayForKey:@"configList"];
  if (a) {
    ma = [NSMutableArray arrayWithArray:a];
  } else {
    ma = [NSMutableArray new];
  }
  if (!ma) return;

  struct timeval tm;
  gettimeofday(&tm, NULL);
  NSString* identifier = [NSString stringWithFormat:@"config_%ld_%ld", (time_t)(tm.tv_sec), (time_t)(tm.tv_usec)];

  NSMutableDictionary* md = [NSMutableDictionary dictionaryWithCapacity:0];
  md[@"name"] = @"NewItem";
  md[@"identify"] = identifier;
  md[@"appendIndex"] = @([self configlist_maxAppendIndex] + 1);

  [ma addObject:md];

  [[NSUserDefaults standardUserDefaults] setObject:ma forKey:@"configList"];

  [[NSNotificationCenter defaultCenter] postNotificationName:kConfigListChangedNotification object:nil];
}

- (void)configlist_delete:(NSInteger)rowIndex {
  NSArray* a = [[NSUserDefaults standardUserDefaults] arrayForKey:@"configList"];
  if (!a) return;

  if (rowIndex < 0 || (NSUInteger)(rowIndex) >= [a count]) return;

  NSInteger selectedIndex = [self configlist_selectedIndex];
  if (rowIndex == selectedIndex) return;

  NSMutableArray* ma = [NSMutableArray arrayWithArray:a];
  if (!ma) return;

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

  [[NSNotificationCenter defaultCenter] postNotificationName:kConfigListChangedNotification object:nil];
}

- (void)configlist_clear_all_values:(NSInteger)rowIndex {
  NSString* identifier = [self configlist_identifier:rowIndex];
  if (!identifier) return;

  [[NSUserDefaults standardUserDefaults] setObject:[NSMutableDictionary new] forKey:identifier];

  [[NSNotificationCenter defaultCenter] postNotificationName:kPreferencesChangedNotification object:nil];
  [clientForKernelspace_ send_config_to_kext];
}

- (NSInteger)configlist_maxAppendIndex {
  NSInteger maxAppendIndex = 0;

  for (NSDictionary* d in [[NSUserDefaults standardUserDefaults] arrayForKey:@"configList"]) {
    if (d[@"appendIndex"]) {
      NSInteger index = [d[@"appendIndex"] integerValue];
      if (maxAppendIndex < index) {
        maxAppendIndex = index;
      }
    }
  }

  return maxAppendIndex;
}

- (void)configlist_sortByAppendIndex {
  NSString* identifier = [self configlist_selectedIdentifier];

  NSArray* a = [[NSUserDefaults standardUserDefaults] arrayForKey:@"configList"];
  NSArray* sorted = [a sortedArrayUsingComparator:^NSComparisonResult(id obj1, id obj2) {
    return [obj1[@"appendIndex"] compare:obj2[@"appendIndex"]];
  }];
  [[NSUserDefaults standardUserDefaults] setObject:sorted forKey:@"configList"];

  [self configlist_selectByIdentifier:identifier];
}

- (void)configlist_sortByName {
  NSString* identifier = [self configlist_selectedIdentifier];

  NSArray* a = [[NSUserDefaults standardUserDefaults] arrayForKey:@"configList"];
  NSArray* sorted = [a sortedArrayUsingComparator:^NSComparisonResult(id obj1, id obj2) {
    return [obj1[@"name"] compare:obj2[@"name"] options:NSCaseInsensitiveSearch];
  }];
  [[NSUserDefaults standardUserDefaults] setObject:sorted forKey:@"configList"];

  [self configlist_selectByIdentifier:identifier];
}

// ----------------------------------------------------------------------
- (BOOL)isCheckForUpdates {
  return [[NSUserDefaults standardUserDefaults] boolForKey:kCheckForUpdates];
}

// ----------------------------------------------------------------------
- (IBAction)sendConfigListChangedNotification:(id)sender {
  [[NSNotificationCenter defaultCenter] postNotificationName:kConfigListChangedNotification object:nil];
}

@end
