// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>

@protocol KeyRemap4MacBookPreferencesManagerProtocol
- (int) value:(NSString*)name;

- (NSInteger)  configlist_selectedIndex;
- (NSString*)  configlist_selectedName;
- (NSArray*)   configlist_getConfigList;
- (NSUInteger) configlist_count;
- (NSString*)  configlist_name:(NSInteger)rowIndex;
- (void)       configlist_select:(NSInteger)newindex;
- (void)       configlist_setName:(NSInteger)rowIndex name:(NSString*)name;

- (BOOL) isStatusbarEnable;
- (BOOL) isShowSettingNameInStatusBar;
@end

@interface PreferencesClient : NSObject {
  id proxy;
}

@property (assign) id proxy;

@end
