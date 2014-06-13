// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>

@interface MigrationUtilities : NSObject

+ (void) migrate:(NSArray*)bundleIdentifiers appURLs:(NSArray*)appURLs;

@end
