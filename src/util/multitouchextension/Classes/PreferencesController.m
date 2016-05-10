// -*- Mode: objc -*-

#import "PreferencesController.h"
#import "PreferencesKeys.h"
#import "PreferencesModel.h"
#import "ServerClient.h"
#import "SharedPreferencesManager.h"
#import "StartAtLoginUtilities.h"

@interface PreferencesController ()

@property NSMutableArray* oldSettings;
@property(weak) IBOutlet NSWindow* preferencesWindow;
@property(weak) IBOutlet ServerClient* client;
@property(weak) IBOutlet NSButton* startAtLoginCheckbox;
@property(weak) IBOutlet SharedPreferencesManager* sharedPreferencesManager;

@end

@implementation PreferencesController

+ (void)initialize {
  static dispatch_once_t once;
  dispatch_once(&once, ^{
    NSDictionary* dict = @{
      @"hideIconInDock" : @NO,
      @"relaunchAfterWakeUpFromSleep" : @YES,
      @"relaunchWait" : @"3",
      @"targetSettingIsEnabled1" : @NO,
      @"targetSettingIsEnabled2" : @NO,
      @"targetSettingIsEnabled3" : @NO,
      @"targetSettingIsEnabled4" : @NO,
      @"targetSetting1" : @"notsave.thumbsense",
      @"targetSetting2" : @"notsave.enhanced_copyandpaste",
      @"targetSetting3" : @"notsave.pointing_relative_to_scroll",
      @"targetSetting4" : @"notsave.pointing_relative_to_scroll",
      @"ignoredAreaTop" : @"0",
      @"ignoredAreaBottom" : @"0",
      @"ignoredAreaLeft" : @"0",
      @"ignoredAreaRight" : @"0",
      kDelayBeforeTurnOff : @"0",
      kDelayBeforeTurnOn : @"0",
    };
    [[NSUserDefaults standardUserDefaults] registerDefaults:dict];
  });
}

- (instancetype)init {
  self = [super init];

  if (self) {
    self.oldSettings = [NSMutableArray new];
  }

  return self;
}

- (void)load {
  if ([StartAtLoginUtilities isStartAtLogin]) {
    [self.startAtLoginCheckbox setState:NSOnState];
  } else {
    [self.startAtLoginCheckbox setState:NSOffState];
  }
}

- (void)show {
  [self.preferencesWindow makeKeyAndOrderFront:nil];
}

- (IBAction)setStartAtLogin:(id)sender {
  // startAtLogin
  if ([StartAtLoginUtilities isStartAtLogin]) {
    [StartAtLoginUtilities setStartAtLogin:NO];
  } else {
    [StartAtLoginUtilities setStartAtLogin:YES];
  }
}

+ (BOOL)isSettingEnabled:(NSInteger)fingers {
  return [[NSUserDefaults standardUserDefaults] boolForKey:[NSString stringWithFormat:@"targetSettingIsEnabled%d", (int)(fingers)]];
}

+ (NSString*)getSettingName:(NSInteger)fingers {
  return [[NSUserDefaults standardUserDefaults] stringForKey:[NSString stringWithFormat:@"targetSetting%d", (int)(fingers)]];
}

- (IBAction)set:(id)sender {
  // ------------------------------------------------------------
  // disable old settings
  [self.sharedPreferencesManager load];

  for (NSString* name in self.oldSettings) {
    @try {
      [self.sharedPreferencesManager setValue:0 forName:name];
    }
    @catch (NSException* exception) {
      NSLog(@"%@", exception);
    }
  }

  [self.oldSettings removeAllObjects];
  for (int i = 1; i <= 4; ++i) {
    if ([PreferencesController isSettingEnabled:i]) {
      [self.oldSettings addObject:[PreferencesController getSettingName:i]];
    }
  }
}

- (void)windowWillClose:(NSNotification*)notification {
  [self set:nil];
}

@end
