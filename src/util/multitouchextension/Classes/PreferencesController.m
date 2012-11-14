// -*- Mode: objc -*-

#import "PreferencesController.h"
#import "StartAtLoginController.h"

@implementation PreferencesController

NSDictionary* defaults_dictionary = nil;

+ (void) initialize
{
  defaults_dictionary = [NSDictionary dictionaryWithObjectsAndKeys:
                         [NSNumber numberWithBool:NO],
                         @"hideIconInDock",

                         [NSNumber numberWithBool:YES],
                         @"targetSettingIsEnabled1",

                         [NSNumber numberWithBool:NO],
                         @"targetSettingIsEnabled2",

                         [NSNumber numberWithBool:NO],
                         @"targetSettingIsEnabled3",

                         [NSNumber numberWithBool:NO],
                         @"targetSettingIsEnabled4",

                         @"notsave.thumbsense",
                         @"targetSetting1",

                         @"notsave.enhanced_copyandpaste",
                         @"targetSetting2",

                         @"notsave.pointing_relative_to_scroll",
                         @"targetSetting3",

                         @"notsave.pointing_relative_to_scroll",
                         @"targetSetting4",

                         @"0",
                         @"ignoredAreaTop",

                         @"0",
                         @"ignoredAreaBottom",

                         @"0",
                         @"ignoredAreaLeft",

                         @"0",
                         @"ignoredAreaRight",

                         nil];
  [defaults_dictionary retain];

  [[NSUserDefaults standardUserDefaults] registerDefaults:defaults_dictionary];
}

- (id) init
{
  self = [super init];

  if (self) {
    oldSettings_ = [NSMutableArray new];
  }

  return self;
}

- (void) dealloc
{
  [oldSettings_ release];

  [super dealloc];
}

- (void) load
{
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

+ (BOOL) isSettingEnabled:(NSInteger)fingers
{
  return [[NSUserDefaults standardUserDefaults] boolForKey:[NSString stringWithFormat:@"targetSettingIsEnabled%d", fingers]];
}

+ (NSString*) getSettingName:(NSInteger)fingers
{
  return [[NSUserDefaults standardUserDefaults] stringForKey:[NSString stringWithFormat:@"targetSetting%d", fingers]];
}

- (IBAction) set:(id)sender
{
  // ------------------------------------------------------------
  // disable old settings
  for (NSString* name in oldSettings_) {
    [[client_ proxy] setValueForName:0 forName:name];
  }

  [oldSettings_ removeAllObjects];
  for (int i = 1; i <= 4; ++i) {
    if ([PreferencesController isSettingEnabled:i]) {
      [oldSettings_ addObject:[PreferencesController getSettingName:i]];
    }
  }
}

- (void) windowWillClose:(NSNotification*)notification
{
  [self set:nil];
}

@end
