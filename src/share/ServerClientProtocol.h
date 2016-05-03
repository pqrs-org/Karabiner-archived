// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@class PreferencesModel;
@class AXNotifierPreferencesModel;
@class SharedXMLCompilerTree;

@protocol ServerClientProtocol

- (NSString*)bundleVersion;

- (void)loadPreferencesModel:(PreferencesModel*)preferencesModel;
- (void)loadAXNotifierPreferencesModel:(AXNotifierPreferencesModel*)axNotifierPreferencesModel;
- (void)savePreferencesModel:(PreferencesModel*)preferencesModel processIdentifier:(int)processIdentifier;
- (void)updateStartAtLogin;
- (void)updateStatusBar;
- (void)updateStatusWindow;
- (void)restartAXNotifier;

- (int)value:(NSString*)name;
- (int)defaultValue:(NSString*)name;
- (void)setValue:(int)newval forName:(NSString*)name;

- (NSDictionary*)changed;

- (NSArray*)configlist_getConfigList;
- (void)configlist_setName:(NSInteger)rowIndex name:(NSString*)name;
- (void)configlist_delete:(NSInteger)rowIndex;
- (void)configlist_clear_all_values:(NSInteger)rowIndex;

- (void)configxml_reload;
- (NSString*)symbolMapName:(NSString*)type value:(NSInteger)value;

- (void)terminateServerProcess;
- (void)relaunch;

- (void)openEventViewer;
- (void)openMultiTouchExtension;
- (void)openPrivateXMLDirectory;
- (void)openSystemPreferencesKeyboard;
- (void)openUninstaller;

// for SharedXMLCompilerTree
- (SharedXMLCompilerTree*)sharedCheckboxTree;
- (SharedXMLCompilerTree*)narrowedSharedCheckboxTree:(BOOL)isEnabledOnly strings:(NSArray*)strings;
- (NSString*)checkboxItemGetName:(NSNumber*)id;
- (NSString*)checkboxItemGetStyle:(NSNumber*)id;
- (NSString*)checkboxItemGetIdentifier:(NSNumber*)id;
- (NSUInteger)checkboxItemGetChildrenCount:(NSNumber*)id;
- (BOOL)checkboxItemNeedsShowCheckbox:(NSNumber*)id;

- (SharedXMLCompilerTree*)sharedParameterTree;
- (NSString*)parameterItemGetName:(NSNumber*)id;
- (NSString*)parameterItemGetIdentifier:(NSNumber*)id;
- (NSInteger)parameterItemGetDefaultValue:(NSNumber*)id;
- (NSInteger)parameterItemGetStep:(NSNumber*)id;
- (NSString*)parameterItemGetBaseUnit:(NSNumber*)id;
- (NSUInteger)parameterItemGetChildrenCount:(NSNumber*)id;

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
