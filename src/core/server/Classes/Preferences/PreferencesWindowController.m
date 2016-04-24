/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "PreferencesWindowController.h"
#import "AXNotifierManager.h"
#import "AppLauncher.h"
#import "CheckboxOutlineView.h"
#import "CheckboxOutlineViewDataSource.h"
#import "CheckboxOutlineViewDelegate.h"
#import "ClientForKernelspace.h"
#import "NotificationKeys.h"
#import "ParameterOutlineView.h"
#import "ParameterOutlineViewDataSource.h"
#import "ParameterOutlineViewDelegate.h"
#import "PreferencesKeys.h"
#import "PreferencesManager.h"
#import "PreferencesModel.h"
#import "ProfileCellView.h"
#import "ProfileTableView.h"
#import "ProfileTableViewDataSource.h"
#import "ProfileTableViewDelegate.h"
#import "Relauncher.h"
#import "ServerController.h"
#import "ServerForUserspace.h"
#import "ServerObjects.h"
#import "SharedUtilities.h"
#import "StatusMessageManager.h"
#import "Updater.h"
#import "XMLCompiler.h"

#include <sys/sysctl.h>
#include <sys/types.h>

@interface PreferencesWindowController ()

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
@property(weak) IBOutlet PreferencesModel* preferencesModel;
@property(weak) IBOutlet ParameterOutlineView* parameterOutlineView;
@property(weak) IBOutlet ParameterOutlineViewDelegate* parameterOutlineViewDelegate;
@property(weak) IBOutlet ParameterOutlineViewDataSource* parameterOutlineViewDataSource;
@property(weak) IBOutlet ProfileTableView* profileTableView;
@property(weak) IBOutlet ProfileTableViewDataSource* profileTableViewDataSource;
@property(weak) IBOutlet ProfileTableViewDelegate* profileTableViewDelegate;
@property NSTimer* resizeTimer;

@end

@implementation PreferencesWindowController

- (void)observer_ConfigListChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self drawEnabledCount];
    [self refreshKeyRepeatTab:nil];

    if ([self.checkbox_showEnabledOnly state] == NSOnState) {
      [self.checkboxOutlineViewDataSource clearFilterCondition];
      [self filterCheckboxOutlineView:self];
    } else {
      [self.checkboxOutlineView reloadData];
    }

    [self.parameterOutlineView reloadData];

    if (notification.userInfo && notification.userInfo[kPreferencesChangedNotificationUserInfoKeyPreferencesChangedFromGUI]) {
      // do nothing
    } else {
      [self.profileTableView reloadData];
    }
  });
}

- (void)observer_ConfigXMLReloaded:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self drawEnabledCount];
    // refreshKeyRepeatTab is not needed here.

    [self.checkboxOutlineViewDataSource load:YES];
    [self.parameterOutlineViewDataSource load:YES];

    [self.checkboxOutlineViewDelegate clearHeightCache];
    [self.checkboxOutlineViewDataSource clearFilterCondition];
    [self filterCheckboxOutlineView:self];

    [self.parameterOutlineView reloadData];
    [self expandParameterOutlineView:self];
  });
}

- (void)observer_PreferencesChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self drawEnabledCount];
    [self refreshKeyRepeatTab:nil];

    if (notification.userInfo && notification.userInfo[kPreferencesChangedNotificationUserInfoKeyPreferencesChangedFromGUI]) {
      // do nothing
    } else {
      [self.checkboxOutlineView reloadData];
      [self.parameterOutlineView reloadData];
    }
  });
}

- (instancetype)initWithServerObjects:(NSString*)windowNibName serverObjects:(ServerObjects*)serverObjects {
  self = [super initWithWindowNibName:windowNibName];

  if (self) {
    self.serverObjects = serverObjects;

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_ConfigListChanged:)
                                                 name:kConfigListChangedNotification
                                               object:nil];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_ConfigXMLReloaded:)
                                                 name:kConfigXMLReloadedNotification
                                               object:nil];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_PreferencesChanged:)
                                                 name:kPreferencesChangedNotification
                                               object:nil];

    // Show icon in Dock only when Preferences has been opened.
    if ([[NSUserDefaults standardUserDefaults] boolForKey:kShowIconInDock]) {
      ProcessSerialNumber psn = {0, kCurrentProcess};
      TransformProcessType(&psn, kProcessTransformToForegroundApplication);
    }
  }

  return self;
}

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

/* ---------------------------------------------------------------------- */
- (void)windowDidLoad {
  [self.serverObjects.serverForUserspace loadPreferencesModel:self.preferencesModel];

  [self expandParameterOutlineView:self];
}

/* ---------------------------------------------------------------------- */
- (void)drawVersion {
  NSString* version = [[NSBundle mainBundle] infoDictionary][@"CFBundleVersion"];
  [self.versionText setStringValue:version];
}

- (void)drawEnabledCount {
  // Calculating enabled_count is a bit heavy.
  // So, we skip this calculation if the preferences window was invisible.
  if (![self.window isVisible]) return;

  int count = [self enabled_count:self.serverObjects.xmlCompiler.preferencepane_checkbox
                          changed:[self.serverObjects.preferencesManager changed]];

  [self.checkbox_showEnabledOnly setTitle:[NSString stringWithFormat:@"show enabled only (%d %@)", count, count >= 2 ? @"items" : @"item"]];
}

/* ---------------------------------------------------------------------- */
- (int)enabled_count:(XMLCompilerTree*)tree changed:(NSDictionary*)changed {
  int count = 0;

  if (tree) {
    CheckboxItem* checkboxItem = [tree.node castToCheckboxItem];
    NSString* identifier = [checkboxItem getIdentifier];
    if ([identifier length] > 0) {
      if ([changed[identifier] intValue] != 0) {
        ++count;
      }
    }

    if (tree.children) {
      for (XMLCompilerTree* child in tree.children) {
        count += [self enabled_count:child changed:changed];
      }
    }
  }

  return count;
}

/* ---------------------------------------------------------------------- */
- (void)sendStatusWindowPreferencesNotification:(NSString*)identifier {
  if ([identifier isEqualToString:@"StatusMessage"]) {
    [[NSNotificationCenter defaultCenter] postNotificationName:kStatusWindowPreferencesOpenedNotification object:nil];
  } else {
    [[NSNotificationCenter defaultCenter] postNotificationName:kStatusWindowPreferencesClosedNotification object:nil];
  }
}

/* ---------------------------------------------------------------------- */
- (void)windowDidBecomeMain:(NSNotification*)notification {
  [self drawVersion];
  [self drawEnabledCount];
  [self refreshKeyRepeatTab:nil];
  [self updateDebugModeGuide];
  [self sendStatusWindowPreferencesNotification:[[self.tabView selectedTabViewItem] identifier]];
}

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
  [self.serverObjects.serverForUserspace savePreferencesModel:self.preferencesModel processIdentifier:[NSProcessInfo processInfo].processIdentifier];
}

- (IBAction)preferencesChanged:(id)sender {
  [self savePreferencesModel];
}

- (IBAction)resumeAtLoginChanged:(id)sender {
  [self savePreferencesModel];
  [self.serverObjects.serverForUserspace updateStartAtLogin];
}

- (void)show {
  [self.window makeKeyAndOrderFront:self];
  [NSApp activateIgnoringOtherApps:YES];
}

- (IBAction)focusSearchField:(id)sender {
  [self.tabView selectFirstTabViewItem:sender];
  [self.window makeFirstResponder:self.checkboxSearchText];
}

- (IBAction)reloadXML:(id)sender {
  [self.serverObjects.xmlCompiler reload];
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

- (IBAction)setCheckboxOutlineViewFont:(id)sender {
  [self.checkboxOutlineViewDelegate updateFont];
  [self.checkboxOutlineViewDelegate clearHeightCache];
  [self.checkboxOutlineView reloadData];
}

- (IBAction)quitWithConfirmation:(id)sender {
  if ([SharedUtilities confirmQuit]) {
    @try {
      [self.serverObjects.serverForUserspace terminateServerProcess];
    } @catch (NSException* exception) {
    }

    // TODO: uncomment this line after separate preferences window to another process.
    //[NSApp terminate:nil];
  }
}

- (IBAction)expandParameterOutlineView:(id)sender {
  [self.parameterOutlineView expandItem:nil expandChildren:YES];
}

- (IBAction)collapseParameterOutlineView:(id)sender {
  [self.parameterOutlineView collapseItem:nil collapseChildren:YES];
}

- (IBAction)updateStatusBar:(id)sender {
  [[NSNotificationCenter defaultCenter] postNotificationName:kStatusBarConfigurationChangedNotification object:nil];
}

- (IBAction)addNewProfile:(id)sender {
  [self.serverObjects.preferencesManager configlist_append];
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
  [self.serverObjects.preferencesManager configlist_sortByName];
}

- (IBAction)sortProfilesByCreated:(id)sender {
  [self disableAllProfileCells];
  [self.serverObjects.preferencesManager configlist_sortByAppendIndex];
}

- (IBAction)openURL:(id)sender {
  [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[sender title]]];
}

- (IBAction)openPrivateXMLUsageURL:(id)sender {
  NSString* url = @"https://pqrs.org/osx/karabiner/document.html#privatexml";
  [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:url]];
}

- (IBAction)refreshKeyRepeatTab:(id)sender {
  BOOL enabled = [[NSUserDefaults standardUserDefaults] boolForKey:kIsOverwriteKeyRepeat];

  if (enabled) {
    [self.keyRepeatParameters selectTabViewItemAtIndex:1];
  } else {
    [self.keyRepeatParameters selectTabViewItemAtIndex:0];
  }

  [self.delayUntilRepeatTextField setEnabled:enabled];
  [self.delayUntilRepeatStepper setEnabled:enabled];
  [self.keyRepeatTextField setEnabled:enabled];
  [self.keyRepeatStepper setEnabled:enabled];

  int delayUntilRepeat = [self.serverObjects.preferencesManager value:@"repeat.initial_wait"];
  int keyRepeat = [self.serverObjects.preferencesManager value:@"repeat.wait"];

  [self.delayUntilRepeatLabel setStringValue:[NSString stringWithFormat:@"%d milliseconds", delayUntilRepeat]];
  [self.delayUntilRepeatTextField setIntegerValue:delayUntilRepeat];
  [self.delayUntilRepeatStepper setIntegerValue:delayUntilRepeat];
  [self.keyRepeatLabel setStringValue:[NSString stringWithFormat:@"%d milliseconds", keyRepeat]];
  [self.keyRepeatTextField setIntegerValue:keyRepeat];
  [self.keyRepeatStepper setIntegerValue:keyRepeat];
}

- (IBAction)openSystemPreferencesKeyboard:(id)sender {
  [AppLauncher openSystemPreferencesKeyboard];
}

- (IBAction)changeDelayUntilRepeat:(id)sender {
  [self.serverObjects.preferencesManager setValue:[sender intValue] forName:@"repeat.initial_wait"];
  [self refreshKeyRepeatTab:sender];
}

- (IBAction)changeKeyRepeat:(id)sender {
  [self.serverObjects.preferencesManager setValue:[sender intValue] forName:@"repeat.wait"];
  [self refreshKeyRepeatTab:sender];
}

- (IBAction)refreshStatusMessage:(id)sender {
  [self.serverObjects.statusMessageManager refresh];
}

- (IBAction)restartAXNotifier:(id)sender {
  if (![[NSUserDefaults standardUserDefaults] boolForKey:kIsAXNotifierEnabled]) {
    NSAlert* alert = [NSAlert new];
    [alert setMessageText:@"Karabiner Alert"];
    [alert addButtonWithTitle:@"Close"];
    [alert setInformativeText:@"AXNotifier is disabled.\nPlease enable AXNotifier if you want to start."];
    [alert runModal];
    return;
  }

  [AXNotifierManager restartAXNotifier];
}

- (IBAction)restartAXNotifierWithoutAlert:(id)sender {
  [AXNotifierManager restartAXNotifier];
}

- (IBAction)checkForUpdatesStableOnly:(id)sender {
  [self.serverObjects.updater checkForUpdatesStableOnly];
}

- (IBAction)checkForUpdatesWithBetaVersion:(id)sender {
  [self.serverObjects.updater checkForUpdatesWithBetaVersion];
}

- (IBAction)launchUninstaller:(id)sender {
  [AppLauncher openUninstaller];
}

- (IBAction)openPrivateXML:(id)sender {
  [AppLauncher openPrivateXMLDirectory];
}

- (IBAction)launchEventViewer:(id)sender {
  [AppLauncher openEventViewer];
}

- (IBAction)launchMultiTouchExtension:(id)sender {
  [AppLauncher openMultiTouchExtension];
}

- (IBAction)openConsoleApp:(id)sender {
  [[NSWorkspace sharedWorkspace] openFile:@"/Applications/Utilities/Console.app"];
}

- (IBAction)relaunch:(id)sender {
  [Relauncher relaunch];
}

- (IBAction)toggleDebugMode:(id)sender {
  if ([self isDebugMode]) {
    [self.serverObjects.clientForKernelspace unset_debug_flags];
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
