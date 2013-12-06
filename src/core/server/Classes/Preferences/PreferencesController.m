/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "NotificationKeys.h"
#import "PreferencesController.h"
#import "PreferencesManager.h"
#import "XMLCompiler.h"

@implementation PreferencesController

- (void) observer_ConfigListChanged:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    [self drawEnabledCount];
  });
}

- (void) observer_ConfigXMLReloaded:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    [self drawEnabledCount];
  });
}

- (void) observer_PreferencesChanged:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    [self drawEnabledCount];
  });
}

- (id) init
{
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

- (void) dealloc
{
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

/* ---------------------------------------------------------------------- */
- (void) drawVersion
{
  NSString* version = [[NSBundle mainBundle] infoDictionary][@"CFBundleVersion"];
  [versionText_ setStringValue:version];
}

- (void) drawEnabledCount
{
  // Calculating enabled_count is a bit heavy.
  // So, we skip this calculation if the preferences window was invisible.
  if (! [preferencesWindow_ isVisible]) return;

  int count = [self enabled_count:[xmlCompiler_ preferencepane_checkbox]
                          changed:[preferencesManager_ changed]];

  [checkbox_showEnabledOnly_ setTitle:[NSString stringWithFormat:@"show enabled only (%d %@)", count, count >= 2 ? @"items":@"item"]];
}

/* ---------------------------------------------------------------------- */
- (int) enabled_count:(NSArray*)checkbox changed:(NSDictionary*)changed
{
  int count = 0;

  if (checkbox) {
    for (NSDictionary* dict in checkbox) {
      NSString* identifier = dict[@"identifier"];
      if (identifier) {
        if ([changed[identifier] intValue] != 0) {
          ++count;
        }
      }

      count += [self enabled_count:dict[@"children"] changed:changed];
    }
  }

  return count;
}

/* ---------------------------------------------------------------------- */
- (void) sendStatusWindowPreferencesNotification:(NSString*)identifier
{
  if ([identifier isEqualToString:@"StatusMessage"]) {
    [[NSNotificationCenter defaultCenter] postNotificationName:kStatusWindowPreferencesOpenedNotification object:nil];
  } else {
    [[NSNotificationCenter defaultCenter] postNotificationName:kStatusWindowPreferencesClosedNotification object:nil];
  }
}

/* ---------------------------------------------------------------------- */
- (void) windowDidBecomeMain:(NSNotification*)notification
{
  [self drawVersion];
  [self drawEnabledCount];
  [self sendStatusWindowPreferencesNotification:[[tabView_ selectedTabViewItem] identifier]];
}

- (void) windowWillClose:(NSNotification*)notification
{
  [self sendStatusWindowPreferencesNotification:nil];
}

/* ---------------------------------------------------------------------- */
- (void) tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem
{
  [self sendStatusWindowPreferencesNotification:[tabViewItem identifier]];
}

/* ---------------------------------------------------------------------- */
- (void) show
{
  [preferencesWindow_ makeKeyAndOrderFront:self];
  [NSApp activateIgnoringOtherApps:YES];
}

@end
