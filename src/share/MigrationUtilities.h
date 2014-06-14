// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>

@interface MigrationUtilities : NSObject

+ (NSString*) applicationSupportName;

+ (void) migrate:(NSArray*)oldBundleIdentifiers
  oldApplicationSupports:(NSArray*)oldApplicationSupports
  oldPaths:(NSArray*)oldPaths;

@end
