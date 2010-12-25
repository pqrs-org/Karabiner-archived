/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import <PreferencePanes/PreferencePanes.h>
#import "KeyRemap4MacBookClient.h"

@interface KeyRemap4MacBookPref : NSPreferencePane
{
  IBOutlet id _versionText;
  IBOutlet id _checkbox_statusbar;
  IBOutlet id _checkbox_statusbar_showname;
  IBOutlet id _popup_checkupdate;
  IBOutlet id _label_updatechecked;
  IBOutlet org_pqrs_KeyRemap4MacBook_Client* client_;
}

- (IBAction) toggleStatusBar:(id)sender;
- (IBAction) toggleStatusBarShowName:(id)sender;
- (IBAction) changeCheckUpdate:(id)sender;
- (IBAction) launchUninstaller:(id)sender;
- (IBAction) openPrivateXML:(id)sender;
- (IBAction) launchEventViewer:(id)sender;
- (IBAction) launchThreeFingerRelativeToScroll:(id)sender;
- (IBAction) checkUpdateNow:(id)sender;
- (void) mainViewDidLoad;

@end
