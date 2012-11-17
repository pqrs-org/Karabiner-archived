/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "KeyRemap4MacBookPref.h"
#import "KeyRemap4MacBookKeys.h"
#import "KeyRemap4MacBookNSDistributedNotificationCenter.h"

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

- (void) drawEnabledCount
{
  int count = [[client_ proxy] preferencepane_enabled_count];
  [checkbox_showEnabledOnly_ setTitle:[NSString stringWithFormat:@"show enabled only (%d %@)", count, count >= 2 ? @"items":@"item"]];
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
  [[NSWorkspace sharedWorkspace] launchApplication:@"/Library/org.pqrs/KeyRemap4MacBook/app/EventViewer.app"];
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
  [[client_ proxy] checkForUpdates];
}

/* ---------------------------------------------------------------------- */
- (void) distributedObserver_preferencesChanged:(NSNotification*)notification
{
  // [NSAutoreleasePool drain] is never called from NSDistributedNotificationCenter.
  // Therefore, we need to make own NSAutoreleasePool.
  NSAutoreleasePool* pool = [NSAutoreleasePool new];
  {
    [self drawEnabledCount];
  }
  [pool drain];
}

- (void) distributedObserver_configXMLReloaded:(NSNotification*)notification
{
  // [NSAutoreleasePool drain] is never called from NSDistributedNotificationCenter.
  // Therefore, we need to make own NSAutoreleasePool.
  NSAutoreleasePool* pool = [NSAutoreleasePool new];
  {
    // If activated preferences in private.xml were vanished, the count is changed.
    // So, we need to call drawEnabledCount at configXMLReloaded notification.
    [self drawEnabledCount];
  }
  [pool drain];
}

- (void) mainViewDidLoad
{
  [self drawVersion];
  [self drawEnabledCount];
  [self setStatusBarState];
  [self setStatusBarShowNameState];
  [self setCheckUpdateState];

  [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter addObserver:self
                                                                selector:@selector(distributedObserver_preferencesChanged:)
                                                                    name:kKeyRemap4MacBookPreferencesChangedNotification];

  [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter addObserver:self
                                                                selector:@selector(distributedObserver_configXMLReloaded:)
                                                                    name:kKeyRemap4MacBookConfigXMLReloadedNotification];

  // For some reason, launchd does not start KeyRemap4MacBook server process permanently.
  // (And we can recover it by reloading plist.)
  //
  // However, calling launchctl in Terminal.app is not familiar for everyone.
  // Therefore, we call launchctl at prefpane.
  system("/bin/launchctl load -w /Library/LaunchAgents/org.pqrs.KeyRemap4MacBook.server.plist 2> /dev/null");
}

- (void) dealloc
{
  [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter removeObserver:self];

  [super dealloc];
}

@end
