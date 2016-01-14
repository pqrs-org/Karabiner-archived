// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@class PreferencesManager;
@class XMLCompiler;
@class OutlineViewDataSourceCheckbox;
@class OutlineViewDataSourceNumber;

@interface OutlineView : NSObject {
@protected
  BOOL ischeckbox_;

  IBOutlet NSOutlineView* outlineview_;
  IBOutlet PreferencesManager* preferencesManager_;
  IBOutlet XMLCompiler* xmlCompiler_;
  IBOutlet OutlineViewDataSourceCheckbox* outlineViewDataSourceCheckbox_;
  IBOutlet OutlineViewDataSourceNumber* outlineViewDataSourceNumber_;

  // for OutlineView_checkbox
  IBOutlet NSSearchField* searchText_;
  IBOutlet NSButton* showEnabledOnly_;
}

- (void)load:(BOOL)force;

- (IBAction)filter:(id)sender;
- (IBAction)expand:(id)sender;
- (IBAction)collapse:(id)sender;

@end
