#import "FrontmostWindow.h"

@interface FrontmostWindow ()

@property(readwrite) CGRect windowBounds;
@property(copy, readwrite) NSString* windowName;
@property(copy, readwrite) NSString* bundleIdentifier;

@end

@implementation FrontmostWindow

- (instancetype)init {
  self = [super init];

  if (self) {
    self.windowBounds = CGRectNull;

    NSRunningApplication* frontmostApplication = [[NSWorkspace sharedWorkspace] frontmostApplication];
    pid_t frontmostApplicationPid = [frontmostApplication processIdentifier];

    NSArray* windows = (__bridge_transfer NSArray*)(CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly |
                                                                                   kCGWindowListExcludeDesktopElements,
                                                                               kCGNullWindowID));
    for (NSDictionary* window in windows) {
      // Target windows:
      //   * frontmostApplication
      //   * loginwindow (shutdown dialog)
      //   * Launchpad
      //
      // Limitations:
      //   * There is not reliable way to judge whether Dashboard is shown.
      //
      pid_t windowOwnerPID = [window[(__bridge NSString*)(kCGWindowOwnerPID)] intValue];
      NSString* windowName = window[(__bridge NSString*)(kCGWindowName)];
      NSInteger windowLayer = [window[(__bridge NSString*)(kCGWindowLayer)] integerValue];
      NSString* bundleIdentifier = [[NSRunningApplication runningApplicationWithProcessIdentifier:windowOwnerPID] bundleIdentifier];

      if ((windowOwnerPID == frontmostApplicationPid) ||
          ([bundleIdentifier isEqualToString:@"com.apple.loginwindow"]) ||
          ([bundleIdentifier isEqualToString:@"com.apple.dock"] &&
           [windowName isEqualToString:@"Launchpad"]) ||
          ([bundleIdentifier isEqualToString:@"com.apple.Spotlight"] &&
           windowLayer < 25)) {
        CGFloat windowAlpha = [window[(__bridge NSString*)(kCGWindowAlpha)] floatValue];
        CGRect windowBounds;
        CGRectMakeWithDictionaryRepresentation((__bridge CFDictionaryRef)(window[(__bridge NSString*)(kCGWindowBounds)]), &windowBounds);

        // ----------------------------------------
        // Ignore windows.
        //
        // There are well known windows that we need ignore:
        //   * Google Chrome has some transparent windows.
        //   * Google Chrome's status bar which is shown when mouse cursor is on links.
        //   * Karabiner's status message windows.
        //
        // Do not forget to treat this situations:
        //   * Do not ignore menubar.
        //   * Do not ignore popup menu.
        //   * Do not ignore alert window on web browsers.
        //   * Do not ignore iTunes's preferences window which has some special behavior.

        // Ignore transparent windows.
        CGFloat transparentThreshold = 0.001;
        if (windowAlpha < transparentThreshold) {
          continue;
        }
        // Ignore small windows. (For example, a status bar of Google Chrome.)
        CGFloat windowSizeThreshold = 40;
        if (windowBounds.size.width < windowSizeThreshold ||
            windowBounds.size.height < windowSizeThreshold) {
          continue;
        }
        // Ignore some app windows.
        if ([bundleIdentifier isEqualToString:@"org.pqrs.Karabiner"]) {
          // There is no reliable public specifications for kCGWindowLayer.
          // So, we use magic numbers that are confirmed by "dump_windows" option.

          // Status message windows.
          if (windowLayer == 25) {
            continue;
          }
        }

        // ----------------------------------------
        self.windowBounds = windowBounds;
        self.windowName = windowName;
        self.bundleIdentifier = bundleIdentifier;
        break;
      }
    }

    if (CGRectIsNull(self.windowBounds)) {
      if ([[frontmostApplication bundleIdentifier] isEqualToString:@"com.apple.finder"]) {
        // Desktop
        self.windowBounds = [[NSScreen mainScreen] frame];
        self.windowName = nil;
        self.bundleIdentifier = [frontmostApplication bundleIdentifier];
      }
    }
  }

  return self;
}

@end
