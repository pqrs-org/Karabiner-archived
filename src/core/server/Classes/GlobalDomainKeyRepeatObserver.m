#import "GlobalDomainKeyRepeatObserver.h"
#import "NotificationKeys.h"
#import "PreferencesKeys.h"
#import "PreferencesManager.h"
#import "PreferencesModel.h"

@interface GlobalDomainKeyRepeatObserver ()

@property(weak) IBOutlet PreferencesManager* preferencesManager;
@property(weak) IBOutlet PreferencesModel* preferencesModel;
@property NSTimer* timer;
@property int previousInitialKeyRepeat;
@property int previousKeyRepeat;

@end

@implementation GlobalDomainKeyRepeatObserver

- (void)observer_PreferencesChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    self.previousInitialKeyRepeat = -1;
    self.previousKeyRepeat = -1;
  });
}

- (instancetype)init {
  self = [super init];

  if (self) {
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

- (void)start {
  self.timer = [NSTimer scheduledTimerWithTimeInterval:1
                                                target:self
                                              selector:@selector(timerFireMethod:)
                                              userInfo:nil
                                               repeats:YES];
}

- (void)timerFireMethod:(NSTimer*)timer {
  dispatch_async(dispatch_get_main_queue(), ^{
    if (self.preferencesModel.overrideKeyRepeat) {
      self.previousInitialKeyRepeat = -1;
      self.previousKeyRepeat = -1;
      return;
    }

    NSDictionary* dictionary = [[NSUserDefaults standardUserDefaults] persistentDomainForName:NSGlobalDomain];
    int currentInitialKeyRepeat = [self getInitialKeyRepeatFromDictionary:dictionary];
    int currentKeyRepeat = [self getKeyRepeatFromDictionary:dictionary];

    if (self.previousInitialKeyRepeat != currentInitialKeyRepeat) {
      self.previousInitialKeyRepeat = currentInitialKeyRepeat;

      NSString* name = @"repeat.initial_wait";
      if ([self.preferencesModel value:name] != currentInitialKeyRepeat) {
        NSLog(@"Set %@ from NSGlobalDomain.InitialKeyRepeat: %d milliseconds", name, currentInitialKeyRepeat);
        [self.preferencesModel setValue:currentInitialKeyRepeat forName:name];
        [self.preferencesManager save];
        [self.preferencesManager updateKextValue:name];
      }
    }
    if (self.previousKeyRepeat != currentKeyRepeat) {
      self.previousKeyRepeat = currentKeyRepeat;

      NSString* name = @"repeat.wait";
      if ([self.preferencesModel value:name] != currentKeyRepeat) {
        NSLog(@"Set %@ from NSGlobalDomain.KeyRepeat: %d milliseconds", name, currentKeyRepeat);
        [self.preferencesModel setValue:currentKeyRepeat forName:name];
        [self.preferencesManager save];
        [self.preferencesManager updateKextValue:name];
      }
    }
  });
}

- (int)getInitialKeyRepeatFromDictionary:(NSDictionary*)dictionary {
  // If System Preferences has never changed, dictionary[@"InitialKeyRepeat"] is nil.
  // (We can confirm in Guest account.)
  if (!dictionary || !dictionary[@"InitialKeyRepeat"]) {
    return (int)([self.preferencesModel defaultValue:@"repeat.initial_wait"]);
  }
  // The unit of InitialKeyRepeat is 1/60 second.
  return (int)([dictionary[@"InitialKeyRepeat"] floatValue] * 1000 / 60);
}

- (int)getKeyRepeatFromDictionary:(NSDictionary*)dictionary {
  // If System Preferences has never changed, dictionary[@"KeyRepeat"] is nil.
  // (We can confirm in Guest account.)
  if (!dictionary || !dictionary[@"KeyRepeat"]) {
    return (int)([self.preferencesModel defaultValue:@"repeat.wait"]);
  }
  // The unit of KeyRepeat is 1/60 second.
  return (int)([dictionary[@"KeyRepeat"] floatValue] * 1000 / 60);
}

@end
