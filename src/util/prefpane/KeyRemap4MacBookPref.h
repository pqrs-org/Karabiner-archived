/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import <PreferencePanes/PreferencePanes.h>

@interface KeyRemap4MacBookPref : NSPreferencePane
{
  IBOutlet id _versionText;
  IBOutlet id _checkbox_statusbar;
  IBOutlet id _popup_checkupdate;
  IBOutlet id _label_updatechecked;
}

- (IBAction) toggleStatusBar:(id)sender;
- (IBAction) changeCheckUpdate:(id)sender;
- (IBAction) launchUninstaller:(id)sender;
- (IBAction) launchEventViewer:(id)sender;
- (IBAction) launchThreeFingerRelativeToScroll:(id)sender;
- (IBAction) checkUpdateNow:(id)sender;
- (void) mainViewDidLoad;

@end
