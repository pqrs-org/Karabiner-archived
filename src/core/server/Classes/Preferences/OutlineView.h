// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@class PreferencesManager;
@class XMLCompiler;

@interface OutlineView : NSObject {
  @protected
  NSArray* datasource_;
  NSString* error_message_;
  BOOL ischeckbox_;

  IBOutlet NSOutlineView* outlineview_;
  IBOutlet PreferencesManager* preferencesManager_;
  IBOutlet XMLCompiler* xmlCompiler_;
}

- (void) load:(BOOL)force;
- (void) filterDataSource:(BOOL)isEnabledOnly string:(NSString*)string;

- (void) observer_ConfigListChanged:(NSNotification*)notification;
- (void) observer_ConfigXMLReloaded:(NSNotification*)notification;

@end
