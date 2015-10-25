#import "GlobalDomainKeyRepeatObserver.h"
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

- (instancetype)initWithPreferencesManager:(PreferencesManager*)manager {
  self = [super init];

  if (self) {
    preferencesManager_ = manager;
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
  if ([[NSUserDefaults standardUserDefaults] boolForKey:kIsOverwriteKeyRepeat]) {
    previousInitialKeyRepeat_ = -1;
    previousKeyRepeat_ = -1;
    return;
  }

  NSDictionary* dictionary = [[NSUserDefaults standardUserDefaults] persistentDomainForName:NSGlobalDomain];
  int currentInitialKeyRepeat = [self getInitialKeyRepeatFromDictionary:dictionary];
  int currentKeyRepeat = [self getKeyRepeatFromDictionary:dictionary];

  if (previousInitialKeyRepeat_ != currentInitialKeyRepeat) {
    [preferencesManager_ setValue:currentInitialKeyRepeat forName:@"repeat.initial_wait"];
    previousInitialKeyRepeat_ = currentInitialKeyRepeat;
  }
  if (previousKeyRepeat_ != currentKeyRepeat) {
    [preferencesManager_ setValue:currentKeyRepeat forName:@"repeat.wait"];
    previousKeyRepeat_ = currentKeyRepeat;
  }
}

- (int)getInitialKeyRepeatFromDictionary:(NSDictionary*)dictionary {
  if (!dictionary) {
    return 0;
  }
  // The unit of InitialKeyRepeat is 1/60 second.
  return (int)([dictionary[@"InitialKeyRepeat"] floatValue] * 1000 / 60);
}

- (int)getKeyRepeatFromDictionary:(NSDictionary*)dictionary {
  if (!dictionary) {
    return 0;
  }
  // The unit of KeyRepeat is 1/60 second.
  return (int)([dictionary[@"KeyRepeat"] floatValue] * 1000 / 60);
}

- (int)getInitialKeyRepeat {
  NSDictionary* dictionary = [[NSUserDefaults standardUserDefaults] persistentDomainForName:NSGlobalDomain];
  return [self getInitialKeyRepeatFromDictionary:dictionary];
}

- (int)getKeyRepeat {
  NSDictionary* dictionary = [[NSUserDefaults standardUserDefaults] persistentDomainForName:NSGlobalDomain];
  return [self getKeyRepeatFromDictionary:dictionary];
}

@end
