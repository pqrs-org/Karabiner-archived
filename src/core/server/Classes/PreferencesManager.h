// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@class PreferencesModel;
@class AXNotifierPreferencesModel;

@interface PreferencesManager : NSObject

- (void)loadPreferencesModel:(PreferencesModel*)preferencesModel;
- (void)savePreferencesModel:(PreferencesModel*)preferencesModel processIdentifier:(int)processIdentifier;
- (void)loadAXNotifierPreferencesModel:(AXNotifierPreferencesModel*)axNotifierPreferencesModel;
- (void)saveAXNotifierPreferencesModel:(AXNotifierPreferencesModel*)axNotifierPreferencesModel processIdentifier:(int)processIdentifier;

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
- (NSString*)configlist_selectedIdentifier;
- (NSDictionary*)configlist_dictionary:(NSInteger)rowIndex;
- (NSString*)configlist_identifier:(NSInteger)rowIndex;
- (void)configlist_clear_all_values:(NSInteger)rowIndex;

@end
