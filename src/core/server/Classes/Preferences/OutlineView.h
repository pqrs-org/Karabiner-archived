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

  // for OutlineView_checkbox
  IBOutlet NSSearchField* searchText_;
  IBOutlet NSButton* showEnabledOnly_;
}

- (void) load:(BOOL)force;
- (void) filterDataSource:(BOOL)isEnabledOnly string:(NSString*)string;

- (IBAction) filter:(id)sender;
- (IBAction) expand:(id)sender;
- (IBAction) collapse:(id)sender;

@end
