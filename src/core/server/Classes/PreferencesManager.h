// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "ConfigXMLParser.h"

@interface PreferencesManager : NSObject {
  NSMutableDictionary* default_;
  NSMutableDictionary* value_;
  NSArray* essential_config_index_;
  NSConnection* serverconnection_;

  IBOutlet ConfigXMLParser* configxmlparser_;
}

- (int) value:(NSString*)name;
- (NSArray*) essential_config;

// --------------------------------------------------
- (NSInteger)  configlist_selectedIndex;
- (NSString*)  configlist_selectedName;
- (NSArray*)   configlist_getConfigList;
- (NSUInteger) configlist_count;
- (NSString*)  configlist_name:(NSInteger)rowIndex;
- (void)       configlist_select:(NSInteger)newindex;
- (void)       configlist_setName:(NSInteger)rowIndex name:(NSString*)name;
- (void)       configlist_append;
- (void)       configlist_delete:(NSInteger)rowIndex;

- (BOOL) isStatusbarEnable;
- (BOOL) isShowSettingNameInStatusBar;


@end
