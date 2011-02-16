/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "KeyRemap4MacBookPref.h"

@implementation KeyRemap4MacBookPref

/* ---------------------------------------------------------------------- */
- (void) drawVersion
{
  NSString* version = [[client_ proxy] preferencepane_version];
  if (! version) {
    version = @"-.-.-";
  }
  [_versionText setStringValue:version];
}

- (void) terminateTargetApplication:(NSString*)bundleidentifier
{
  NSArray* list = [[NSWorkspace sharedWorkspace] runningApplications];

  for (NSRunningApplication* app in list) {
    if (! app) continue;

    NSString* bi = [app bundleIdentifier];
    if (! bi) continue;

    if ([bi isEqualToString:bundleidentifier]) {
      [app terminate];
    }
  }
}

/* ---------------------------------------------------------------------- */
- (void) setStatusBarState
{
  if ([[client_ proxy] isStatusbarEnable]) {
    [_checkbox_statusbar setState:NSOnState];
  } else {
    [_checkbox_statusbar setState:NSOffState];
  }
}

- (IBAction) toggleStatusBar:(id)sender
{
  [[client_ proxy] toggleStatusbarEnable];
}

- (void) setStatusBarShowNameState
{
  if ([[client_ proxy] isShowSettingNameInStatusBar]) {
    [_checkbox_statusbar_showname setState:NSOnState];
  } else {
    [_checkbox_statusbar_showname setState:NSOffState];
  }
}

- (IBAction) toggleStatusBarShowName:(id)sender
{
  [[client_ proxy] toggleShowSettingNameInStatusBar];
}

/* ---------------------------------------------------------------------- */
- (void) setCheckUpdateState
{
  [_popup_checkupdate selectItemAtIndex:[[client_ proxy] checkForUpdatesMode]];
}

- (IBAction) changeCheckUpdate:(id)sender
{
  [[client_ proxy] setCheckForUpdatesMode:[_popup_checkupdate indexOfSelectedItem]];
  [self setCheckUpdateState];
}

/* ---------------------------------------------------------------------- */
- (IBAction) launchUninstaller:(id)sender
{
  system("/Library/org.pqrs/KeyRemap4MacBook/extra/launchUninstaller.sh");
}

- (IBAction) launchEventViewer:(id)sender
{
  [[NSWorkspace sharedWorkspace] launchApplication:@"/Library/org.pqrs/KeyRemap4MacBook/app/KeyDump.app"];
}

- (IBAction) launchMultiTouchExtension:(id)sender
{
  [[NSWorkspace sharedWorkspace] launchApplication:@"/Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook_multitouchextension.app"];
}

- (IBAction) openPrivateXML:(id)sender
{
  // Open a directory which contains private.xml.
  NSString* path = [[client_ proxy] preferencepane_get_private_xml_path];
  if ([path length] > 0) {
    [[NSWorkspace sharedWorkspace] openFile:[path stringByDeletingLastPathComponent]];
  }
}

- (IBAction) checkUpdateNow:(id)sender
{
  NSString* observedObject = @"org.pqrs.KeyRemap4MacBook.updater";
  [[NSDistributedNotificationCenter defaultCenter] postNotificationName:@"check" object:observedObject userInfo:nil deliverImmediately:YES];
}

/* ---------------------------------------------------------------------- */
- (void) mainViewDidLoad
{
  [self drawVersion];
  [self setStatusBarState];
  [self setStatusBarShowNameState];
  [self setCheckUpdateState];
}

@end
