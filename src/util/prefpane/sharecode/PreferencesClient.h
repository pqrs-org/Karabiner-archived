// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>

@protocol KeyRemap4MacBookPreferencesManagerProtocol
- (int) value:(NSString*)name;

- (NSInteger) selectedIndex;
- (NSArray*) getConfigList;
- (void) select:(NSInteger)newindex;

- (BOOL) isStatusbarEnable;
- (BOOL) isShowSettingNameInStatusBar;
@end

@interface PreferencesClient : NSObject {
  NSDistantObject* proxy;
}

@property (assign) NSDistantObject* proxy;

@end
