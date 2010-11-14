// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "ConfigXMLParser.h"

@interface PreferencesManager : NSObject {
  NSMutableDictionary* default_;
  NSMutableDictionary* value_;
  NSArray* essential_config_index_;
  IBOutlet ConfigXMLParser* configxmlparser_;
}

- (int) value:(NSString*)name;
- (NSArray*) essential_config;

@end
