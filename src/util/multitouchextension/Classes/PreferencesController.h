// -*- Mode: objc -*-

@import Cocoa;

@interface PreferencesController : NSObject

- (void)load;
- (void)show;
+ (BOOL)isSettingEnabled:(NSInteger)fingers;
+ (NSString*)getSettingName:(NSInteger)fingers;

@end
