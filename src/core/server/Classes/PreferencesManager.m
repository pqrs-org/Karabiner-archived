#import "PreferencesManager.h"
#import "ClientForKernelspace.h"
#import "GlobalDomainKeyRepeatObserver.h"
#import "NotificationKeys.h"
#import "PreferencesKeys.h"
#import "PreferencesModel.h"
#import "SharedKeys.h"
#import "XMLCompiler.h"
#include <sys/time.h>

@interface PreferencesManager ()

@property(weak) IBOutlet ClientForKernelspace* clientForKernelspace;
@property(weak) IBOutlet PreferencesModel* preferencesModel;
@property(weak) IBOutlet XMLCompiler* xmlCompiler;
@property NSMutableDictionary* defaults;
@property(copy) NSArray* essential_configuration_identifiers;
@property GlobalDomainKeyRepeatObserver* globalDomainKeyRepeatObserver;

@end

@implementation PreferencesManager

// ----------------------------------------
+ (void)initialize {
  NSDictionary* dict = @{
    kStatusBarEnabled : @YES,
    kShowProfileNameInStatusBar : @NO,
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
    kResumeAtLogin : @YES,
    kKarabinerPreferencesCheckboxFont : @0,
  };
  [[NSUserDefaults standardUserDefaults] registerDefaults:dict];
}

- (void)loadPreferencesModel:(PreferencesModel*)preferencesModel {
  preferencesModel.resumeAtLogin = [[NSUserDefaults standardUserDefaults] boolForKey:kResumeAtLogin];
  preferencesModel.checkForUpdates = [[NSUserDefaults standardUserDefaults] boolForKey:kCheckForUpdates];
  preferencesModel.statusBarEnabled = [[NSUserDefaults standardUserDefaults] boolForKey:kStatusBarEnabled];
  preferencesModel.showProfileNameInStatusBar = [[NSUserDefaults standardUserDefaults] boolForKey:kShowProfileNameInStatusBar];
  preferencesModel.usePreparedSettings = [[NSUserDefaults standardUserDefaults] boolForKey:kUsePreparedSettings];
  preferencesModel.useStatusWindow = [[NSUserDefaults standardUserDefaults] boolForKey:kIsStatusWindowEnabled];
  preferencesModel.showCapsLockStateInStatusWindow = [[NSUserDefaults standardUserDefaults] boolForKey:kIsStatusWindowShowCapsLock];
  preferencesModel.showStickyModifiersStateInStatusWindow = [[NSUserDefaults standardUserDefaults] boolForKey:kIsStatusWindowShowStickyModifier];
  preferencesModel.showPointingButtonLockStateInStatusWindow = [[NSUserDefaults standardUserDefaults] boolForKey:kIsStatusWindowShowPointingButtonLock];
  preferencesModel.statusWindowType = [[NSUserDefaults standardUserDefaults] integerForKey:kStatusWindowType];
  preferencesModel.statusWindowTheme = [[NSUserDefaults standardUserDefaults] integerForKey:kStatusWindowTheme];

  if (!preferencesModel.axNotifierPreferencesModel) {
    preferencesModel.axNotifierPreferencesModel = [AXNotifierPreferencesModel new];
  }
  [self loadAXNotifierPreferencesModel:preferencesModel.axNotifierPreferencesModel];
}

- (void)loadAXNotifierPreferencesModel:(AXNotifierPreferencesModel*)axNotifierPreferencesModel {
  axNotifierPreferencesModel.useAXNotifier = [[NSUserDefaults standardUserDefaults] boolForKey:kIsAXNotifierEnabled];
  axNotifierPreferencesModel.disableAXNotifierInJavaApps = [[NSUserDefaults standardUserDefaults] boolForKey:kAXNotifierDisabledInJavaApps];
  axNotifierPreferencesModel.disableAXNotifierInQtApps = [[NSUserDefaults standardUserDefaults] boolForKey:kAXNotifierDisabledInQtApps];
  axNotifierPreferencesModel.disableAXNotifierInPreview = [[NSUserDefaults standardUserDefaults] boolForKey:kAXNotifierDisabledInPreview];
  axNotifierPreferencesModel.disableAXNotifierInMicrosoftOffice = [[NSUserDefaults standardUserDefaults] boolForKey:kAXNotifierDisabledInMicrosoftOffice];
}

- (void)savePreferencesModel:(PreferencesModel*)preferencesModel processIdentifier:(int)processIdentifier {
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.resumeAtLogin) forKey:kResumeAtLogin];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.checkForUpdates) forKey:kCheckForUpdates];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.statusBarEnabled) forKey:kStatusBarEnabled];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.showProfileNameInStatusBar) forKey:kShowProfileNameInStatusBar];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.usePreparedSettings) forKey:kUsePreparedSettings];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.useStatusWindow) forKey:kIsStatusWindowEnabled];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.showCapsLockStateInStatusWindow) forKey:kIsStatusWindowShowCapsLock];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.showStickyModifiersStateInStatusWindow) forKey:kIsStatusWindowShowStickyModifier];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.showPointingButtonLockStateInStatusWindow) forKey:kIsStatusWindowShowPointingButtonLock];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.statusWindowType) forKey:kStatusWindowType];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.statusWindowTheme) forKey:kStatusWindowTheme];

  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.axNotifierPreferencesModel.useAXNotifier) forKey:kIsAXNotifierEnabled];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.axNotifierPreferencesModel.disableAXNotifierInJavaApps) forKey:kAXNotifierDisabledInJavaApps];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.axNotifierPreferencesModel.disableAXNotifierInQtApps) forKey:kAXNotifierDisabledInQtApps];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.axNotifierPreferencesModel.disableAXNotifierInPreview) forKey:kAXNotifierDisabledInPreview];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.axNotifierPreferencesModel.disableAXNotifierInMicrosoftOffice) forKey:kAXNotifierDisabledInMicrosoftOffice];

  // ----------------------------------------
  // refresh local model.
  if (preferencesModel != self.preferencesModel) {
    [self loadPreferencesModel:self.preferencesModel];
  }

  // ----------------------------------------
  [[NSNotificationCenter defaultCenter] postNotificationName:kPreferencesChangedNotification object:nil];
  [[NSDistributedNotificationCenter defaultCenter] postNotificationName:kKarabinerPreferencesUpdatedNotification
                                                                 object:nil
                                                               userInfo:@{ @"processIdentifier" : @(processIdentifier) }
                                                     deliverImmediately:YES];
}

// ----------------------------------------
- (void)addToDefault:(NSXMLElement*)element {
  for (NSXMLElement* e in [element elementsForName:@"identifier"]) {
    NSXMLNode* attr_default = [e attributeForName:@"default"];
    if (!attr_default) continue;

    self.defaults[[e stringValue]] = @([[attr_default stringValue] intValue]);
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
- (instancetype)init {
  self = [super init];

  if (self) {
    self.defaults = [NSMutableDictionary new];
    [self setDefault];

    self.globalDomainKeyRepeatObserver = [[GlobalDomainKeyRepeatObserver alloc] initWithPreferencesManager:self];

    self.essential_configuration_identifiers = [NSArray arrayWithObjects:
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
      [self configlist_setName:0 name:@"Default Profile"];
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
  [self.globalDomainKeyRepeatObserver start];
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
  NSNumber* number = self.defaults[name];
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
    NSNumber* defaultnumber = self.defaults[name];
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

  for (NSUInteger i = 0, count = [self.essential_configuration_identifiers count]; i < count; ++i) {
    if ([self.essential_configuration_identifiers[i] isEqualToString:name]) {
      bridgeSetConfigOne.isEssentialConfig = 1;
      bridgeSetConfigOne.index = (uint32_t)(i);
      bridgeSetConfigOne.value = (int32_t)(value);
      [self.clientForKernelspace set_config_one:&bridgeSetConfigOne];
      return;
    }
  }

  int config_index = [self.xmlCompiler config_index:name];
  if (config_index >= 0) {
    bridgeSetConfigOne.isEssentialConfig = 0;
    bridgeSetConfigOne.index = (uint32_t)(config_index);
    bridgeSetConfigOne.value = (int32_t)(value);
    [self.clientForKernelspace set_config_one:&bridgeSetConfigOne];
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
          [self.clientForKernelspace send_config_to_kext];
        }
      }
    }
  }
}

- (NSArray*)essential_config {
  NSMutableArray* a = [NSMutableArray new];

  if (self.essential_configuration_identifiers) {
    for (NSString* identifier in self.essential_configuration_identifiers) {
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
  [self.clientForKernelspace send_config_to_kext];
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
  [self configlist_setName:rowIndex name:name notificationUserInfo:nil];
}

- (void)configlist_setName:(NSInteger)rowIndex name:(NSString*)name notificationUserInfo:(NSDictionary*)notificationUserInfo {
  if ([name length] == 0) return;

  NSArray* a = [[NSUserDefaults standardUserDefaults] arrayForKey:@"configList"];
  if (!a) return;
  if (rowIndex < 0 || (NSUInteger)(rowIndex) >= [a count]) return;

  NSDictionary* d = a[rowIndex];
  if (!d) return;

  NSMutableDictionary* md = [NSMutableDictionary dictionaryWithDictionary:d];
  if (!md) return;
  if ([md[@"name"] isEqualToString:name]) {
    return;
  }
  md[@"name"] = name;

  NSMutableArray* ma = [NSMutableArray arrayWithArray:a];
  if (!ma) return;
  ma[rowIndex] = md;

  [[NSUserDefaults standardUserDefaults] setObject:ma forKey:@"configList"];

  [[NSNotificationCenter defaultCenter] postNotificationName:kConfigListChangedNotification object:nil userInfo:notificationUserInfo];
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
  md[@"name"] = @"New Profile";
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
  [self.clientForKernelspace send_config_to_kext];
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

  [[NSNotificationCenter defaultCenter] postNotificationName:kConfigListChangedNotification object:nil];
}

- (void)configlist_sortByName {
  NSString* identifier = [self configlist_selectedIdentifier];

  NSArray* a = [[NSUserDefaults standardUserDefaults] arrayForKey:@"configList"];
  NSArray* sorted = [a sortedArrayUsingComparator:^NSComparisonResult(id obj1, id obj2) {
    return [obj1[@"name"] compare:obj2[@"name"] options:NSCaseInsensitiveSearch];
  }];
  [[NSUserDefaults standardUserDefaults] setObject:sorted forKey:@"configList"];

  [self configlist_selectByIdentifier:identifier];

  [[NSNotificationCenter defaultCenter] postNotificationName:kConfigListChangedNotification object:nil];
}

@end
