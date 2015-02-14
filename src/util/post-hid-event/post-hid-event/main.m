#import <Foundation/Foundation.h>
#import "IOHIDPostEventWrapper.h"

@interface PostHIDEvent : NSObject

- (int)main;

@end

@implementation PostHIDEvent

- (void)output:(NSString*)string {
  NSFileHandle* fh = [NSFileHandle fileHandleWithStandardOutput];
  [fh writeData:[string dataUsingEncoding:NSUTF8StringEncoding]];
}

- (NSArray*)keycodes {

#define KEYCODE_DEFINITION(KEYCODE) \
  @[ @ #KEYCODE, @(KEYCODE) ]

  return @[
    KEYCODE_DEFINITION(NX_KEYTYPE_SOUND_UP),
    KEYCODE_DEFINITION(NX_KEYTYPE_SOUND_DOWN),
    KEYCODE_DEFINITION(NX_KEYTYPE_BRIGHTNESS_UP),
    KEYCODE_DEFINITION(NX_KEYTYPE_BRIGHTNESS_DOWN),
    KEYCODE_DEFINITION(NX_KEYTYPE_MUTE),
    KEYCODE_DEFINITION(NX_KEYTYPE_PLAY),
    KEYCODE_DEFINITION(NX_KEYTYPE_NEXT),
    KEYCODE_DEFINITION(NX_KEYTYPE_PREVIOUS),
    KEYCODE_DEFINITION(NX_KEYTYPE_ILLUMINATION_UP),
    KEYCODE_DEFINITION(NX_KEYTYPE_ILLUMINATION_DOWN),
    KEYCODE_DEFINITION(NX_KEYTYPE_ILLUMINATION_TOGGLE),
    KEYCODE_DEFINITION(NX_POWER_KEY),
  ];
}

- (void)usage {
  [self output:@"Usage:\n"];
  [self output:@"  post-hid-event keycode\n"];
  [self output:@"\n"];
  [self output:@"Example:\n"];
  [self output:@"  post-hid-event NX_KEYTYPE_MUTE\n"];
  [self output:@"  post-hid-event NX_KEYTYPE_PLAY\n"];
  [self output:@"  post-hid-event NX_KEYTYPE_BRIGHTNESS_UP\n"];
  [self output:@"\n"];
  [self output:@"List of keycodes:\n"];
  for (NSArray* a in [self keycodes]) {
    NSString* name = a[0];
    [self output:[NSString stringWithFormat:@"  %@\n", name]];
  }

  [[NSApplication sharedApplication] terminate:nil];
}

- (int)main {
  NSArray* arguments = [[NSProcessInfo processInfo] arguments];

  if ([arguments count] == 1) {
    [self usage];
  } else {
    NSString* keycode = arguments[1];

    for (NSArray* a in [self keycodes]) {
      NSString* name = a[0];
      if ([name isEqualToString:keycode]) {
        [[IOHIDPostEventWrapper new] postKey:[a[1] intValue]];
        return 0;
      }
    }

    [self output:[NSString stringWithFormat:@"Unknown keycode:%@\n", keycode]];
    return 1;
  }

  return 0;
}

@end

int main(int argc, const char* argv[]) {
  @autoreleasepool {
    return [[PostHIDEvent new] main];
  }
}
