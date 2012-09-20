// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "InputSource.h"

@interface WorkSpaceData : NSObject

+ (NSString*) getActiveApplicationName;
+ (unsigned int) getApplicationType:(NSString*)name;

@end


@interface WorkSpaceData (InputSource)
+ (void) refreshEnabledInputSources;

+ (InputSource*) getCurrentInputSource;
+ (void) getInputSourceID:(InputSource*)inputSource
 output_inputSource:(uint32_t*)output_inputSource
 output_inputSourceDetail:(uint32_t*)output_inputSourceDetail;
+ (void) selectInputSource:(unsigned int)vk_keycode;
@end
