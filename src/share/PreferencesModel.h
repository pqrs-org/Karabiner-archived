// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

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
@property BOOL statusBarEnabled;
@property BOOL showProfileNameInStatusBar;
@property BOOL usePreparedSettings;

@property BOOL useStatusWindow;
@property BOOL showCapsLockStateInStatusWindow;
@property BOOL showStickyModifiersStateInStatusWindow;
@property BOOL showPointingButtonLockStateInStatusWindow;
@property NSInteger statusWindowType;

@property AXNotifierPreferencesModel* axNotifierPreferencesModel;

@end
