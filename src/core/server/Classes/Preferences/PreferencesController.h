/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

@class ClientForKernelspace;
@class PreferencesManager;
@class XMLCompiler;

@interface PreferencesController : NSObject <NSWindowDelegate, NSTabViewDelegate> {
  IBOutlet ClientForKernelspace* clientForKernelspace_;
  IBOutlet NSButton* checkbox_showEnabledOnly_;
  IBOutlet NSTabView* tabView_;
  IBOutlet NSTextField* versionText_;
  IBOutlet NSWindow* preferencesWindow_;
  IBOutlet PreferencesManager* preferencesManager_;
  IBOutlet XMLCompiler* xmlCompiler_;

  IBOutlet NSTabView* keyRepeatParameters_;
  IBOutlet NSTextField* delayUntilRepeatLabel_;
  IBOutlet NSTextField* keyRepeatLabel_;
  IBOutlet NSTextField* delayUntilRepeatTextField_;
  IBOutlet NSTextField* keyRepeatTextField_;
  IBOutlet NSStepper* delayUntilRepeatStepper_;
  IBOutlet NSStepper* keyRepeatStepper_;
  IBOutlet NSTextField* debugModeGuideTextField_;
}

- (void)show;
- (IBAction)openURL:(id)sender;
- (IBAction)openPrivateXMLUsageURL:(id)sender;
- (IBAction)refreshKeyRepeatTab:(id)sender;
- (IBAction)openSystemPreferencesKeyboard:(id)sender;
- (IBAction)changeDelayUntilRepeat:(id)sender;
- (IBAction)changeKeyRepeat:(id)sender;
- (IBAction)openConsoleApp:(id)sender;
- (IBAction)toggleDebugMode:(id)sender;

@end
