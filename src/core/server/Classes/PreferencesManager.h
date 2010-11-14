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

- (NSInteger) selectedIndex;
- (NSString*) selectedName;
- (NSArray*) getConfigList;
- (void) select:(NSInteger)newindex;

- (BOOL) isStatusbarEnable;
- (BOOL) isShowSettingNameInStatusBar;

@end
