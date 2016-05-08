// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@class AXNotifierPreferencesModel;
@class CheckboxTree;
@class ParameterTree;
@class PreferencesModel;

@protocol ServerClientProtocol

- (NSString*)bundleVersion;

- (bycopy PreferencesModel*)preferencesModel;
- (bycopy AXNotifierPreferencesModel*)axNotifierPreferencesModel;
- (void)savePreferencesModel:(PreferencesModel*)preferencesModel processIdentifier:(int)processIdentifier;

- (void)updateKextValue:(NSString*)name;
- (void)updateKextValues;

- (void)updateStartAtLogin;
- (void)updateStatusBar;
- (void)updateStatusWindow;
- (void)restartAXNotifier;
- (void)unsetDebugFlags;

- (NSString*)symbolMapName:(NSString*)type value:(NSInteger)value;

- (void)terminateServerProcess;
- (void)relaunch;

- (void)checkForUpdatesStableOnly;
- (void)checkForUpdatesWithBetaVersion;

- (void)reloadXML;

- (void)openEventViewer;
- (void)openMultiTouchExtension;
- (void)openPrivateXMLDirectory;
- (void)openSystemPreferencesKeyboard;
- (void)openUninstaller;

- (void)showExampleStatusWindow:(BOOL)visibility;

// for PreferencesWindow
- (bycopy CheckboxTree*)checkboxTree;
- (bycopy ParameterTree*)parameterTree;

// for AXNotifier
- (void)updateFocusedUIElementInformation:(NSDictionary*)information;

// For EventViewer
- (NSArray*)device_information:(NSInteger)type;
- (NSDictionary*)focused_uielement_information;
- (NSArray*)workspace_app_ids;
- (NSArray*)workspace_window_name_ids;
- (NSArray*)workspace_inputsource_ids;

- (NSDictionary*)inputsource_information;

@end
