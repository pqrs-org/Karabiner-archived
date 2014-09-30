// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>

@interface MigrationUtilities : NSObject

+ (NSString *)applicationSupportName;

+ (BOOL)migrate:(NSArray *)oldBundleIdentifiers
    oldApplicationSupports:(NSArray *)oldApplicationSupports
                  oldPaths:(NSArray *)oldPaths;

@end
