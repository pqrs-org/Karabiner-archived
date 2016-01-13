// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@class PreferencesManager;
@class XMLCompiler;
@class OutlineViewDataSourceCheckbox;

@interface OutlineView : NSObject {
@protected
  NSArray* datasource_;
  BOOL ischeckbox_;

  IBOutlet NSOutlineView* outlineview_;
  IBOutlet PreferencesManager* preferencesManager_;
  IBOutlet XMLCompiler* xmlCompiler_;
  IBOutlet OutlineViewDataSourceCheckbox* outlineViewDataSourceCheckbox_;

  // for OutlineView_checkbox
  IBOutlet NSSearchField* searchText_;
  IBOutlet NSButton* showEnabledOnly_;
}

- (void)load:(BOOL)force;

- (IBAction)filter:(id)sender;
- (IBAction)expand:(id)sender;
- (IBAction)collapse:(id)sender;

@end
