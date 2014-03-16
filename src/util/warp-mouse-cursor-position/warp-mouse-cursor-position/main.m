@import Cocoa;

@interface WarpMouseCursorPosition : NSObject

- (int) main;

@end

@implementation WarpMouseCursorPosition

- (void) output:(NSString*)string
{
  NSFileHandle* fh = [NSFileHandle fileHandleWithStandardOutput];
  [fh writeData:[string dataUsingEncoding:NSUTF8StringEncoding]];
}

- (void) usage
{
  [self output:@"Usage:\n"];
  [self output:@"  warp-mouse-cursor-position screen NUM VERTICAL X_OFFSET HORIZONTAL Y_OFFSET\n"];
  [self output:@"  warp-mouse-cursor-position front_window VERTICAL X_OFFSET HORIZONTAL Y_OFFSET\n"];
  [self output:@"  warp-mouse-cursor-position dump_windows (all|onscreen)\n"];
  [self output:@"\n"];
  [self output:@"Example:\n"];
  [self output:@"  warp-mouse-cursor-position screen 0 top +10 left +20\n"];
  [self output:@"  warp-mouse-cursor-position screen 0 middle +0 center +0\n"];
  [self output:@"  warp-mouse-cursor-position screen 0 bottom -10 right -20\n"];
  [self output:@"  warp-mouse-cursor-position front_window top +10 left +20\n"];
  [self output:@"  warp-mouse-cursor-position front_window middle +0 center +0\n"];
  [self output:@"  warp-mouse-cursor-position front_window bottom -10 right -20\n"];
  [self output:@"  warp-mouse-cursor-position dump_windows all\n"];
  [self output:@"  warp-mouse-cursor-position dump_windows onscreen\n"];
  [self output:@"\n"];
  [self output:@"VERTICAL:\n"];
  [self output:@"  top middle bottom\n"];
  [self output:@"\n"];
  [self output:@"HORIZONTAL:\n"];
  [self output:@"  left center right\n"];

  [[NSApplication sharedApplication] terminate:nil];
}

- (CGPoint) position:(CGRect)frame vertical:(NSString*)vertical x:(CGFloat)x horizontal:(NSString*)horizontal y:(CGFloat)y
{
  CGPoint position = CGPointZero;

  if ([vertical isEqualToString:@"middle"]) {
    position.y = frame.origin.y + (frame.size.height / 2);
  } else if ([vertical isEqualToString:@"bottom"]) {
    position.y = frame.origin.y + frame.size.height;
  } else {
    position.y = frame.origin.y;
  }
  position.y += y;

  if ([horizontal isEqualToString:@"center"]) {
    position.x = frame.origin.x + (frame.size.width / 2);
  } else if ([horizontal isEqualToString:@"right"]) {
    position.x = frame.origin.x + frame.size.width;
  } else {
    position.x = frame.origin.x;
  }
  position.x += x;

  return position;
}

- (NSString*) bundleIdentifierFromPid:(pid_t)pid
{
  NSRunningApplication* runningApplication = [NSRunningApplication runningApplicationWithProcessIdentifier:pid];
  return [runningApplication bundleIdentifier];
}

- (int) main
{
  NSArray* arguments = [[NSProcessInfo processInfo] arguments];

  if ([arguments count] == 1) {
    [self usage];
  } else {
    @try {
      NSString* command = arguments[1];
      if ([command isEqualToString:@"screen"]) {
        if ([arguments count] != 7) {
          [self usage];
        } else {
          NSUInteger number    = [arguments[2] integerValue];
          NSString* vertical   = arguments[3];
          CGFloat x            = [arguments[4] floatValue];
          NSString* horizontal = arguments[5];
          CGFloat y            = [arguments[6] floatValue];

          NSArray* screens = [NSScreen screens];
          if (number <= [screens count] - 1) {
            NSScreen* screen = screens[number];
            CGPoint position = [self position:screen.frame
                                     vertical:vertical
                                            x:x
                                   horizontal:horizontal
                                            y:y];
            CGWarpMouseCursorPosition(position);

          } else {
            [self output:[NSString stringWithFormat:@"screen number should be 0-%ld\n", [screens count] - 1]];
            return 1;
          }
        }

      } else if ([command isEqualToString:@"front_window"]) {
        if ([arguments count] != 6) {
          [self usage];
        } else {
          NSString* vertical   = arguments[2];
          CGFloat x            = [arguments[3] floatValue];
          NSString* horizontal = arguments[4];
          CGFloat y            = [arguments[5] floatValue];

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
            NSString* bundleIdentifier = [self bundleIdentifierFromPid:windowOwnerPID];
            NSString* windowName = window[(__bridge NSString*)(kCGWindowName)];

            if ((windowOwnerPID == frontmostApplicationPid) ||
                ([bundleIdentifier isEqualToString:@"com.apple.loginwindow"]) ||
                ([bundleIdentifier isEqualToString:@"com.apple.dock"] &&
                 [windowName isEqualToString:@"Launchpad"])) {
              CGFloat windowAlpha   = [window[(__bridge NSString*)(kCGWindowAlpha)] floatValue];
              NSInteger windowLayer = [window[(__bridge NSString*)(kCGWindowLayer)] integerValue];
              CGRect windowBounds;
              CGRectMakeWithDictionaryRepresentation((__bridge CFDictionaryRef)(window[(__bridge NSString*)(kCGWindowBounds)]), &windowBounds);

              // ----------------------------------------
              // Ignore windows.
              //
              // There are well known windows that we need ignore:
              //   * Google Chrome has some transparent windows.
              //   * Google Chrome's status bar which is shown when mouse cursor is on links.
              //   * KeyRemap4MacBook's status message windows.
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
              if ([bundleIdentifier isEqualToString:@"org.pqrs.KeyRemap4MacBook"]) {
                // There is no reliable public specifications for kCGWindowLayer.
                // So, we use magic numbers that are confirmed by "dump_windows" option.

                // Status message windows.
                if (windowLayer == 25) {
                  continue;
                }
              }

              // ----------------------------------------
              CGPoint position = [self position:windowBounds
                                       vertical:vertical
                                              x:x
                                     horizontal:horizontal
                                              y:y];
              CGWarpMouseCursorPosition(position);
              break;
            }
          }
        }

      } else if ([command isEqualToString:@"dump_windows"]) {
        if ([arguments count] != 3) {
          [self usage];
        } else {
          NSString* filter = arguments[2];

          CGWindowListOption option = kCGWindowListExcludeDesktopElements;
          if ([filter isEqualToString:@"onscreen"]) {
            option |= kCGWindowListOptionOnScreenOnly;
          }
          NSArray* windows = (__bridge_transfer NSArray*)(CGWindowListCopyWindowInfo(option, kCGNullWindowID));
          for (NSDictionary* window in windows) {
            pid_t windowOwnerPID = [window[(__bridge NSString*)(kCGWindowOwnerPID)] intValue];
            [self output:[self bundleIdentifierFromPid:windowOwnerPID]];
            [self output:@"\n"];
            [self output:[NSString stringWithFormat:@"%@", window]];
            [self output:@"\n"];
            [self output:@"\n"];
          }
        }
      }

    } @catch (NSException* exception) {
      NSLog(@"%@", exception);
      return 1;
    }
  }
}

@end

int
main(int argc, const char* argv[])
{
  return [[WarpMouseCursorPosition new] main];
}
