// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@interface KeyRemap4MacBookNSDistributedNotificationCenter : NSObject

+ (void) addObserver:(id)notificationObserver selector:(SEL)notificationSelector name:(NSString*)notificationName;
+ (void) addObserver:(id)notificationObserver selector:(SEL)notificationSelector name:(NSString*)notificationName object:(NSString*)notificationSender;
+ (void) removeObserver:(id)notificationObserver;
+ (void) removeObserver:(id)notificationObserver name:(NSString*)notificationName;
+ (void) removeObserver:(id)notificationObserver name:(NSString*)notificationName object:(NSString*)notificationSender;
+ (void) postNotificationName:(NSString*)notificationName userInfo:(NSDictionary*)userInfo;

@end
