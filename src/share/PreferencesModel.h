// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@interface ProfileModel : NSObject <NSCoding, NSCopying>

@property(copy) NSString* name;
@property(copy) NSString* identifier;
@property NSInteger appendIndex;

@end

@interface AXNotifierPreferencesModel : NSObject <NSCoding, NSCopying>

@property BOOL useAXNotifier;
@property BOOL disableAXNotifierInJavaApps;
@property BOOL disableAXNotifierInQtApps;
@property BOOL disableAXNotifierInPreview;
@property BOOL disableAXNotifierInMicrosoftOffice;

- (void)log;

@end

@interface PreferencesModel : NSObject

@property BOOL resumeAtLogin;
@property BOOL checkForUpdates;
@property BOOL overrideKeyRepeat;
@property BOOL statusBarEnabled;
@property BOOL showProfileNameInStatusBar;
@property BOOL usePreparedSettings;

@property(copy) NSArray* profiles;
@property NSInteger currentProfileIndex;
@property(copy, readonly) NSString* currentProfileIdentifier;

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

@property AXNotifierPreferencesModel* axNotifierPreferencesModel;

- (ProfileModel*)profile:(NSInteger)index;
- (void)addProfile:(NSString*)name;
- (void)renameProfile:(NSInteger)index name:(NSString*)name;
- (void)deleteProfile:(NSInteger)index;
- (void)sortProfilesByAppendIndex;
- (void)sortProfilesByName;

@end
