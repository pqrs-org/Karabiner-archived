#import "PreferencesManager.h"
#import "ClientForKernelspace.h"
#import "NotificationKeys.h"
#import "PreferencesKeys.h"
#import "PreferencesModel.h"
#import "SharedKeys.h"
#import "XMLCompiler.h"

@interface PreferencesManager ()

@property(weak) IBOutlet ClientForKernelspace* clientForKernelspace;
@property(weak) IBOutlet PreferencesModel* preferencesModel;
@property(weak) IBOutlet XMLCompiler* xmlCompiler;

@end

@implementation PreferencesManager

// ----------------------------------------
+ (void)initialize {
  static dispatch_once_t once;
  dispatch_once(&once, ^{
    NSDictionary* dict = @{
      kStatusBarEnabled : @YES,
      kShowProfileNameInStatusBar : @NO,
      kProfiles : @[],
      kCurrentProfileIndex : @0,
      kCheckForUpdates : @YES,
      kIsStatusWindowEnabled : @YES,
      kIsStatusWindowUseModifierSymbols : @NO,
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
      kAXNotifierEnableDebuggingLog : @NO,
      kUsePreparedSettings : @YES,
      kIsMigratedIsOverwriteKeyRepeat : @NO,
      kIsOverrideKeyRepeat : @NO,
      kShowIconInDock : @NO,
      kResumeAtLogin : @YES,
      kKarabinerPreferencesCheckboxFont : @0,
    };
    [[NSUserDefaults standardUserDefaults] registerDefaults:dict];
  });
}

- (void)loadPreferencesModel:(PreferencesModel*)preferencesModel {
  preferencesModel.resumeAtLogin = [[NSUserDefaults standardUserDefaults] boolForKey:kResumeAtLogin];
  preferencesModel.checkForUpdates = [[NSUserDefaults standardUserDefaults] boolForKey:kCheckForUpdates];
  preferencesModel.overrideKeyRepeat = [[NSUserDefaults standardUserDefaults] boolForKey:kIsOverrideKeyRepeat];
  preferencesModel.statusBarEnabled = [[NSUserDefaults standardUserDefaults] boolForKey:kStatusBarEnabled];
  preferencesModel.showProfileNameInStatusBar = [[NSUserDefaults standardUserDefaults] boolForKey:kShowProfileNameInStatusBar];
  preferencesModel.usePreparedSettings = [[NSUserDefaults standardUserDefaults] boolForKey:kUsePreparedSettings];
  preferencesModel.currentProfileIndex = [[NSUserDefaults standardUserDefaults] integerForKey:kCurrentProfileIndex];
  preferencesModel.useStatusWindow = [[NSUserDefaults standardUserDefaults] boolForKey:kIsStatusWindowEnabled];
  preferencesModel.useModifierSymbolsInStatusWindow = [[NSUserDefaults standardUserDefaults] boolForKey:kIsStatusWindowUseModifierSymbols];
  preferencesModel.showCapsLockStateInStatusWindow = [[NSUserDefaults standardUserDefaults] boolForKey:kIsStatusWindowShowCapsLock];
  preferencesModel.showStickyModifiersStateInStatusWindow = [[NSUserDefaults standardUserDefaults] boolForKey:kIsStatusWindowShowStickyModifier];
  preferencesModel.showPointingButtonLockStateInStatusWindow = [[NSUserDefaults standardUserDefaults] boolForKey:kIsStatusWindowShowPointingButtonLock];
  preferencesModel.statusWindowType = [[NSUserDefaults standardUserDefaults] integerForKey:kStatusWindowType];
  preferencesModel.statusWindowTheme = [[NSUserDefaults standardUserDefaults] integerForKey:kStatusWindowTheme];
  preferencesModel.statusWindowOpacity = [[NSUserDefaults standardUserDefaults] integerForKey:kStatusWindowOpacity];
  preferencesModel.statusWindowFontSize = [[NSUserDefaults standardUserDefaults] integerForKey:kStatusWindowFontSize];
  preferencesModel.statusWindowPosition = [[NSUserDefaults standardUserDefaults] integerForKey:kStatusWindowPosition];
  preferencesModel.preferencesCheckboxFont = [[NSUserDefaults standardUserDefaults] integerForKey:kKarabinerPreferencesCheckboxFont];

  // ----------------------------------------
  // profiles
  NSMutableArray* profiles = [NSMutableArray new];
  for (NSDictionary* profile in [[NSUserDefaults standardUserDefaults] arrayForKey:kProfiles]) {
    NSString* name = profile[@"name"];
    NSString* identifier = profile[@"identify"];
    NSInteger appendIndex = [profile[@"appendIndex"] integerValue];
    NSDictionary* values = [[NSUserDefaults standardUserDefaults] dictionaryForKey:identifier];
    ProfileModel* profileModel = [[ProfileModel alloc] initWithName:name
                                                         identifier:identifier
                                                        appendIndex:appendIndex
                                                             values:values];
    [profiles addObject:profileModel];
  }
  preferencesModel.profiles = profiles;

  // ----------------------------------------
  // axNotifier
  preferencesModel.axNotifier = [AXNotifierPreferencesModel new];
  preferencesModel.axNotifier.useAXNotifier = [[NSUserDefaults standardUserDefaults] boolForKey:kIsAXNotifierEnabled];
  preferencesModel.axNotifier.disabledInJavaApps = [[NSUserDefaults standardUserDefaults] boolForKey:kAXNotifierDisabledInJavaApps];
  preferencesModel.axNotifier.disabledInQtApps = [[NSUserDefaults standardUserDefaults] boolForKey:kAXNotifierDisabledInQtApps];
  preferencesModel.axNotifier.disabledInPreview = [[NSUserDefaults standardUserDefaults] boolForKey:kAXNotifierDisabledInPreview];
  preferencesModel.axNotifier.disabledInMicrosoftOffice = [[NSUserDefaults standardUserDefaults] boolForKey:kAXNotifierDisabledInMicrosoftOffice];
  preferencesModel.axNotifier.debuggingLogEnabled = [[NSUserDefaults standardUserDefaults] boolForKey:kAXNotifierEnableDebuggingLog];
}

- (NSString*)savedProfileIdentifier {
  NSArray* profiles = [[NSUserDefaults standardUserDefaults] arrayForKey:kProfiles];
  NSInteger index = [[NSUserDefaults standardUserDefaults] integerForKey:kCurrentProfileIndex];
  if (0 <= index && index < (NSInteger)([profiles count])) {
    NSDictionary* profile = profiles[index];
    return profile[@"identify"];
  }
  return nil;
}

- (void)savePreferencesModel:(PreferencesModel*)preferencesModel processIdentifier:(int)processIdentifier {
  // We should run `savePreferencesModel` in the main thread because `savePreferencesModel` calls `loadPreferencesModel` internally.
  // We should touch self.preferencesModel only in the main thread to avoid race condition.

  if (![NSThread isMainThread]) {
    NSLog(@"WARNING [PreferencesManager savePreferencesModel] is not running in main thread.");
  }

  if (!preferencesModel) {
    return;
  }

  // ----------------------------------------
  NSString* oldProfileIdentifier = [self savedProfileIdentifier];

  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.resumeAtLogin) forKey:kResumeAtLogin];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.checkForUpdates) forKey:kCheckForUpdates];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.overrideKeyRepeat) forKey:kIsOverrideKeyRepeat];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.statusBarEnabled) forKey:kStatusBarEnabled];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.showProfileNameInStatusBar) forKey:kShowProfileNameInStatusBar];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.usePreparedSettings) forKey:kUsePreparedSettings];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.currentProfileIndex) forKey:kCurrentProfileIndex];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.useStatusWindow) forKey:kIsStatusWindowEnabled];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.useModifierSymbolsInStatusWindow) forKey:kIsStatusWindowUseModifierSymbols];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.showCapsLockStateInStatusWindow) forKey:kIsStatusWindowShowCapsLock];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.showStickyModifiersStateInStatusWindow) forKey:kIsStatusWindowShowStickyModifier];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.showPointingButtonLockStateInStatusWindow) forKey:kIsStatusWindowShowPointingButtonLock];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.statusWindowType) forKey:kStatusWindowType];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.statusWindowTheme) forKey:kStatusWindowTheme];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.statusWindowOpacity) forKey:kStatusWindowOpacity];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.statusWindowFontSize) forKey:kStatusWindowFontSize];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.statusWindowPosition) forKey:kStatusWindowPosition];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.preferencesCheckboxFont) forKey:kKarabinerPreferencesCheckboxFont];

  // ----------------------------------------
  // profiles
  NSMutableArray* profiles = [NSMutableArray new];
  for (ProfileModel* profileModel in preferencesModel.profiles) {
    [profiles addObject:@{
      @"name" : profileModel.name ? profileModel.name : @"",
      @"identify" : profileModel.identifier ? profileModel.identifier : @"",
      @"appendIndex" : @(profileModel.appendIndex),
    }];

    if (profileModel.identifier && profileModel.values) {
      [[NSUserDefaults standardUserDefaults] setObject:profileModel.values forKey:profileModel.identifier];
    }
  }
  [[NSUserDefaults standardUserDefaults] setObject:profiles forKey:kProfiles];

  // ----------------------------------------
  // axNotifier
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.axNotifier.useAXNotifier) forKey:kIsAXNotifierEnabled];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.axNotifier.disabledInJavaApps) forKey:kAXNotifierDisabledInJavaApps];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.axNotifier.disabledInQtApps) forKey:kAXNotifierDisabledInQtApps];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.axNotifier.disabledInPreview) forKey:kAXNotifierDisabledInPreview];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.axNotifier.disabledInMicrosoftOffice) forKey:kAXNotifierDisabledInMicrosoftOffice];
  [[NSUserDefaults standardUserDefaults] setObject:@(preferencesModel.axNotifier.debuggingLogEnabled) forKey:kAXNotifierEnableDebuggingLog];

  // ----------------------------------------
  // refresh local model.
  if (preferencesModel != self.preferencesModel) {
    [self loadPreferencesModel:self.preferencesModel];
  }

  // ----------------------------------------
  // post notifications
  [[NSNotificationCenter defaultCenter] postNotificationName:kPreferencesChangedNotification object:nil];
  [[NSDistributedNotificationCenter defaultCenter] postNotificationName:kKarabinerPreferencesUpdatedNotification
                                                                 object:nil
                                                               userInfo:@{ @"processIdentifier" : @(processIdentifier) }
                                                     deliverImmediately:YES];

  if (![self.preferencesModel.currentProfileIdentifier isEqualToString:oldProfileIdentifier]) {
    [[NSNotificationCenter defaultCenter] postNotificationName:kProfileChangedNotification object:nil];
  }
}

// ----------------------------------------
- (void)save {
  [self savePreferencesModel:self.preferencesModel processIdentifier:[NSProcessInfo processInfo].processIdentifier];
}

// ----------------------------------------------------------------------
- (void)updateKextValue:(NSString*)name {
  struct BridgeSetConfigOne bridgeSetConfigOne;

  NSInteger essentialConfigurationIndex = [self.preferencesModel essentialConfigurationIndex:name];
  if (essentialConfigurationIndex >= 0) {
    bridgeSetConfigOne.isEssentialConfig = 1;
    bridgeSetConfigOne.index = (uint32_t)(essentialConfigurationIndex);
    bridgeSetConfigOne.value = (int32_t)([self.preferencesModel value:name]);
    [self.clientForKernelspace set_config_one:&bridgeSetConfigOne];
    return;
  }

  int config_index = [self.xmlCompiler config_index:name];
  if (config_index >= 0) {
    bridgeSetConfigOne.isEssentialConfig = 0;
    bridgeSetConfigOne.index = (uint32_t)(config_index);
    bridgeSetConfigOne.value = (int32_t)([self.preferencesModel value:name]);
    [self.clientForKernelspace set_config_one:&bridgeSetConfigOne];
    return;
  }
}

@end
