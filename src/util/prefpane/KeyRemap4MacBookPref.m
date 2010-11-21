/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "KeyRemap4MacBookPref.h"
#import "Common.h"
#import "SysctlWrapper.h"

@implementation KeyRemap4MacBookPref

static NSString* launchUninstallerCommand = @"/Library/org.pqrs/KeyRemap4MacBook/extra/launchUninstaller.sh";

/* ---------------------------------------------------------------------- */
- (void) drawVersion
{
  NSString* version = [BUNDLEPREFIX (SysctlWrapper) getString:@"keyremap4macbook.version"];
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
  if ([[preferencesclient_ proxy] isStatusbarEnable]) {
    [_checkbox_statusbar setState:NSOnState];
  } else {
    [_checkbox_statusbar setState:NSOffState];
  }
}

- (IBAction) toggleStatusBar:(id)sender
{
  [[preferencesclient_ proxy] toggleStatusbarEnable];
}

- (void) setStatusBarShowNameState
{
  if ([[preferencesclient_ proxy] isShowSettingNameInStatusBar]) {
    [_checkbox_statusbar_showname setState:NSOnState];
  } else {
    [_checkbox_statusbar_showname setState:NSOffState];
  }
}

- (IBAction) toggleStatusBarShowName:(id)sender
{
  [[preferencesclient_ proxy] toggleShowSettingNameInStatusBar];
}

/* ---------------------------------------------------------------------- */
- (void) setCheckUpdateState
{
  [_popup_checkupdate selectItemAtIndex:[[preferencesclient_ proxy] checkForUpdatesMode]];
}

- (IBAction) changeCheckUpdate:(id)sender
{
  [[preferencesclient_ proxy] setCheckForUpdatesMode:[_popup_checkupdate indexOfSelectedItem]];
  [self setCheckUpdateState];
}

/* ---------------------------------------------------------------------- */
- (IBAction) launchUninstaller:(id)sender
{
  [BUNDLEPREFIX (Common) getExecResult:launchUninstallerCommand args:[NSArray arrayWithObjects:@"force", nil]];
}

- (IBAction) launchEventViewer:(id)sender
{
  [[NSWorkspace sharedWorkspace] launchApplication:@"/Library/org.pqrs/KeyRemap4MacBook/app/KeyDump.app"];
}

- (IBAction) launchThreeFingerRelativeToScroll:(id)sender
{
  [[NSWorkspace sharedWorkspace] launchApplication:@"/Library/org.pqrs/KeyRemap4MacBook/app/ThreeFingerRelativeToScroll.app"];
}

- (IBAction) openPrivateXML:(id)sender
{
  // Open a directory which contains private.xml.
  NSString* path = [[preferencesclient_ proxy] preferencepane_get_private_xml_path];
  if ([path length] > 0) {
    [[NSWorkspace sharedWorkspace] openFile:[path stringByDeletingLastPathComponent]];
  }
}

- (IBAction) checkUpdateNow:(id)sender
{
  NSString* observedObject = @"org.pqrs.KeyRemap4MacBook.updater";
  [[NSDistributedNotificationCenter defaultCenter] postNotificationName:@"check" object:observedObject userInfo:nil deliverImmediately:YES];

  // We wait 1sec to improve the impression that pushed the button.
  // This is useless processing, but it is surely necessary for UI.
  [NSThread sleepForTimeInterval:1.0];

  [_label_updatechecked setHidden:NO];
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
