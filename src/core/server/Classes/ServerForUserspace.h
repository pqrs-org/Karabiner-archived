// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>
#import "PreferencesManager.h"
#import "Updater.h"
#import "UserClient_userspace.h"
#import "XMLCompiler.h"

@interface ServerForUserspace : NSObject {
  NSConnection* connection_;

  IBOutlet PreferencesManager* preferencesManager_;
  IBOutlet Updater* updater_;
  IBOutlet UserClient_userspace* userClient_userspace_;
  IBOutlet XMLCompiler* xmlCompiler_;
}

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

// --------------------------------------------------
- (void) configxml_reload;

- (NSArray*) preferencepane_checkbox;
- (NSArray*) preferencepane_number;
- (int) preferencepane_enabled_count;
- (NSString*) preferencepane_error_message;
- (NSString*) preferencepane_get_private_xml_path;
- (NSString*) preferencepane_version;

- (void) checkForUpdates;

- (NSArray*) device_information:(NSInteger)type;

@end
