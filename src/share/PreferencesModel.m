#import "PreferencesModel.h"
#import "weakify.h"
#include <sys/time.h>

#define DECODE_BOOL(KEY) _##KEY = [decoder decodeBoolForKey:@ #KEY];
#define DECODE_INTEGER(KEY) _##KEY = [decoder decodeIntegerForKey:@ #KEY];
#define DECODE_OBJECT(KEY) _##KEY = [decoder decodeObjectForKey:@ #KEY];

#define ENCODE_BOOL(KEY) [encoder encodeBool:self.KEY forKey:@ #KEY];
#define ENCODE_INTEGER(KEY) [encoder encodeInteger:self.KEY forKey:@ #KEY];
#define ENCODE_OBJECT(KEY) [encoder encodeObject:self.KEY forKey:@ #KEY];

@interface ProfileModel ()

@property(copy, readwrite) NSString* name;
@property(copy, readwrite) NSString* identifier;
@property(readwrite) NSInteger appendIndex;
@property(copy, readwrite) NSDictionary* values;

@end

@implementation ProfileModel

- (instancetype)initWithName:(NSString*)name
                  identifier:(NSString*)identifier
                 appendIndex:(NSInteger)appendIndex
                      values:(NSDictionary*)values {
  self = [super init];

  if (self) {
    _name = name;
    _identifier = identifier;
    _appendIndex = appendIndex;
    _values = values;
  }

  return self;
}

#pragma mark - NSObject

- (id)replacementObjectForPortCoder:(NSPortCoder*)encoder {
  if ([encoder isBycopy]) return self;
  return [super replacementObjectForPortCoder:encoder];
}

#pragma mark - NSCoding

- (instancetype)initWithCoder:(NSCoder*)decoder {
  self = [super init];

  if (self) {
    DECODE_OBJECT(name);
    DECODE_OBJECT(identifier);
    DECODE_INTEGER(appendIndex);
    DECODE_OBJECT(values);
  }

  return self;
}

- (void)encodeWithCoder:(NSCoder*)encoder {
  ENCODE_OBJECT(name);
  ENCODE_OBJECT(identifier);
  ENCODE_INTEGER(appendIndex);
  ENCODE_OBJECT(values);
}

@end

@implementation AXNotifierPreferencesModel

- (void)log {
  NSMutableString* string = [NSMutableString new];
  [string appendString:@"AXNotifierPreferencesModel:\n"];
  [string appendString:@"{\n"];

#define LOG(KEY)                                           \
  {                                                        \
    [string appendFormat:@"    %s: %d\n", #KEY, self.KEY]; \
  }

  LOG(useAXNotifier);
  LOG(disableAXNotifierInJavaApps);
  LOG(disableAXNotifierInQtApps);
  LOG(disableAXNotifierInPreview);
  LOG(disableAXNotifierInMicrosoftOffice);

  [string appendString:@"}\n"];

  NSLog(@"%@", string);
}

#pragma mark - NSObject

- (id)replacementObjectForPortCoder:(NSPortCoder*)encoder {
  if ([encoder isBycopy]) return self;
  return [super replacementObjectForPortCoder:encoder];
}

#pragma mark - NSCoding

- (instancetype)initWithCoder:(NSCoder*)decoder {
  self = [super init];

  if (self) {
    DECODE_BOOL(useAXNotifier);
    DECODE_BOOL(disableAXNotifierInJavaApps);
    DECODE_BOOL(disableAXNotifierInQtApps);
    DECODE_BOOL(disableAXNotifierInPreview);
    DECODE_BOOL(disableAXNotifierInMicrosoftOffice);
  }

  return self;
}

- (void)encodeWithCoder:(NSCoder*)encoder {
  ENCODE_BOOL(useAXNotifier);
  ENCODE_BOOL(disableAXNotifierInJavaApps);
  ENCODE_BOOL(disableAXNotifierInQtApps);
  ENCODE_BOOL(disableAXNotifierInPreview);
  ENCODE_BOOL(disableAXNotifierInMicrosoftOffice);
}

@end

static dispatch_queue_t profileModelQueue_;
static NSDictionary* essentialConfigurationDefaults_ = nil;
static NSArray* essentialConfigurationIdentifiers_ = nil;

@interface PreferencesModel ()

@property(copy, readwrite) NSArray* profiles;

@end

@implementation PreferencesModel

+ (void)initialize {
  static dispatch_once_t once;
  dispatch_once(&once, ^{
    profileModelQueue_ = dispatch_queue_create("org.pqrs.Karabiner.PreferencesModel.profileModelQueue_", NULL);

    essentialConfigurationDefaults_ = @{
#include "../bridge/output/include.bridge_essential_configuration_default_values.m"
    };
    essentialConfigurationIdentifiers_ = @[
#include "../bridge/output/include.bridge_essential_configuration_identifiers.m"
    ];
  });
}

#pragma mark - NSObject

- (id)replacementObjectForPortCoder:(NSPortCoder*)encoder {
  if ([encoder isBycopy]) return self;
  return [super replacementObjectForPortCoder:encoder];
}

#pragma mark - NSCoding

- (instancetype)initWithCoder:(NSCoder*)decoder {
  self = [super init];

  if (self) {
    DECODE_BOOL(resumeAtLogin);
    DECODE_BOOL(checkForUpdates);
    DECODE_BOOL(overrideKeyRepeat);
    DECODE_BOOL(statusBarEnabled);
    DECODE_BOOL(showProfileNameInStatusBar);
    DECODE_BOOL(usePreparedSettings);

    DECODE_OBJECT(profiles);
    DECODE_INTEGER(currentProfileIndex);

    DECODE_BOOL(useStatusWindow);
    DECODE_BOOL(useModifierSymbolsInStatusWindow);
    DECODE_BOOL(showCapsLockStateInStatusWindow);
    DECODE_BOOL(showStickyModifiersStateInStatusWindow);
    DECODE_BOOL(showPointingButtonLockStateInStatusWindow);
    DECODE_INTEGER(statusWindowType);
    DECODE_INTEGER(statusWindowTheme);
    DECODE_INTEGER(statusWindowOpacity);
    DECODE_INTEGER(statusWindowFontSize);
    DECODE_INTEGER(statusWindowPosition);

    DECODE_INTEGER(preferencesCheckboxFont);

    DECODE_OBJECT(axNotifier);
  }

  return self;
}

- (void)encodeWithCoder:(NSCoder*)encoder {
  ENCODE_BOOL(resumeAtLogin);
  ENCODE_BOOL(checkForUpdates);
  ENCODE_BOOL(overrideKeyRepeat);
  ENCODE_BOOL(statusBarEnabled);
  ENCODE_BOOL(showProfileNameInStatusBar);
  ENCODE_BOOL(usePreparedSettings);

  ENCODE_OBJECT(profiles);
  ENCODE_INTEGER(currentProfileIndex);

  ENCODE_BOOL(useStatusWindow);
  ENCODE_BOOL(useModifierSymbolsInStatusWindow);
  ENCODE_BOOL(showCapsLockStateInStatusWindow);
  ENCODE_BOOL(showStickyModifiersStateInStatusWindow);
  ENCODE_BOOL(showPointingButtonLockStateInStatusWindow);
  ENCODE_INTEGER(statusWindowType);
  ENCODE_INTEGER(statusWindowTheme);
  ENCODE_INTEGER(statusWindowOpacity);
  ENCODE_INTEGER(statusWindowFontSize);
  ENCODE_INTEGER(statusWindowPosition);

  ENCODE_INTEGER(preferencesCheckboxFont);

  ENCODE_OBJECT(axNotifier);
}

- (NSInteger)value:(NSString*)name {
  if ([name length] == 0) {
    return 0;
  }

  ProfileModel* profileModel = [self profile:self.currentProfileIndex];
  if (!profileModel) {
    return 0;
  }

  NSNumber* value = profileModel.values[name];
  if (!value) {
    return [essentialConfigurationDefaults_[name] integerValue];
  }

  return [value integerValue];
}

- (BOOL)setValue:(NSInteger)value forName:(NSString*)name {
  @weakify(self);

  __block BOOL result = NO;

  dispatch_sync(profileModelQueue_, ^{
    @strongify(self);
    if (!self) return;

    if ([name length] > 0) {
      ProfileModel* profileModel = [self profile:self.currentProfileIndex];
      if (profileModel) {
        if ([profileModel.values[name] integerValue] != value) {
          NSMutableDictionary* values = [profileModel.values mutableCopy];
          if (value == [essentialConfigurationDefaults_[name] integerValue]) {
            [values removeObjectForKey:name];
          } else {
            values[name] = @(value);
          }

          profileModel.values = values;
          result = YES;
        }
      }
    }
  });

  return result;
}

- (NSInteger)defaultValue:(NSString*)identifier {
  if ([identifier length] == 0) {
    return 0;
  }
  return [essentialConfigurationDefaults_[identifier] integerValue];
}

- (void)clearNotSave {
  @weakify(self);

  dispatch_sync(profileModelQueue_, ^{
    @strongify(self);
    if (!self) return;

    for (ProfileModel* profileModel in self.profiles) {
      NSMutableDictionary* values = [profileModel.values mutableCopy];
      for (NSString* name in [values allKeys]) {
        if ([name hasPrefix:@"notsave."]) {
          [values removeObjectForKey:name];
        }
      }
      profileModel.values = values;
    }
  });
}

- (void)clearValues:(NSInteger)profileIndex {
  @weakify(self);

  dispatch_sync(profileModelQueue_, ^{
    @strongify(self);
    if (!self) return;

    if (0 <= profileIndex && profileIndex < (NSInteger)([self.profiles count])) {
      ProfileModel* profileModel = self.profiles[profileIndex];
      profileModel.values = @{};
    }
  });
}

- (NSArray*)essentialConfigurations {
  NSMutableArray* essentialConfigurations = [NSMutableArray new];

  for (NSString* identifier in essentialConfigurationIdentifiers_) {
    [essentialConfigurations addObject:@([self value:identifier])];
  }

  return essentialConfigurations;
}

- (NSInteger)essentialConfigurationIndex:(NSString*)identifier {
  NSInteger index = 0;
  for (NSString* key in essentialConfigurationIdentifiers_) {
    if ([key isEqualToString:identifier]) {
      return index;
    }
    ++index;
  }

  return -1;
}

- (NSInteger)profileMaxAppendIndex {
  NSInteger maxAppendIndex = 0;

  for (ProfileModel* profileModel in self.profiles) {
    if (maxAppendIndex < profileModel.appendIndex) {
      maxAppendIndex = profileModel.appendIndex;
    }
  }

  return maxAppendIndex;
}

- (ProfileModel*)currentProfile {
  return [self profile:self.currentProfileIndex];
}

- (NSString*)currentProfileName {
  ProfileModel* profileModel = [self profile:self.currentProfileIndex];
  if (profileModel) {
    return profileModel.name;
  } else {
    return nil;
  }
}

- (NSString*)currentProfileIdentifier {
  ProfileModel* profileModel = [self profile:self.currentProfileIndex];
  if (profileModel) {
    return profileModel.identifier;
  } else {
    return nil;
  }
}

- (void)profileSelectByIdentifier:(NSString*)identifier {
  self.currentProfileIndex = 0;
  int index = 0;
  for (ProfileModel* profileModel in self.profiles) {
    if ([identifier isEqualToString:profileModel.identifier]) {
      self.currentProfileIndex = index;
      break;
    }
    ++index;
  }
}

- (ProfileModel*)profile:(NSInteger)index {
  if (0 <= index && index < (NSInteger)([self.profiles count])) {
    return self.profiles[index];
  }
  return nil;
}

- (NSInteger)profileIndexByName:(NSString*)name {
  int index = 0;
  for (ProfileModel* profileModel in self.profiles) {
    if ([name isEqualToString:profileModel.name]) {
      return index;
    }
    ++index;
  }
  return -1;
}

- (void)replaceProfiles:(NSArray*)profiles {
  @weakify(self);

  dispatch_sync(profileModelQueue_, ^{
    @strongify(self);
    if (!self) return;

    self.profiles = profiles;
  });
}

- (void)addProfile:(NSString*)name {
  @weakify(self);

  dispatch_sync(profileModelQueue_, ^{
    @strongify(self);
    if (!self) return;

    NSMutableArray* profiles = [NSMutableArray arrayWithArray:self.profiles];

    struct timeval tm;
    gettimeofday(&tm, NULL);

    ProfileModel* profileModel = [ProfileModel new];
    profileModel.name = name ? name : @"New Profile";
    profileModel.identifier = [NSString stringWithFormat:@"config_%ld_%ld", (time_t)(tm.tv_sec), (time_t)(tm.tv_usec)];
    profileModel.appendIndex = [self profileMaxAppendIndex] + 1;
    profileModel.values = @{};

    [profiles addObject:profileModel];

    self.profiles = profiles;
  });
}

- (void)renameProfile:(NSInteger)index name:(NSString*)name {
  @weakify(self);

  dispatch_sync(profileModelQueue_, ^{
    @strongify(self);
    if (!self) return;

    ProfileModel* profileModel = [self profile:index];
    if (profileModel) {
      profileModel.name = name;
    }
  });
}

- (void)deleteProfile:(NSInteger)index {
  @weakify(self);

  dispatch_sync(profileModelQueue_, ^{
    @strongify(self);
    if (!self) return;

    if (self.currentProfileIndex != index) {
      ProfileModel* profileModel = [self profile:index];
      if (profileModel) {
        NSString* identifier = self.currentProfileIdentifier;

        NSMutableArray* profiles = [NSMutableArray arrayWithArray:self.profiles];
        [profiles removeObjectAtIndex:index];
        self.profiles = profiles;

        [self profileSelectByIdentifier:identifier];
      }
    }
  });
}

- (void)sortProfilesByAppendIndex {
  @weakify(self);

  dispatch_sync(profileModelQueue_, ^{
    @strongify(self);
    if (!self) return;

    // get current identifier before sort.
    NSString* identifier = self.currentProfileIdentifier;

    NSArray* sorted = [self.profiles sortedArrayUsingComparator:^NSComparisonResult(ProfileModel* obj1, ProfileModel* obj2) {
      return obj1.appendIndex - obj2.appendIndex;
    }];
    self.profiles = sorted;

    [self profileSelectByIdentifier:identifier];
  });
}

- (void)sortProfilesByName {
  @weakify(self);

  dispatch_sync(profileModelQueue_, ^{
    @strongify(self);
    if (!self) return;

    // get current identifier before sort.
    NSString* identifier = self.currentProfileIdentifier;

    NSArray* sorted = [self.profiles sortedArrayUsingComparator:^NSComparisonResult(ProfileModel* obj1, ProfileModel* obj2) {
      return [obj1.name compare:obj2.name options:NSCaseInsensitiveSearch];
    }];
    self.profiles = sorted;

    [self profileSelectByIdentifier:identifier];
  });
}

@end
