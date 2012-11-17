// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "WorkSpaceData.h"

@implementation WorkSpaceData

+ (NSString*) getActiveApplicationName
{
  // ----------------------------------------
  NSWorkspace* ws = [NSWorkspace sharedWorkspace];
  if (! ws) return nil;

  NSArray* a = [ws runningApplications];
  if (! a) return nil;

  for (NSRunningApplication* app in a) {
    if (! app) return nil;

    if ([app isActive]) {
      NSString* nsappname = [app bundleIdentifier];

      if (nsappname) {
        return [NSString stringWithString:nsappname];

      } else {
        // We use localizedName instead of bundleIdentifier,
        // because "MacSOUP" doesn't have bundleIdentifier.
        // http://www.haller-berlin.de/macsoup/index.html
        NSString* localizedName = [app localizedName];
        if (localizedName) {
          return [NSString stringWithFormat:@"org.pqrs.unknownapp.%@", localizedName];
        }
      }
    }
  }

  return nil;
}

- (unsigned int) getApplicationType:(NSString*)name
{
  if (! name) return 0;

  return [xmlCompiler_ appid:name];
}

@end
