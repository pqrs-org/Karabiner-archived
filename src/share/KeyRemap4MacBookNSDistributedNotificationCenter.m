#import "KeyRemap4MacBookNSDistributedNotificationCenter.h"
#import "KeyRemap4MacBookKeys.h"

@implementation KeyRemap4MacBookNSDistributedNotificationCenter

+ (void) addObserver:(id)notificationObserver selector:(SEL)notificationSelector name:(NSString*)notificationName
{
  [KeyRemap4MacBookNSDistributedNotificationCenter addObserver:notificationObserver
                                                      selector:notificationSelector
                                                          name:notificationName
                                                        object:kKeyRemap4MacBookNotificationKey];
}

+ (void) addObserver:(id)notificationObserver selector:(SEL)notificationSelector name:(NSString*)notificationName object:(NSString*)notificationSender
{
  // In Mac OS X 10.7, NSDistributedNotificationCenter is suspended after calling [NSAlert runModal].
  // So, we need to set suspendedDeliveryBehavior to NSNotificationSuspensionBehaviorDeliverImmediately.

  [[NSDistributedNotificationCenter defaultCenter] addObserver:notificationObserver
                                                      selector:notificationSelector
                                                          name:notificationName
                                                        object:notificationSender
                                            suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];
}

+ (void) removeObserver:(id)notificationObserver
{
  [[NSDistributedNotificationCenter defaultCenter] removeObserver:notificationObserver];
}

+ (void) removeObserver:(id)notificationObserver name:(NSString*)notificationName
{
  [KeyRemap4MacBookNSDistributedNotificationCenter removeObserver:notificationObserver
                                                             name:notificationName
                                                           object:kKeyRemap4MacBookNotificationKey];
}

+ (void) removeObserver:(id)notificationObserver name:(NSString*)notificationName object:(NSString*)notificationSender
{
  [[NSDistributedNotificationCenter defaultCenter] removeObserver:notificationObserver
                                                             name:notificationName
                                                           object:notificationSender];
}

+ (void) postNotificationName:(NSString*)notificationName userInfo:(NSDictionary*)userInfo
{
  // In Mac OS X 10.7, NSDistributedNotificationCenter is suspended after calling [NSAlert runModal].
  // So, we need to call postNotificationName with deliverImmediately:YES.

  [[NSDistributedNotificationCenter defaultCenter] postNotificationName:notificationName
                                                                 object:kKeyRemap4MacBookNotificationKey
                                                               userInfo:userInfo
                                                     deliverImmediately:YES];
}

@end
