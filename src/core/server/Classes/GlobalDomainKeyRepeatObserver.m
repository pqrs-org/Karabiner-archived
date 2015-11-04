#import "GlobalDomainKeyRepeatObserver.h"
#import "NotificationKeys.h"
#import "PreferencesKeys.h"
#import "PreferencesManager.h"

@interface GlobalDomainKeyRepeatObserver () {
  PreferencesManager* preferencesManager_;
  NSTimer* timer_;

  int previousInitialKeyRepeat_;
  int previousKeyRepeat_;
}
@end

@implementation GlobalDomainKeyRepeatObserver

- (void)observer_PreferencesChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    previousInitialKeyRepeat_ = -1;
    previousKeyRepeat_ = -1;
  });
}

- (instancetype)initWithPreferencesManager:(PreferencesManager*)manager {
  self = [super init];

  if (self) {
    preferencesManager_ = manager;

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_PreferencesChanged:)
                                                 name:kPreferencesChangedNotification
                                               object:nil];
  }

  return self;
}

- (void)start {
  timer_ = [NSTimer scheduledTimerWithTimeInterval:1
                                            target:self
                                          selector:@selector(timerFireMethod:)
                                          userInfo:nil
                                           repeats:YES];
}

- (void)timerFireMethod:(NSTimer*)timer {
  dispatch_async(dispatch_get_main_queue(), ^{
    if ([[NSUserDefaults standardUserDefaults] boolForKey:kIsOverwriteKeyRepeat]) {
      previousInitialKeyRepeat_ = -1;
      previousKeyRepeat_ = -1;
      return;
    }

    NSDictionary* dictionary = [[NSUserDefaults standardUserDefaults] persistentDomainForName:NSGlobalDomain];
    int currentInitialKeyRepeat = [self getInitialKeyRepeatFromDictionary:dictionary];
    int currentKeyRepeat = [self getKeyRepeatFromDictionary:dictionary];

    if (previousInitialKeyRepeat_ != currentInitialKeyRepeat) {
      previousInitialKeyRepeat_ = currentInitialKeyRepeat;

      NSString* name = @"repeat.initial_wait";
      if ([preferencesManager_ value:name] != currentInitialKeyRepeat) {
        NSLog(@"Set %@ from NSGlobalDomain.InitialKeyRepeat: %d milliseconds", name, currentInitialKeyRepeat);
        [preferencesManager_ setValue:currentInitialKeyRepeat forName:name];
      }
    }
    if (previousKeyRepeat_ != currentKeyRepeat) {
      previousKeyRepeat_ = currentKeyRepeat;

      NSString* name = @"repeat.wait";
      if ([preferencesManager_ value:name] != currentKeyRepeat) {
        NSLog(@"Set %@ from NSGlobalDomain.KeyRepeat: %d milliseconds", name, currentKeyRepeat);
        [preferencesManager_ setValue:currentKeyRepeat forName:name];
      }
    }
  });
}

- (int)getInitialKeyRepeatFromDictionary:(NSDictionary*)dictionary {
  // If System Preferences has never changed, dictionary[@"InitialKeyRepeat"] is nil.
  // (We can confirm in Guest account.)
  if (!dictionary || !dictionary[@"InitialKeyRepeat"]) {
    return [preferencesManager_ defaultValue:@"repeat.initial_wait"];
  }
  // The unit of InitialKeyRepeat is 1/60 second.
  return (int)([dictionary[@"InitialKeyRepeat"] floatValue] * 1000 / 60);
}

- (int)getKeyRepeatFromDictionary:(NSDictionary*)dictionary {
  // If System Preferences has never changed, dictionary[@"KeyRepeat"] is nil.
  // (We can confirm in Guest account.)
  if (!dictionary || !dictionary[@"KeyRepeat"]) {
    return [preferencesManager_ defaultValue:@"repeat.wait"];
  }
  // The unit of KeyRepeat is 1/60 second.
  return (int)([dictionary[@"KeyRepeat"] floatValue] * 1000 / 60);
}

@end
