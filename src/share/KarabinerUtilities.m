#import "KarabinerUtilities.h"

@implementation KarabinerUtilities

+ (NSString*)bundleIdentifier:(NSRunningApplication*)runningApplication {
  NSString* bundleIdentifier = [runningApplication bundleIdentifier];
  if (bundleIdentifier) {
    return bundleIdentifier;

  } else {
    // bundleIdentifier will be nil if the application does not have an Info.plist.
    //
    // We use localizedName instead of bundleIdentifier,
    //
    NSString* localizedName = [runningApplication localizedName];
    return [NSString stringWithFormat:@"org.pqrs.unknownapp.%@", localizedName];
  }
}

@end
