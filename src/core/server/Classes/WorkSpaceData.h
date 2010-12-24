// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "ConfigXMLParser.h"

@interface WorkSpaceData : NSObject {
  IBOutlet ConfigXMLParser* configxmlparser_;
}
- (NSString*) getActiveApplicationName;
- (unsigned int) getApplicationType:(NSString*)name;
@end


@interface WorkSpaceData (InputSource)
- (NSString*) getTISPropertyInputModeID;
- (void) getInputMode:(NSString*)name output_inputmode:(unsigned int*)output_inputmode output_inputmodedetail:(unsigned int*)output_inputmodedetail;
- (void) selectInputSource:(unsigned int)vk_keycode;
@end
