// -*- Mode: objc -*-
//
//  KeyRemap4MacBook_multitouchextensionAppDelegate.h
//  KeyRemap4MacBook_multitouchextension
//
//  Created by Takayama Fumihiko on 09/11/24.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "PreferencesController.h"
#import "StartAtLoginController.h"

@implementation PreferencesController

+ (void) initialize
{
  NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                        @"YES",
                        @"targetSettingIsEnabled1",

                        @"NO",
                        @"targetSettingIsEnabled2",

                        @"NO",
                        @"targetSettingIsEnabled3",

                        @"notsave.complete_vimode",
                        @"targetSetting1",

                        @"notsave.mousekeys_mode",
                        @"targetSetting2",

                        @"notsave.pointing_relative_to_scroll",
                        @"targetSetting3",

                        nil];

  [[NSUserDefaults standardUserDefaults] registerDefaults:dict];
}

- (void) load
{
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];

  NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                        targetSettingIsEnabled1_, @"targetSettingIsEnabled1",
                        targetSettingIsEnabled2_, @"targetSettingIsEnabled2",
                        targetSettingIsEnabled3_, @"targetSettingIsEnabled3",
                        targetSetting1_, @"targetSetting1",
                        targetSetting2_, @"targetSetting2",
                        targetSetting3_, @"targetSetting3",
                        nil];
  for (NSString* key in dict) {
    id object = [dict objectForKey:key];

    if ([object isKindOfClass:[NSButton class]]) {
      NSButton* button = object;
      if ([[defaults stringForKey:key] isEqualToString:@"YES"]) {
        [button setState:NSOnState];
      } else {
        [button setState:NSOffState];
      }

    } else if ([object isKindOfClass:[NSTextField class]]) {
      NSTextField* text = object;
      [text setStringValue:[defaults stringForKey:key]];
    }
  }

  if ([StartAtLoginController isStartAtLogin]) {
    [startAtLogin_ setState:NSOnState];
  } else {
    [startAtLogin_ setState:NSOffState];
  }
}

- (void) show
{
  [preferencesWindow_ makeKeyAndOrderFront:nil];
}

- (IBAction) setStartAtLogin:(id)sender
{
  // startAtLogin
  if ([StartAtLoginController isStartAtLogin]) {
    [StartAtLoginController setStartAtLogin:NO];
  } else {
    [StartAtLoginController setStartAtLogin:YES];
  }
}

- (IBAction) set:(id)sender
{
  NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];

  [defaults setObject:([targetSettingIsEnabled1_ state] == NSOnState ? @"YES":@"NO") forKey:@"targetSettingIsEnabled1"];
  [defaults setObject:([targetSettingIsEnabled2_ state] == NSOnState ? @"YES":@"NO") forKey:@"targetSettingIsEnabled2"];
  [defaults setObject:([targetSettingIsEnabled3_ state] == NSOnState ? @"YES":@"NO") forKey:@"targetSettingIsEnabled3"];
  [defaults setObject:[targetSetting1_ stringValue] forKey:@"targetSetting1"];
  [defaults setObject:[targetSetting2_ stringValue] forKey:@"targetSetting2"];
  [defaults setObject:[targetSetting3_ stringValue] forKey:@"targetSetting3"];
}

- (void) windowWillClose:(NSNotification*)notification
{
  [self set:nil];
}

@end
