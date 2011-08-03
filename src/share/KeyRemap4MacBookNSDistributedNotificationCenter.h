// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@interface org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter : NSObject

+ (void) addObserver:(id)notificationObserver selector:(SEL)notificationSelector name:(NSString*)notificationName;
+ (void) removeObserver:(id)notificationObserver name:(NSString*)notificationName;
+ (void) postNotificationName:(NSString*)notificationName userInfo:(NSDictionary*)userInfo;

@end
