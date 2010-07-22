// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>

@interface ConfigControl : NSObject

+ (int) isStatusbarEnable;
+ (int) isShowSettingNameInStatusBar;

+ (NSArray*) getConfigList;
+ (void) setName:(int)index newName:(NSString*)newname;
+ (void) add;
+ (void) delete:(NSInteger)index;
+ (void) select:(NSString*)idx;

@end
