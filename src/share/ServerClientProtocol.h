// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@class PreferencesModel;
@class AXNotifierPreferencesModel;
@class SharedCheckboxTree;

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

- (NSInteger)configlist_selectedIndex;
- (NSArray*)configlist_getConfigList;
- (void)configlist_select:(NSInteger)newindex;
- (void)configlist_setName:(NSInteger)rowIndex name:(NSString*)name;
- (void)configlist_append;
- (void)configlist_delete:(NSInteger)rowIndex;
- (void)configlist_clear_all_values:(NSInteger)rowIndex;

- (void)configxml_reload;
- (NSString*)symbolMapName:(NSString*)type value:(NSInteger)value;

- (void)terminateServerProcess;
- (void)relaunch;

// for SharedCheckboxTree
- (SharedCheckboxTree*)sharedCheckboxTree;
- (NSString*)checkboxItemGetName:(NSNumber*)id;
- (NSString*)checkboxItemGetStyle:(NSNumber*)id;
- (NSString*)checkboxItemGetIdentifier:(NSNumber*)id;
- (NSUInteger)checkboxItemGetChildrenCount:(NSNumber*)id;
- (BOOL)checkboxItemNeedsShowCheckbox:(NSNumber*)id;

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
