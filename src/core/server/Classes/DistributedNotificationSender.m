#import "DistributedNotificationSender.h"
#import "KeyRemap4MacBookKeys.h"
#import "KeyRemap4MacBookNSDistributedNotificationCenter.h"
#import "NotificationKeys.h"

@implementation DistributedNotificationSender

- (void) observer_ConfigXMLReloaded:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
                   [KeyRemap4MacBookNSDistributedNotificationCenter postNotificationName:kKeyRemap4MacBookConfigXMLReloadedNotification userInfo:nil];
                 });
}

- (void) observer_PreferencesChanged:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
                   [KeyRemap4MacBookNSDistributedNotificationCenter postNotificationName:kKeyRemap4MacBookPreferencesChangedNotification userInfo:nil];
                 });
}

- (id) init
{
  self = [super init];

  if (self) {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_ConfigXMLReloaded:)
                                                 name:kConfigXMLReloadedNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_PreferencesChanged:)
                                                 name:kPreferencesChangedNotification object:nil];
  }

  return self;
}

- (void) dealloc
{
  [[NSNotificationCenter defaultCenter] removeObserver:self];

  [super dealloc];
}

@end
