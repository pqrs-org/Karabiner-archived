// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@protocol KeyRemap4MacBookProtocol
- (int) value:(NSString*)name;
- (int) defaultValue:(NSString*)name;
- (void) setValueForName:(int)newval forName:(NSString*)name;

- (NSDictionary*) changed;

- (NSInteger)     configlist_selectedIndex;
- (NSArray*)      configlist_getConfigList;
- (void)          configlist_select:(NSInteger)newindex;

- (void) configxml_reload;

// For EventViewer.
- (NSArray*) device_information:(NSInteger)type;
- (NSDictionary*) application_information;
- (NSDictionary*) inputsource_information;

@end
