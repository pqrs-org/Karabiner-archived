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
+ (void) getLanguage:(InputSource*)inputSource
 output_language:(uint32_t*)output_language
 output_languageDetail:(uint32_t*)output_languageDetail;
+ (void) getInputMode:(NSString*)name output_inputmode:(unsigned int*)output_inputmode output_inputmodedetail:(unsigned int*)output_inputmodedetail;
+ (void) selectInputSource:(unsigned int)vk_keycode;
@end
