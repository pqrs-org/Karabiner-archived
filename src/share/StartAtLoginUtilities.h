// -*- Mode: objc -*-

@import Cocoa;

@interface StartAtLoginUtilities : NSObject

+ (BOOL)isStartAtLogin;
+ (BOOL)isStartAtLogin:(NSURL *)appURL;

+ (void)setStartAtLogin:(BOOL)newvalue;
+ (void)setStartAtLogin:(BOOL)newvalue appURL:(NSURL *)appURL;

@end
