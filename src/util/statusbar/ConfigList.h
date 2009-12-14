// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>

@interface ConfigList : NSObject

+ (void) refresh;
+ (int) isStatusbarEnable;
+ (int) getSelectedIndex;
+ (int) getSize;
+ (NSString*) getName:(int)index;
+ (void) setName:(int)index newName:(NSString*)newname;
+ (void) add;
+ (void) delete:(NSInteger)index;
+ (void) select:(NSString*)idx;

@end
