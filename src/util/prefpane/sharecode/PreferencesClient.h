// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>

@protocol org_pqrs_KeyRemap4MacBook_PreferencesManagerProtocol
- (int) value:(NSString*)name;
- (int) defaultValue:(NSString*)name;
- (void) setValueForName:(int)newval forName:(NSString*)name;

- (NSArray*) essential_config;
- (NSDictionary*) changed;

// --------------------------------------------------
- (NSInteger)     configlist_selectedIndex;
- (NSString*)     configlist_selectedName;
- (NSString*)     configlist_selectedIdentifier;
- (NSArray*)      configlist_getConfigList;
- (NSUInteger)    configlist_count;
- (NSDictionary*) configlist_dictionary:(NSInteger)rowIndex;
- (NSString*)     configlist_name:(NSInteger)rowIndex;
- (NSString*)     configlist_identifier:(NSInteger)rowIndex;
- (void)          configlist_select:(NSInteger)newindex;
- (void)          configlist_setName:(NSInteger)rowIndex name:(NSString*)name;
- (void)          configlist_append;
- (void)          configlist_delete:(NSInteger)rowIndex;

- (BOOL) isStatusbarEnable;
- (BOOL) isShowSettingNameInStatusBar;
- (void) toggleStatusbarEnable;
- (void) toggleShowSettingNameInStatusBar;

- (NSInteger) checkForUpdatesMode;
- (void) setCheckForUpdatesMode:(NSInteger)newval;

- (void) configxml_reload;
- (BOOL) configxml_initialized;

- (NSArray*) preferencepane_checkbox;
- (NSArray*) preferencepane_number;
- (NSString*) preferencepane_error_message;
- (NSString*) preferencepane_get_private_xml_path;

@end

@interface org_pqrs_KeyRemap4MacBook_PreferencesClient : NSObject {
  id proxy_;
}

- (void) observer_NSConnectionDidDieNotification:(NSNotification*)notification;
- (id) proxy;

@end
