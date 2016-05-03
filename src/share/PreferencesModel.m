#import "PreferencesModel.h"
#include <sys/time.h>

@implementation ProfileModel

#pragma mark - NSObject

- (id)replacementObjectForPortCoder:(NSPortCoder*)encoder {
  if ([encoder isBycopy]) return self;
  return [super replacementObjectForPortCoder:encoder];
}

#pragma mark - NSCoding

- (instancetype)initWithCoder:(NSCoder*)decoder {
  self = [super init];

  if (self) {
    self.name = [decoder decodeObjectForKey:@"name"];
    self.identifier = [decoder decodeObjectForKey:@"identifier"];
    self.appendIndex = [decoder decodeIntegerForKey:@"appendIndex"];
  }

  return self;
}

- (void)encodeWithCoder:(NSCoder*)encoder {
  [encoder encodeObject:self.name forKey:@"name"];
  [encoder encodeObject:self.identifier forKey:@"identifier"];
  [encoder encodeInteger:self.appendIndex forKey:@"appendIndex"];
}

- (id)copyWithZone:(NSZone*)zone {
  ProfileModel* obj = [[[self class] allocWithZone:zone] init];
  if (obj) {
    obj.name = [self.name copyWithZone:zone];
    obj.identifier = [self.identifier copyWithZone:zone];
    obj.appendIndex = self.appendIndex;
  }
  return obj;
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
#define DECODE(KEY)                               \
  {                                               \
    self.KEY = [decoder decodeBoolForKey:@ #KEY]; \
  }

    DECODE(useAXNotifier);
    DECODE(disableAXNotifierInJavaApps);
    DECODE(disableAXNotifierInQtApps);
    DECODE(disableAXNotifierInPreview);
    DECODE(disableAXNotifierInMicrosoftOffice);
  }

  return self;
}

- (void)encodeWithCoder:(NSCoder*)encoder {
#define ENCODE(KEY)                              \
  {                                              \
    [encoder encodeBool:self.KEY forKey:@ #KEY]; \
  }

  ENCODE(useAXNotifier);
  ENCODE(disableAXNotifierInJavaApps);
  ENCODE(disableAXNotifierInQtApps);
  ENCODE(disableAXNotifierInPreview);
  ENCODE(disableAXNotifierInMicrosoftOffice);
}

- (id)copyWithZone:(NSZone*)zone {
  AXNotifierPreferencesModel* obj = [[[self class] allocWithZone:zone] init];
  if (obj) {
#define COPY(KEY)       \
  {                     \
    obj.KEY = self.KEY; \
  }

    COPY(useAXNotifier);
    COPY(disableAXNotifierInJavaApps);
    COPY(disableAXNotifierInQtApps);
    COPY(disableAXNotifierInPreview);
    COPY(disableAXNotifierInMicrosoftOffice);
  }
  return obj;
}

@end

@implementation PreferencesModel

- (NSInteger)profileMaxAppendIndex {
  NSInteger maxAppendIndex = 0;

  for (ProfileModel* profileModel in self.profiles) {
    if (maxAppendIndex < profileModel.appendIndex) {
      maxAppendIndex = profileModel.appendIndex;
    }
  }

  return maxAppendIndex;
}

- (NSString*)profileSelectedIdentifier {
  if (0 <= self.selectedProfileIndex && self.selectedProfileIndex < (NSInteger)([self.profiles count])) {
    ProfileModel* profileModel = self.profiles[self.selectedProfileIndex];
    return profileModel.identifier;
  } else {
    return nil;
  }
}

- (void)profileSelectByIdentifier:(NSString*)identifier {
  self.selectedProfileIndex = 0;
  if (identifier) {
    int index = 0;
    for (ProfileModel* profileModel in self.profiles) {
      if ([identifier isEqualToString:profileModel.identifier]) {
        self.selectedProfileIndex = index;
        break;
      }
      ++index;
    }
  }
}

- (void)addProfile:(NSString*)name {
  NSMutableArray* profiles = [NSMutableArray arrayWithArray:self.profiles];

  struct timeval tm;
  gettimeofday(&tm, NULL);

  ProfileModel* profileModel = [ProfileModel new];
  profileModel.name = name ? name : @"New Profile";
  profileModel.identifier = [NSString stringWithFormat:@"config_%ld_%ld", (time_t)(tm.tv_sec), (time_t)(tm.tv_usec)];
  profileModel.appendIndex = [self profileMaxAppendIndex] + 1;

  [profiles addObject:profileModel];

  self.profiles = profiles;
}

- (void)renameProfile:(NSInteger)index name:(NSString*)name {
  if (0 <= index && index < (NSInteger)([self.profiles count])) {
    ProfileModel* profileModel = self.profiles[index];
    profileModel.name = name;
  }
}

- (void)deleteProfile:(NSInteger)index {
  if (0 <= index && index < (NSInteger)([self.profiles count])) {
    if (self.selectedProfileIndex != index) {
      NSMutableArray* profiles = [NSMutableArray arrayWithArray:self.profiles];
      [profiles removeObjectAtIndex:index];
      self.profiles = profiles;
    }
  }
}

- (void)sortProfilesByAppendIndex {
  NSString* identifier = [self profileSelectedIdentifier];

  NSArray* sorted = [self.profiles sortedArrayUsingComparator:^NSComparisonResult(ProfileModel* obj1, ProfileModel* obj2) {
    return obj1.appendIndex - obj2.appendIndex;
  }];
  self.profiles = sorted;

  [self profileSelectByIdentifier:identifier];
}

- (void)sortProfilesByName {
  NSString* identifier = [self profileSelectedIdentifier];

  NSArray* sorted = [self.profiles sortedArrayUsingComparator:^NSComparisonResult(ProfileModel* obj1, ProfileModel* obj2) {
    return [obj1.name compare:obj2.name options:NSCaseInsensitiveSearch];
  }];
  self.profiles = sorted;

  [self profileSelectByIdentifier:identifier];
}

@end
