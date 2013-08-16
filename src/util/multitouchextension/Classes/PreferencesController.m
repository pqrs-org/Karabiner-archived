// -*- Mode: objc -*-

#import "PreferencesController.h"
#import "StartAtLoginController.h"

@implementation PreferencesController

+ (void) initialize
{
  NSDictionary* dict = @{
    @"hideIconInDock" : @NO,
    @"relaunchAfterWakeUpFromSleep" : @YES,
    @"relaunchWait" : @"3",
    @"targetSettingIsEnabled1" : @YES,
    @"targetSettingIsEnabled2" : @NO,
    @"targetSettingIsEnabled3" : @NO,
    @"targetSettingIsEnabled4" : @NO,
    @"targetSetting1" : @"notsave.thumbsense",
    @"targetSetting2" : @"notsave.enhanced_copyandpaste",
    @"targetSetting3" : @"notsave.pointing_relative_to_scroll",
    @"targetSetting4" : @"notsave.pointing_relative_to_scroll",
    @"ignoredAreaTop"    : @"0",
    @"ignoredAreaBottom" : @"0",
    @"ignoredAreaLeft"   : @"0",
    @"ignoredAreaRight"  : @"0",
  };
  [[NSUserDefaults standardUserDefaults] registerDefaults:dict];
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
  return [[NSUserDefaults standardUserDefaults] boolForKey:[NSString stringWithFormat:@"targetSettingIsEnabled%d", (int)(fingers)]];
}

+ (NSString*) getSettingName:(NSInteger)fingers
{
  return [[NSUserDefaults standardUserDefaults] stringForKey:[NSString stringWithFormat:@"targetSetting%d", (int)(fingers)]];
}

- (IBAction) set:(id)sender
{
  // ------------------------------------------------------------
  // disable old settings
  for (NSString* name in oldSettings_) {
    @try {
      [[client_ proxy] setValueForName:0 forName:name];
    } @catch (NSException* exception) {
      NSLog(@"%@", exception);
    }
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
