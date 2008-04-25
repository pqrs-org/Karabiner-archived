// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "KeyRemap4MacBookPref.h"
#import "SysctlWrapper.h"

@implementation KeyRemap4MacBookPref

// ----------------------------------------------------------------------
- (void) drawVersion
{
  NSString *version = [SysctlWrapper getString:@"keyremap4macbook.version"];
  if (! version) {
    version = @"-.-.-";
  }
  [_versionText setStringValue:version];
}

// ----------------------------------------------------------------------
- (void) mainViewDidLoad
{
  [self drawVersion];
}

@end
