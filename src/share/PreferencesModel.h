// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@interface ProfileModel : NSObject <NSCoding>

@property(copy, readonly) NSString* name;
@property(copy, readonly) NSString* identifier;
@property(readonly) NSInteger appendIndex;
@property(copy, readonly) NSDictionary* values;

- (instancetype)initWithName:(NSString*)name
                  identifier:(NSString*)identifier
                 appendIndex:(NSInteger)appendIndex
                      values:(NSDictionary*)values;

@end

@interface AXNotifierPreferencesModel : NSObject <NSCoding>

@property BOOL useAXNotifier;
@property BOOL disabledInJavaApps;
@property BOOL disabledInQtApps;
@property BOOL disabledInPreview;
@property BOOL disabledInMicrosoftOffice;
@property BOOL debuggingLogEnabled;

- (void)log;

@end

@interface PreferencesModel : NSObject <NSCoding>

@property BOOL resumeAtLogin;
@property BOOL checkForUpdates;
@property BOOL overrideKeyRepeat;
@property BOOL statusBarEnabled;
@property BOOL showProfileNameInStatusBar;
@property BOOL usePreparedSettings;

@property(copy) NSArray* profiles;
@property NSInteger currentProfileIndex;

@property BOOL useStatusWindow;
@property BOOL useModifierSymbolsInStatusWindow;
@property BOOL showCapsLockStateInStatusWindow;
@property BOOL showStickyModifiersStateInStatusWindow;
@property BOOL showPointingButtonLockStateInStatusWindow;
@property NSInteger statusWindowType;
@property NSInteger statusWindowTheme;
@property NSInteger statusWindowOpacity;
@property NSInteger statusWindowFontSize;
@property NSInteger statusWindowPosition;

@property NSInteger preferencesCheckboxFont;

@property AXNotifierPreferencesModel* axNotifier;

// readonly method
@property(readonly) ProfileModel* currentProfile;
@property(copy, readonly) NSString* currentProfileName;
@property(copy, readonly) NSString* currentProfileIdentifier;

- (NSInteger)value:(NSString*)name;
// return YES if the configuration was changed.
- (BOOL)setValue:(NSInteger)value forName:(NSString*)name;
- (NSInteger)defaultValue:(NSString*)identifier;
- (void)clearNotSave;
- (void)clearValues:(NSInteger)profileIndex;
@property(copy, readonly) NSArray* essentialConfigurations;
- (NSInteger)essentialConfigurationIndex:(NSString*)identifier;

- (ProfileModel*)profile:(NSInteger)index;
- (NSInteger)profileIndexByName:(NSString*)name;
- (void)addProfile:(NSString*)name;
- (void)renameProfile:(NSInteger)index name:(NSString*)name;
- (void)deleteProfile:(NSInteger)index;
- (void)sortProfilesByAppendIndex;
- (void)sortProfilesByName;

@end
