// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;
#import "InputSource.h"
#import "XMLCompiler.h"

@interface WorkSpaceData : NSObject

@property(weak) IBOutlet XMLCompiler* xmlCompiler;

- (unsigned int)getUIElementRole:(NSString*)name;

@end

@interface WorkSpaceData (InputSource)

+ (void)refreshEnabledInputSources;
+ (InputSource*)getCurrentInputSource;
- (void)selectInputSource:(unsigned int)vk_keycode;

@end
