// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@class InputSource;

@interface WorkSpaceData : NSObject

- (unsigned int)getUIElementRole:(NSString*)name;

// InputSource
+ (void)refreshEnabledInputSources;
+ (InputSource*)getCurrentInputSource;
- (void)selectInputSource:(unsigned int)vk_keycode;

@end
