// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@protocol KarabinerProtocol
- (int)value:(NSString *)name;
- (int)defaultValue:(NSString *)name;
- (void)setValue:(int)newval forName:(NSString *)name;

- (NSDictionary *)changed;

- (NSInteger)configlist_selectedIndex;
- (NSArray *)configlist_getConfigList;
- (void)configlist_select:(NSInteger)newindex;
- (void)configlist_setName:(NSInteger)rowIndex name:(NSString *)name;
- (void)configlist_append;
- (void)configlist_delete:(NSInteger)rowIndex;
- (void)configlist_clear_all_values:(NSInteger)rowIndex;

- (void)configxml_reload;
- (NSString *)symbolMapName:(NSString *)type value:(NSInteger)value;

- (void)relaunch;

// for AXNotifier
- (NSDictionary *)preferencesForAXNotifier;
- (void)updateFocusedUIElementInformation:(NSDictionary *)information;

// For EventViewer.
- (NSArray *)device_information:(NSInteger)type;
- (NSDictionary *)focused_uielement_information;
- (NSArray *)workspace_app_ids;
- (NSArray *)workspace_window_name_ids;
- (NSArray *)workspace_inputsource_ids;
;
- (NSDictionary *)inputsource_information;

@end
