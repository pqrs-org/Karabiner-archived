// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>

@protocol KeyRemap4MacBookPreferencesManagerProtocol
- (int) value:(NSString*)name;

- (NSInteger) selectedIndex;
- (NSArray*) getConfigList;
- (NSUInteger) count;
- (NSString*) name:(NSInteger)rowIndex;
- (void) select:(NSInteger)newindex;

- (BOOL) isStatusbarEnable;
- (BOOL) isShowSettingNameInStatusBar;

- (void) setName:(NSInteger)rowIndex name:(NSString*)name;
@end

@interface PreferencesClient : NSObject {
  id proxy;
}

@property (assign) id proxy;

@end
