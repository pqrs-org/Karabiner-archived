/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "PreferencesWindowController.h"
#import "CheckboxOutlineView.h"
#import "CheckboxOutlineViewDataSource.h"
#import "CheckboxOutlineViewDelegate.h"
#import "ParameterOutlineView.h"
#import "ParameterOutlineViewDataSource.h"
#import "ParameterOutlineViewDelegate.h"
#import "PreferencesModel.h"
#import "ProfileCellView.h"
#import "ProfileTableView.h"
#import "ProfileTableViewDataSource.h"
#import "ProfileTableViewDelegate.h"
#import "Relauncher.h"
#import "ServerClient.h"
#import "SharedKeys.h"
#import "SharedUtilities.h"

#include <sys/sysctl.h>
#include <sys/types.h>

@interface PreferencesWindowController ()

@property(weak) IBOutlet AXNotifierPreferencesModel* axNotifierPreferencesModel;
@property(weak) IBOutlet CheckboxOutlineView* checkboxOutlineView;
@property(weak) IBOutlet CheckboxOutlineViewDataSource* checkboxOutlineViewDataSource;
@property(weak) IBOutlet CheckboxOutlineViewDelegate* checkboxOutlineViewDelegate;
@property(weak) IBOutlet NSButton* checkbox_showEnabledOnly;
@property(weak) IBOutlet NSSearchField* checkboxSearchText;
@property(weak) IBOutlet NSSegmentedControl* checkboxFontSegmentedControl;
@property(weak) IBOutlet NSStepper* delayUntilRepeatStepper;
@property(weak) IBOutlet NSStepper* keyRepeatStepper;
@property(weak) IBOutlet NSTabView* keyRepeatParameters;
@property(weak) IBOutlet NSTabView* tabView;
@property(weak) IBOutlet NSTextField* debugModeGuideTextField;
@property(weak) IBOutlet NSTextField* delayUntilRepeatLabel;
@property(weak) IBOutlet NSTextField* delayUntilRepeatTextField;
@property(weak) IBOutlet NSTextField* keyRepeatLabel;
@property(weak) IBOutlet NSTextField* keyRepeatTextField;
@property(weak) IBOutlet NSTextField* versionText;
@property(weak) IBOutlet ParameterOutlineView* parameterOutlineView;
@property(weak) IBOutlet ParameterOutlineViewDataSource* parameterOutlineViewDataSource;
@property(weak) IBOutlet ParameterOutlineViewDelegate* parameterOutlineViewDelegate;
@property(weak) IBOutlet PreferencesModel* preferencesModel;
@property(weak) IBOutlet ProfileTableView* profileTableView;
@property(weak) IBOutlet ProfileTableViewDataSource* profileTableViewDataSource;
@property(weak) IBOutlet ProfileTableViewDelegate* profileTableViewDelegate;
@property(weak) IBOutlet ServerClient* client;
@property NSTimer* resizeTimer;

@end

@implementation PreferencesWindowController

- (void)observer_kKarabinerServerDidLaunchNotification:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [Relauncher relaunch];
  });
}

- (void)observer_kKarabinerPreferencesUpdatedNotification:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    if (notification.userInfo &&
        [notification.userInfo[@"processIdentifier"] intValue] != [NSProcessInfo processInfo].processIdentifier) {
      NSLog(@"PreferencesModel is changed in another process.");
      [self.client.proxy loadPreferencesModel:self.preferencesModel];
      [self.client.proxy loadAXNotifierPreferencesModel:self.axNotifierPreferencesModel];

      [self drawEnabledCount];
      [self refreshKeyRepeatTab];

      if ([self.checkbox_showEnabledOnly state] == NSOnState) {
        [self.checkboxOutlineViewDataSource clearFilterCondition];
        [self filterCheckboxOutlineView:self];
      } else {
        [self.checkboxOutlineView reloadData];
      }
      [self.parameterOutlineView reloadData];
      [self.profileTableView reloadData];
    }
  });
}

- (void)observer_kKarabinerXMLReloadedNotification:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self drawEnabledCount];
    // refreshKeyRepeatTab is not needed here.

    [self.checkboxOutlineViewDataSource setup];
    [self.parameterOutlineViewDataSource setup];

    [self.checkboxOutlineViewDelegate clearHeightCache];
    [self.checkboxOutlineViewDataSource clearFilterCondition];
    [self filterCheckboxOutlineView:self];

    [self.parameterOutlineView reloadData];
    [self expandParameterOutlineView:self];
  });
}

- (void)setup {
  [Relauncher resetRelaunchedCount];

  [self checkServerClient];

  // In Mac OS X 10.7, NSDistributedNotificationCenter is suspended after calling [NSAlert runModal].
  // So, we need to set suspendedDeliveryBehavior to NSNotificationSuspensionBehaviorDeliverImmediately.
  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(observer_kKarabinerServerDidLaunchNotification:)
                                                          name:kKarabinerServerDidLaunchNotification
                                                        object:nil
                                            suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];

  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(observer_kKarabinerPreferencesUpdatedNotification:)
                                                          name:kKarabinerPreferencesUpdatedNotification
                                                        object:nil
                                            suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];

  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(observer_kKarabinerXMLReloadedNotification:)
                                                          name:kKarabinerXMLReloadedNotification
                                                        object:nil
                                            suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];

  NSString* version = [[NSBundle mainBundle] infoDictionary][@"CFBundleVersion"];
  [self.versionText setStringValue:version];

  [self drawEnabledCount];
  [self refreshKeyRepeatTab];

  [self.checkboxOutlineViewDataSource setup];
  [self.parameterOutlineViewDataSource setup];

  [self.checkboxOutlineView reloadData];
  [self.parameterOutlineView reloadData];
  [self.profileTableView reloadData];
}

- (void)dealloc {
  [[NSDistributedNotificationCenter defaultCenter] removeObserver:self];
}

- (void)windowDidBecomeMain:(NSNotification*)notification {
  [self checkServerClient];
  [self updateDebugModeGuide];
  [self sendStatusWindowPreferencesNotification:[[self.tabView selectedTabViewItem] identifier]];
}

- (void)show {
  [self.window makeKeyAndOrderFront:self];
  [NSApp activateIgnoringOtherApps:YES];
}

- (void)checkServerClient {
  if ([[self.client.proxy bundleVersion] length] == 0) {
    NSLog(@"Karabiner server is not running.");
    [NSApp terminate:self];
  }
}

/* ---------------------------------------------------------------------- */
- (void)drawEnabledCount {
  NSUInteger count = [self.client.proxy enabledCheckboxCount];
  [self.checkbox_showEnabledOnly setTitle:[NSString stringWithFormat:@"show enabled only (%d %@)", (int)(count), count >= 2 ? @"items" : @"item"]];
}

/* ---------------------------------------------------------------------- */
- (void)sendStatusWindowPreferencesNotification:(NSString*)identifier {
  if ([identifier isEqualToString:@"StatusMessage"]) {
    [self.client.proxy showExampleStatusWindow:YES];
  } else {
    [self.client.proxy showExampleStatusWindow:NO];
  }
}

/* ---------------------------------------------------------------------- */
- (void)windowWillClose:(NSNotification*)notification {
  [self sendStatusWindowPreferencesNotification:nil];
}

- (void)windowDidResize:(NSNotification*)notification {
  [self.resizeTimer invalidate];

  self.resizeTimer = [NSTimer scheduledTimerWithTimeInterval:0.5
                                                      target:self
                                                    selector:@selector(reloadCheckboxOutlineView:)
                                                    userInfo:nil
                                                     repeats:NO];
}

- (void)reloadCheckboxOutlineView:(NSTimer*)timer {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self.checkboxOutlineViewDelegate clearHeightCache];
    [self.checkboxOutlineView reloadData];
  });
}

/* ---------------------------------------------------------------------- */
- (void)tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem {
  [self updateDebugModeGuide];
  [self sendStatusWindowPreferencesNotification:[tabViewItem identifier]];
}

/* ---------------------------------------------------------------------- */
- (void)savePreferencesModel {
  [self.client.proxy savePreferencesModel:self.preferencesModel processIdentifier:[NSProcessInfo processInfo].processIdentifier];
}

- (IBAction)preferencesChanged:(id)sender {
  [self savePreferencesModel];
}

- (IBAction)resumeAtLoginChanged:(id)sender {
  [self savePreferencesModel];
  [self.client.proxy updateStartAtLogin];
}

- (IBAction)checkboxFontConfigurationChanged:(id)sender {
  [self savePreferencesModel];
  [self.checkboxOutlineViewDelegate updateFont];
  [self.checkboxOutlineViewDelegate clearHeightCache];
  [self.checkboxOutlineView reloadData];
}

- (IBAction)overrideKeyRepeatConfigurationChanged:(id)sender {
  [self savePreferencesModel];
  [self refreshKeyRepeatTab];
}

- (IBAction)statusBarConfigurationChanged:(id)sender {
  [self savePreferencesModel];
  [self.client.proxy updateStatusBar];
}

- (IBAction)statusWindowConfigurationChanged:(id)sender {
  [self savePreferencesModel];
  [self.client.proxy updateStatusWindow];
}

- (IBAction)axNotifierConfigurationChanged:(id)sender {
  [self.client.proxy saveAXNotifierPreferencesModel:self.axNotifierPreferencesModel processIdentifier:[NSProcessInfo processInfo].processIdentifier];
  [self.client.proxy restartAXNotifier];
}

- (IBAction)focusSearchField:(id)sender {
  [self.tabView selectFirstTabViewItem:sender];
  [self.window makeFirstResponder:self.checkboxSearchText];
}

- (IBAction)reloadXML:(id)sender {
  [self.client.proxy reloadXML];
}

- (IBAction)usePreparedSettingsConfigurationChanged:(id)sender {
  [self savePreferencesModel];
  [self.client.proxy reloadXML];
}

- (IBAction)filterCheckboxOutlineView:(id)sender {
  BOOL isEnabledOnly = ([self.checkbox_showEnabledOnly state] == NSOnState);
  NSString* string = [self.checkboxSearchText stringValue];
  if ([self.checkboxOutlineViewDataSource filterDataSource:isEnabledOnly string:string]) {
    [self.checkboxOutlineView reloadData];

    if ([string length] == 0 && isEnabledOnly == NO) {
      [self.checkboxOutlineView collapseItem:nil collapseChildren:YES];
    } else {
      [self.checkboxOutlineView expandItem:nil expandChildren:YES];
    }
  }
}

- (IBAction)expandCheckboxOutlineView:(id)sender {
  [self.checkboxOutlineView expandItem:nil expandChildren:YES];
}

- (IBAction)collapseCheckboxOutlineView:(id)sender {
  [self.checkboxOutlineView collapseItem:nil collapseChildren:YES];
}

- (IBAction)quitWithConfirmation:(id)sender {
  if ([SharedUtilities confirmQuit]) {
    @try {
      [self.client.proxy terminateServerProcess];
    } @catch (NSException* exception) {
    }

    [NSApp terminate:nil];
  }
}

- (IBAction)expandParameterOutlineView:(id)sender {
  [self.parameterOutlineView expandItem:nil expandChildren:YES];
}

- (IBAction)collapseParameterOutlineView:(id)sender {
  [self.parameterOutlineView collapseItem:nil collapseChildren:YES];
}

- (IBAction)addNewProfile:(id)sender {
  [self.preferencesModel addProfile:@"New Profile"];
  [self savePreferencesModel];

  [self.profileTableView reloadData];
}

- (void)disableAllProfileCells {
  // Set profileIndex to -1 for all ProfileCellView in order to avoid to call configlist_setName while sorting.
  //
  // If you call sortProfilesByName or sortProfilesByCreated while you are editing profile name,
  // [ProfileCellView nameChanged] method will be called with wrong name and profileIndex pair. (sorted name and unsorted profileIndex).
  // Therefore, we need to ignore the nameChanged call while sorting by this method.

  for (NSInteger i = 0; i < self.profileTableView.numberOfRows; ++i) {
    NSTableRowView* rowView = [self.profileTableView rowViewAtRow:i makeIfNecessary:NO];
    if (rowView) {
      ProfileCellView* cellView = [rowView viewAtColumn:0];
      cellView.profileIndex = -1;
    }
  }
}

- (IBAction)sortProfilesByName:(id)sender {
  [self disableAllProfileCells];

  [self.preferencesModel sortProfilesByName];
  [self savePreferencesModel];

  [self.profileTableView reloadData];
}

- (IBAction)sortProfilesByCreated:(id)sender {
  [self disableAllProfileCells];

  [self.preferencesModel sortProfilesByAppendIndex];
  [self savePreferencesModel];

  [self.profileTableView reloadData];
}

- (IBAction)openURL:(id)sender {
  [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[sender title]]];
}

- (IBAction)openPrivateXMLUsageURL:(id)sender {
  NSString* url = @"https://pqrs.org/osx/karabiner/document.html#privatexml";
  [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:url]];
}

- (void)refreshKeyRepeatTab {
  BOOL enabled = self.preferencesModel.overrideKeyRepeat;

  if (enabled) {
    [self.keyRepeatParameters selectTabViewItemAtIndex:1];
  } else {
    [self.keyRepeatParameters selectTabViewItemAtIndex:0];
  }

  [self.delayUntilRepeatTextField setEnabled:enabled];
  [self.delayUntilRepeatStepper setEnabled:enabled];
  [self.keyRepeatTextField setEnabled:enabled];
  [self.keyRepeatStepper setEnabled:enabled];

  int delayUntilRepeat = [self.client.proxy value:@"repeat.initial_wait"];
  int keyRepeat = [self.client.proxy value:@"repeat.wait"];

  [self.delayUntilRepeatLabel setStringValue:[NSString stringWithFormat:@"%d milliseconds", delayUntilRepeat]];
  [self.delayUntilRepeatTextField setIntegerValue:delayUntilRepeat];
  [self.delayUntilRepeatStepper setIntegerValue:delayUntilRepeat];
  [self.keyRepeatLabel setStringValue:[NSString stringWithFormat:@"%d milliseconds", keyRepeat]];
  [self.keyRepeatTextField setIntegerValue:keyRepeat];
  [self.keyRepeatStepper setIntegerValue:keyRepeat];
}

- (IBAction)openSystemPreferencesKeyboard:(id)sender {
  [self.client.proxy openSystemPreferencesKeyboard];
}

- (IBAction)changeDelayUntilRepeat:(id)sender {
  [self.client.proxy setValue:[sender intValue] forName:@"repeat.initial_wait"];
  [self refreshKeyRepeatTab];
}

- (IBAction)changeKeyRepeat:(id)sender {
  [self.client.proxy setValue:[sender intValue] forName:@"repeat.wait"];
  [self refreshKeyRepeatTab];
}

- (IBAction)restartAXNotifier:(id)sender {
  if (!self.axNotifierPreferencesModel.useAXNotifier) {
    NSAlert* alert = [NSAlert new];
    [alert setMessageText:@"Karabiner Alert"];
    [alert addButtonWithTitle:@"Close"];
    [alert setInformativeText:@"AXNotifier is disabled.\nPlease enable AXNotifier if you want to start."];
    [alert runModal];
    return;
  }

  [self.client.proxy restartAXNotifier];
}

- (IBAction)checkForUpdatesStableOnly:(id)sender {
  [self.client.proxy checkForUpdatesStableOnly];
}

- (IBAction)checkForUpdatesWithBetaVersion:(id)sender {
  [self.client.proxy checkForUpdatesWithBetaVersion];
}

- (IBAction)launchUninstaller:(id)sender {
  [self.client.proxy openUninstaller];
}

- (IBAction)openPrivateXML:(id)sender {
  [self.client.proxy openPrivateXMLDirectory];
}

- (IBAction)launchEventViewer:(id)sender {
  [self.client.proxy openEventViewer];
}

- (IBAction)launchMultiTouchExtension:(id)sender {
  [self.client.proxy openMultiTouchExtension];
}

- (IBAction)openConsoleApp:(id)sender {
  [[NSWorkspace sharedWorkspace] openFile:@"/Applications/Utilities/Console.app"];
}

- (IBAction)relaunch:(id)sender {
  [Relauncher relaunch];
}

- (IBAction)toggleDebugMode:(id)sender {
  if ([self isDebugMode]) {
    [self.client.proxy unsetDebugFlags];
  } else {
    [[[NSAppleScript alloc] initWithSource:@"do shell script \"/usr/sbin/sysctl -w karabiner.debug=1\" with administrator privileges"] executeAndReturnError:nil];
  }
  [self updateDebugModeGuide];
}

- (BOOL)isDebugMode {
  int debug;
  size_t len = sizeof(debug);
  sysctlbyname("karabiner.debug", &debug, &len, NULL, 0);
  return debug;
}

- (void)updateDebugModeGuide {
  if ([self isDebugMode]) {
    [self.debugModeGuideTextField setStringValue:@"Debug Mode is enabled.\nDo not type sensitive information such as password.\nYour key input are saved into system.log."];
    // #a94442
    [self.debugModeGuideTextField setTextColor:[NSColor colorWithCalibratedRed:(0xa9 * 1.0 / 255)
                                                                         green:(0x44 * 1.0 / 255)
                                                                          blue:(0x42 * 1.0 / 255)
                                                                         alpha:1.0]];
    // #f2dede
    [self.debugModeGuideTextField setBackgroundColor:[NSColor colorWithCalibratedRed:(0xf2 * 1.0 / 255)
                                                                               green:(0xde * 1.0 / 255)
                                                                                blue:(0xde * 1.0 / 255)
                                                                               alpha:1.0]];
    self.debugModeGuideTextField.drawsBackground = YES;

  } else {
    [self.debugModeGuideTextField setStringValue:@"Debug Mode is disabled.\nAdministrator privileges are required to enable Debug Mode."];
    [self.debugModeGuideTextField setTextColor:[NSColor textColor]];
    self.debugModeGuideTextField.drawsBackground = NO;
  }
}

@end
