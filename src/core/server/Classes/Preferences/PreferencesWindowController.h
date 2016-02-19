/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

@class ClientForKernelspace;
@class PreferencesManager;
@class XMLCompiler;

@interface PreferencesWindowController : NSObject <NSWindowDelegate, NSTabViewDelegate>

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
