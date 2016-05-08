// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@interface ProfileModel : NSObject <NSCoding>

@property(copy) NSString* name;
@property(copy) NSString* identifier;
@property NSInteger appendIndex;
@property(copy) NSDictionary* values;

@end

@interface AXNotifierPreferencesModel : NSObject <NSCoding>

@property BOOL useAXNotifier;
@property BOOL disableAXNotifierInJavaApps;
@property BOOL disableAXNotifierInQtApps;
@property BOOL disableAXNotifierInPreview;
@property BOOL disableAXNotifierInMicrosoftOffice;

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
- (void)setValue:(NSInteger)value forName:(NSString*)name;
- (void)clearNotSave;
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
