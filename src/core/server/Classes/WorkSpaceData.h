// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "ConfigXMLParser.h"

@interface WorkSpaceData : NSObject {
  IBOutlet ConfigXMLParser* configxmlparser_;
}

- (unsigned int) getApplicationType:(NSString*)name;
- (void) getInputMode:(NSString*)name output_inputmode:(unsigned int*)output_inputmode output_inputmodedetail:(unsigned int*)output_inputmodedetail;

@end
