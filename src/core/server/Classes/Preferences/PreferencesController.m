/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "NotificationKeys.h"
#import "PreferencesController.h"
#import "PreferencesManager.h"
#import "XMLCompiler.h"

@implementation PreferencesController

- (void) observer_ConfigListChanged:(NSNotification*)notification
{
  [self drawEnabledCount];
}

- (void) observer_ConfigXMLReloaded:(NSNotification*)notification
{
  [self drawEnabledCount];
}

- (void) observer_PreferencesChanged:(NSNotification*)notification
{
  [self drawEnabledCount];
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

  [super dealloc];
}

/* ---------------------------------------------------------------------- */
- (void) drawVersion
{
  NSString* version = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"];
  [versionText_ setStringValue:version];
}

- (void) drawEnabledCount
{
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
      NSString* identifier = [dict objectForKey:@"identifier"];
      if (identifier) {
        if ([[changed objectForKey:identifier] intValue] != 0) {
          ++count;
        }
      }

      count += [self enabled_count:[dict objectForKey:@"children"] changed:changed];
    }
  }

  return count;
}

/* ---------------------------------------------------------------------- */
- (void) windowDidBecomeMain:(NSNotification*)notification
{
  [self drawVersion];
  [self drawEnabledCount];
}

/* ---------------------------------------------------------------------- */
- (void) show
{
  [preferencesWindow_ performSelectorOnMainThread:@selector(makeKeyAndOrderFront:)
                                       withObject:nil
                                    waitUntilDone:YES];
  [NSApp activateIgnoringOtherApps:YES];
}

@end
