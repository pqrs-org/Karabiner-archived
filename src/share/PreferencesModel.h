// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@interface PreferencesModel : NSObject

@property BOOL resumeAtLogin;
@property BOOL checkForUpdates;
@property BOOL statusBarEnabled;
@property BOOL showProfileNameInStatusBar;
@property BOOL usePreparedSettings;

@property BOOL useAXNotifier;
@property BOOL disableAXNotifierInJavaApps;
@property BOOL disableAXNotifierInQtApps;
@property BOOL disableAXNotifierInPreview;
@property BOOL disableAXNotifierInMicrosoftOffice;

@property BOOL useStatusWindow;

@end
