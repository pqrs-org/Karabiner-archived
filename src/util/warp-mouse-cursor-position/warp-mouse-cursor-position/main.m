@import Cocoa;

@interface WarpMouseCursor : NSObject

- (int) main;

@end

@implementation WarpMouseCursor

- (void) output:(NSString*)string
{
  NSFileHandle* fh = [NSFileHandle fileHandleWithStandardOutput];
  [fh writeData:[string dataUsingEncoding:NSUTF8StringEncoding]];
}

- (void) usage
{
  [self output:@"Usage:\n"];
  [self output:@"  warp-mouse-cursor-position screen NUM TOP LEFT\n"];
  [self output:@"\n"];
  [self output:@"Example:\n"];
  [self output:@"  warp-mouse-cursor-position screen 0 300 400\n"];

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
          CGFloat top       = [arguments[3] integerValue];
          CGFloat left      = [arguments[4] integerValue];

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
  return [[WarpMouseCursor new] main];
}
