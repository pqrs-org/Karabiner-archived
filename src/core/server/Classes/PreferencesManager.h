// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "ConfigXMLParser.h"

@interface PreferencesManager : NSObject {
  NSMutableDictionary* default_;
  IBOutlet ConfigXMLParser* configxmlparser_;
}

- (unsigned int) value:(NSString*)name;

@end
