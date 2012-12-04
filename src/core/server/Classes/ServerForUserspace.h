// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>
#import "KeyRemap4MacBookProtocol.h"

@class ClientForKernelspace;
@class PreferencesManager;
@class Updater;
@class XMLCompiler;

@interface ServerForUserspace : NSObject<org_pqrs_KeyRemap4MacBook_Protocol> {
  NSConnection* connection_;

  IBOutlet ClientForKernelspace* clientForKernelspace_;
  IBOutlet PreferencesManager* preferencesManager_;
  IBOutlet Updater* updater_;
  IBOutlet XMLCompiler* xmlCompiler_;
}

- (BOOL) register;

- (int) value:(NSString*)name;
- (int) defaultValue:(NSString*)name;
- (void) setValueForName:(int)newval forName:(NSString*)name;

- (NSDictionary*) changed;

// --------------------------------------------------
- (NSInteger)     configlist_selectedIndex;
- (NSArray*)      configlist_getConfigList;
- (void)          configlist_select:(NSInteger)newindex;

// --------------------------------------------------
- (void) configxml_reload;

- (NSArray*) device_information:(NSInteger)type;

@end
