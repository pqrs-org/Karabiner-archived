// -*- Mode: objc -*-

@import Cocoa;

@class PreferencesManager;

@interface CheckboxOutlineViewDelegate : NSObject <NSOutlineViewDelegate>

@property(weak) IBOutlet NSOutlineView* outlineView;
@property(weak) IBOutlet PreferencesManager* preferencesManager;

@end
