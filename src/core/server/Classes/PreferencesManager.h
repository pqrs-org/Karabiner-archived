// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@class PreferencesModel;

@interface PreferencesManager : NSObject

- (void)loadPreferencesModel:(PreferencesModel*)preferencesModel;
- (void)savePreferencesModel:(PreferencesModel*)preferencesModel processIdentifier:(int)processIdentifier;

- (void)load;

- (int)value:(NSString*)name;
- (int)defaultValue:(NSString*)name;
- (void)setValue:(int)newval forName:(NSString*)name;
- (void)setValue:(int)newval forName:(NSString*)name tellToKext:(BOOL)tellToKext;
- (void)setValue:(int)newval forName:(NSString*)name tellToKext:(BOOL)tellToKext notificationUserInfo:(NSDictionary*)notificationUserInfo;
- (void)clearNotSave;

- (NSArray*)essential_config;
- (NSDictionary*)changed;

// --------------------------------------------------
- (NSInteger)configlist_selectedIndex;
- (NSString*)configlist_selectedName;
- (NSString*)configlist_selectedIdentifier;
- (NSArray*)configlist_getConfigList;
- (NSUInteger)configlist_count;
- (NSDictionary*)configlist_dictionary:(NSInteger)rowIndex;
- (NSString*)configlist_name:(NSInteger)rowIndex;
- (NSString*)configlist_identifier:(NSInteger)rowIndex;
- (void)configlist_select:(NSInteger)newindex;
- (void)configlist_setName:(NSInteger)rowIndex name:(NSString*)name;
- (void)configlist_setName:(NSInteger)rowIndex name:(NSString*)name notificationUserInfo:(NSDictionary*)notificationUserInfo;
- (void)configlist_append;
- (void)configlist_delete:(NSInteger)rowIndex;
- (void)configlist_clear_all_values:(NSInteger)rowIndex;
- (void)configlist_sortByAppendIndex;
- (void)configlist_sortByName;

@end
