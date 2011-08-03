#import "KeyRemap4MacBookNSDistributedNotificationCenter.h"
#import "KeyRemap4MacBookKeys.h"

@implementation org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter

+ (void) addObserver:(id)notificationObserver selector:(SEL)notificationSelector name:(NSString*)notificationName
{
  // In Mac OS X 10.7, NSDistributedNotificationCenter is suspended after calling [NSAlert runModal].
  // So, we need to set suspendedDeliveryBehavior to NSNotificationSuspensionBehaviorDeliverImmediately.

  [[NSDistributedNotificationCenter defaultCenter] addObserver:notificationObserver
                                                      selector:notificationSelector
                                                          name:notificationName
                                                        object:kKeyRemap4MacBookNotificationKey
                                            suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];
}

+ (void) removeObserver:(id)notificationObserver name:(NSString*)notificationName
{
  [[NSDistributedNotificationCenter defaultCenter] removeObserver:notificationObserver
                                                             name:notificationName
                                                           object:kKeyRemap4MacBookNotificationKey];
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
