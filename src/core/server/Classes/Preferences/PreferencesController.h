/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

@class PreferencesManager;
@class XMLCompiler;

@interface PreferencesController : NSObject <NSWindowDelegate, NSTabViewDelegate>
{
  IBOutlet NSButton* checkbox_showEnabledOnly_;
  IBOutlet NSTabView* tabView_;
  IBOutlet NSTextField* versionText_;
  IBOutlet NSWindow* preferencesWindow_;
  IBOutlet PreferencesManager* preferencesManager_;
  IBOutlet XMLCompiler* xmlCompiler_;
}

- (void) show;
- (IBAction) openWebPage:(id)sender;
- (IBAction) openGitHub:(id)sender;

@end
