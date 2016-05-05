// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@class AXNotifierPreferencesModel;
@class CheckboxTree;
@class ParameterTree;
@class PreferencesModel;

@protocol ServerClientProtocol

- (NSString*)bundleVersion;

- (void)loadPreferencesModel:(PreferencesModel*)preferencesModel;
- (void)savePreferencesModel:(PreferencesModel*)preferencesModel processIdentifier:(int)processIdentifier;
- (void)loadAXNotifierPreferencesModel:(AXNotifierPreferencesModel*)axNotifierPreferencesModel;
- (void)saveAXNotifierPreferencesModel:(AXNotifierPreferencesModel*)axNotifierPreferencesModel processIdentifier:(int)processIdentifier;
- (void)updateStartAtLogin;
- (void)updateStatusBar;
- (void)updateStatusWindow;
- (void)restartAXNotifier;
- (void)unsetDebugFlags;

- (int)value:(NSString*)name;
- (int)defaultValue:(NSString*)name;
- (void)setValue:(int)newval forName:(NSString*)name;

- (NSDictionary*)changed;

- (void)configlist_clear_all_values:(NSInteger)rowIndex;

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

// for SharedXMLCompilerTree
- (bycopy CheckboxTree*)checkboxTree;
- (bycopy ParameterTree*)parameterTree;

// for AXNotifier
- (void)updateFocusedUIElementInformation:(NSDictionary*)information;

// For EventViewer.
- (NSArray*)device_information:(NSInteger)type;
- (NSDictionary*)focused_uielement_information;
- (NSArray*)workspace_app_ids;
- (NSArray*)workspace_window_name_ids;
- (NSArray*)workspace_inputsource_ids;

- (NSDictionary*)inputsource_information;

@end
