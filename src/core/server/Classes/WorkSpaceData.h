// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>
#import "InputSource.h"
#import "XMLCompiler.h"

@interface WorkSpaceData : NSObject {
  IBOutlet XMLCompiler* xmlCompiler_;
}

- (unsigned int)getUIElementRole:(NSString*)name;

@end

@interface WorkSpaceData (InputSource)
+ (void)refreshEnabledInputSources;

+ (InputSource*)getCurrentInputSource;
- (void)selectInputSource:(unsigned int)vk_keycode;
@end
