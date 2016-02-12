/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "CheckboxOutlineViewDataSource.h"
#import "CheckboxOutlineViewDelegate.h"
#import "ClientForKernelspace.h"
#import "NotificationKeys.h"
#import "ParameterOutlineViewDataSource.h"
#import "PreferencesController.h"
#import "PreferencesKeys.h"
#import "PreferencesManager.h"
#import "XMLCompiler.h"

#include <sys/types.h>
#include <sys/sysctl.h>

@interface PreferencesController ()
@property(weak) IBOutlet CheckboxOutlineViewDataSource* checkboxOutlineViewDataSource;
@property(weak) IBOutlet CheckboxOutlineViewDelegate* checkboxOutlineViewDelegate;
@property(weak) IBOutlet NSOutlineView* checkboxOutlineView;
@property(weak) IBOutlet NSOutlineView* parameterOutlineView;
@property(weak) IBOutlet NSSearchField* checkboxSearchText;
@property(weak) IBOutlet NSSegmentedControl* checkboxFontSegmentedControl;
@property(weak) IBOutlet ParameterOutlineViewDataSource* parameterOutlineViewDataSource;
@property NSTimer* resizeTimer;
@end

@implementation PreferencesController

- (void)observer_ConfigListChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self drawEnabledCount];
    [self refreshKeyRepeatTab:nil];

    if ([checkbox_showEnabledOnly_ state] == NSOnState) {
      [self.checkboxOutlineViewDataSource clearFilterCondition];
      [self filterCheckboxOutlineView:self];
    } else {
      [self.checkboxOutlineView reloadData];
    }

    [self.parameterOutlineView reloadData];
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

- (id)init {
  self = [super init];

  if (self) {
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
  }

  return self;
}

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

/* ---------------------------------------------------------------------- */
- (void)drawVersion {
  NSString* version = [[NSBundle mainBundle] infoDictionary][@"CFBundleVersion"];
  [versionText_ setStringValue:version];
}

- (void)drawEnabledCount {
  // Calculating enabled_count is a bit heavy.
  // So, we skip this calculation if the preferences window was invisible.
  if (![preferencesWindow_ isVisible]) return;

  int count = [self enabled_count:xmlCompiler_.preferencepane_checkbox
                          changed:[preferencesManager_ changed]];

  [checkbox_showEnabledOnly_ setTitle:[NSString stringWithFormat:@"show enabled only (%d %@)", count, count >= 2 ? @"items" : @"item"]];
}

/* ---------------------------------------------------------------------- */
- (int)enabled_count:(XMLCompilerTree*)tree changed:(NSDictionary*)changed {
  int count = 0;

  if (tree) {
    CheckboxItem* checkboxItem = [tree castNodeToCheckboxItem];
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
  [self sendStatusWindowPreferencesNotification:[[tabView_ selectedTabViewItem] identifier]];
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
- (void)show {
  [preferencesWindow_ makeKeyAndOrderFront:self];
  [NSApp activateIgnoringOtherApps:YES];
}

- (IBAction)reloadXML:(id)sender {
  [xmlCompiler_ reload];
}

- (IBAction)filterCheckboxOutlineView:(id)sender {
  BOOL isEnabledOnly = ([checkbox_showEnabledOnly_ state] == NSOnState);
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

- (IBAction)expandParameterOutlineView:(id)sender {
  [self.parameterOutlineView expandItem:nil expandChildren:YES];
}

- (IBAction)collapseParameterOutlineView:(id)sender {
  [self.parameterOutlineView collapseItem:nil collapseChildren:YES];
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
    [keyRepeatParameters_ selectTabViewItemAtIndex:1];
  } else {
    [keyRepeatParameters_ selectTabViewItemAtIndex:0];
  }

  [delayUntilRepeatTextField_ setEnabled:enabled];
  [delayUntilRepeatStepper_ setEnabled:enabled];
  [keyRepeatTextField_ setEnabled:enabled];
  [keyRepeatStepper_ setEnabled:enabled];

  int delayUntilRepeat = [preferencesManager_ value:@"repeat.initial_wait"];
  int keyRepeat = [preferencesManager_ value:@"repeat.wait"];

  [delayUntilRepeatLabel_ setStringValue:[NSString stringWithFormat:@"%d milliseconds", delayUntilRepeat]];
  [delayUntilRepeatTextField_ setIntegerValue:delayUntilRepeat];
  [delayUntilRepeatStepper_ setIntegerValue:delayUntilRepeat];
  [keyRepeatLabel_ setStringValue:[NSString stringWithFormat:@"%d milliseconds", keyRepeat]];
  [keyRepeatTextField_ setIntegerValue:keyRepeat];
  [keyRepeatStepper_ setIntegerValue:keyRepeat];
}

- (IBAction)openSystemPreferencesKeyboard:(id)sender {
  [[NSWorkspace sharedWorkspace] openFile:@"/System/Library/PreferencePanes/Keyboard.prefPane"];
}

- (IBAction)changeDelayUntilRepeat:(id)sender {
  [preferencesManager_ setValue:[sender intValue] forName:@"repeat.initial_wait"];
  [self refreshKeyRepeatTab:sender];
}

- (IBAction)changeKeyRepeat:(id)sender {
  [preferencesManager_ setValue:[sender intValue] forName:@"repeat.wait"];
  [self refreshKeyRepeatTab:sender];
}

- (IBAction)openConsoleApp:(id)sender {
  [[NSWorkspace sharedWorkspace] openFile:@"/Applications/Utilities/Console.app"];
}

- (IBAction)toggleDebugMode:(id)sender {
  if ([self isDebugMode]) {
    [clientForKernelspace_ unset_debug_flags];
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
    [debugModeGuideTextField_ setStringValue:@"Debug Mode is enabled.\nDo not type sensitive information such as password.\nYour key input are saved into system.log."];
    // #a94442
    [debugModeGuideTextField_ setTextColor:[NSColor colorWithCalibratedRed:(0xa9 * 1.0 / 255)
                                                                     green:(0x44 * 1.0 / 255)
                                                                      blue:(0x42 * 1.0 / 255)
                                                                     alpha:1.0]];
    // #f2dede
    [debugModeGuideTextField_ setBackgroundColor:[NSColor colorWithCalibratedRed:(0xf2 * 1.0 / 255)
                                                                           green:(0xde * 1.0 / 255)
                                                                            blue:(0xde * 1.0 / 255)
                                                                           alpha:1.0]];
    debugModeGuideTextField_.drawsBackground = YES;

  } else {
    [debugModeGuideTextField_ setStringValue:@"Debug Mode is disabled.\nAdministrator privileges are required to enable Debug Mode."];
    [debugModeGuideTextField_ setTextColor:[NSColor textColor]];
    debugModeGuideTextField_.drawsBackground = NO;
  }
}

@end
