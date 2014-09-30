#import "AppDelegate.h"

@implementation AppDelegate

- (void)timerFireMethod:(NSTimer *)timer {
  dispatch_async(dispatch_get_main_queue(), ^{
    @synchronized(self) {
      static int counter = 0;
      ++counter;
      if (counter % 2 == 0) {
        [self.window1 makeKeyAndOrderFront:self];
      } else {
        [self.window2 makeKeyAndOrderFront:self];
      }
    }
  });
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
  NSRect rect = [[NSScreen mainScreen] frame];

  [self.window1 setTitle:@"window1"];
  [self.window1 setLevel:NSFloatingWindowLevel];
  [self.window1 setFrameTopLeftPoint:NSMakePoint(100, rect.size.height - 100)];

  [self.window2 setTitle:@"window2"];
  [self.window2 setLevel:NSFloatingWindowLevel];
  [self.window2 setFrameTopLeftPoint:NSMakePoint(200, rect.size.height - 400)];

  [NSTimer scheduledTimerWithTimeInterval:1
                                   target:self
                                 selector:@selector(timerFireMethod:)
                                 userInfo:nil
                                  repeats:YES];
}

@end
