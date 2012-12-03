/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

@class PreferencesManager;
@class XMLCompiler;

@interface PreferencesController : NSObject <NSWindowDelegate>
{
  IBOutlet NSButton* checkbox_showEnabledOnly_;
  IBOutlet NSTextField* versionText_;
  IBOutlet NSWindow* preferencesWindow_;
  IBOutlet PreferencesManager* preferencesManager_;
  IBOutlet XMLCompiler* xmlCompiler_;
}

- (void) show;

@end
