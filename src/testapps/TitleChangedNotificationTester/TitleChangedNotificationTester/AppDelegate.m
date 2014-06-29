#import "AppDelegate.h"

@implementation AppDelegate

- (void) timerFireMethod:(NSTimer*)timer
{
  dispatch_async(dispatch_get_main_queue(), ^{
    @synchronized(self) {
      [self.window2 setTitle:[NSString stringWithFormat:@"window2 %@", [NSDate date]]];
    }
  });
}

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification
{
  NSRect rect = [[NSScreen mainScreen] frame];

  [self.window1 setTitle:@"window1"];
  [self.window1 setLevel:NSFloatingWindowLevel];
  [self.window1 setFrameTopLeftPoint:NSMakePoint(100, rect.size.height - 100)];

  [self.window2 setTitle:@"window2"];
  [self.window2 setLevel:NSFloatingWindowLevel];
  [self.window2 setFrameTopLeftPoint:NSMakePoint(200, rect.size.height - 400)];

  [self.window3 setTitle:@"window3"];
  [self.window3 setLevel:NSFloatingWindowLevel];
  [self.window3 setFrameTopLeftPoint:NSMakePoint(300, rect.size.height - 700)];

  [NSTimer scheduledTimerWithTimeInterval:0.5
                                   target:self
                                 selector:@selector(timerFireMethod:)
                                 userInfo:nil
                                  repeats:YES];
}

- (IBAction) refreshWindow1Title:(id)sender
{
  [self.window1 setTitle:[NSString stringWithFormat:@"window1 %@", [NSDate date]]];
}

@end
