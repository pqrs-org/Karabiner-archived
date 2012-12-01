/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

@class PreferencesManager;
@class XMLCompiler;

@interface PreferencesController : NSObject
{
  IBOutlet NSButton* checkbox_showEnabledOnly_;
  IBOutlet NSTextField* versionText_;
  IBOutlet PreferencesManager* preferencesManager_;
  IBOutlet XMLCompiler* xmlCompiler_;
}

@end
