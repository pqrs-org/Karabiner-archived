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
  [self output:@"  warp-mouse-cursor-position screen NUM TOP LEFT\n"];
  [self output:@"  warp-mouse-cursor-position front_window TOP LEFT\n"];
  [self output:@"\n"];
  [self output:@"Example:\n"];
  [self output:@"  warp-mouse-cursor-position screen 0 300 400\n"];
  [self output:@"  warp-mouse-cursor-position front_window 100 10\n"];

  [[NSApplication sharedApplication] terminate:nil];
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
        if ([arguments count] != 5) {
          [self usage];
        } else {
          NSUInteger number = [arguments[2] integerValue];
          CGFloat top       = [arguments[3] floatValue];
          CGFloat left      = [arguments[4] floatValue];

          NSArray* screens = [NSScreen screens];
          if (number <= [screens count] - 1) {
            NSScreen* screen = screens[number];
            CGWarpMouseCursorPosition(CGPointMake(screen.frame.origin.x + top,
                                                  screen.frame.origin.y + left));
          } else {
            [self output:[NSString stringWithFormat:@"screen number should be 0-%ld\n", [screens count] - 1]];
            return 1;
          }
        }

      } else if ([command isEqualToString:@"front_window"]) {
        if ([arguments count] != 4) {
          [self usage];
        } else {
          CGFloat top  = [arguments[2] floatValue];
          CGFloat left = [arguments[3] floatValue];

          pid_t pid = [[[NSWorkspace sharedWorkspace] frontmostApplication] processIdentifier];

          NSArray* windows = (__bridge NSArray*)(CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly |
                                                                            kCGWindowListExcludeDesktopElements,
                                                                            kCGNullWindowID));
          for (NSDictionary* window in windows) {
            if ([window[(__bridge NSString*)(kCGWindowOwnerPID)] integerValue] == pid) {
              // Ignore transparent windows.
              if ([window[(__bridge NSString*)(kCGWindowAlpha)] floatValue] < 0.001) {
                continue;
              }
              // Ignore system layer windows.
              // (See /System/Library/Frameworks/CoreGraphics.framework/Versions/A/Headers/CGWindowLevel.h )
              if ([window[(__bridge NSString*)(kCGWindowLayer)] integerValue] >= kCGDockWindowLevelKey) {
                continue;
              }

              NSDictionary* rect = window[(__bridge NSString*)(kCGWindowBounds)];
              CGWarpMouseCursorPosition(CGPointMake([rect[@"X"] floatValue] + top,
                                                    [rect[@"Y"] floatValue] + left));
              break;
            }
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
