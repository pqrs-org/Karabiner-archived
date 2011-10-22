// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "KeyCode.h"
#import "RemapClassesInitializeVector.h"

@interface ConfigXMLParser : NSObject {
  unsigned int simultaneous_keycode_index_;
  NSMutableDictionary* dict_config_name_;
  RemapClassesInitializeVector* remapclasses_initialize_vector_;
  NSMutableArray* preferencepane_checkbox_;
  NSMutableArray* preferencepane_number_;
  NSMutableArray* appdefdata_;
  KeyCode* keycode_;
  NSString* error_message_;
  BOOL initialized_;
}

+ (ConfigXMLParser*) getInstance;

- (BOOL) initialized;
- (BOOL) reload;
- (NSUInteger) count;
- (RemapClassesInitializeVector*) remapclasses_initialize_vector;
- (unsigned int) keycode:(NSString*)name;
- (NSString*) configname:(unsigned int)configindex;

- (NSArray*) preferencepane_checkbox;
- (NSArray*) preferencepane_number;
- (NSString*) preferencepane_error_message;
- (NSString*) preferencepane_get_private_xml_path;

- (NSString*) appdef_getApplicationType:(NSString*)bundleIdentifier;

@end

@interface ConfigXMLParserAppDefData : NSObject {
  NSString* name_;
  NSMutableArray* rules_equal_;
  NSMutableArray* rules_prefix_;
}

@property (assign, readonly) NSString* name;

- (void) setName:(NSString*)newname;
- (void) addEqualRule:(NSString*)identifier;
- (void) addPrefixRule:(NSString*)identifier;
- (NSString*) getApplicationType:(NSString*)bundleIdentifier;

@end

@interface ConfigXMLParserDeviceDefData : NSObject {
  NSString* name_;
  NSInteger value_;
}

@property (assign, readonly) NSString* name;

- (void) setName:(NSString*)newname;
- (void) setValue:(NSInteger)newvalue;
- (NSInteger) value;

@end

@interface ConfigXMLParser (Private)
enum {
  CONFIGXMLPARSER_XML_TYPE_CHECKBOX,
  CONFIGXMLPARSER_XML_TYPE_NUMBER,
  CONFIGXMLPARSER_XML_OWNER_SYSTEM,
  CONFIGXMLPARSER_XML_OWNER_USER,
};
- (NSArray*) get_xml_paths;

- (void) setErrorMessage:(NSException*)exception xmlpath:(NSString*)xmlpath;
- (void) removeErrorMessage;

- (NSXMLElement*) castToNSXMLElement:(NSXMLNode*)node;
@end
