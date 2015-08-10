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
  [self output:@"  post-hid-event [--flag flag] keycode\n"];
  [self output:@"\n"];
  [self output:@"Example:\n"];
  [self output:@"  post-hid-event NX_KEYTYPE_MUTE\n"];
  [self output:@"  post-hid-event NX_KEYTYPE_PLAY\n"];
  [self output:@"  post-hid-event NX_KEYTYPE_BRIGHTNESS_UP\n"];
  [self output:@"  post-hid-event --flag shift --flag option NX_KEYTYPE_SOUND_UP\n"];
  [self output:@"\n"];
  [self output:@"List of keycodes:\n"];
  for (NSArray* a in [self keycodes]) {
    NSString* name = a[0];
    [self output:[NSString stringWithFormat:@"  %@\n", name]];
  }
  [self output:@"\n"];
  [self output:@"List of flags:\n"];
  [self output:@"  command\n"];
  [self output:@"  control\n"];
  [self output:@"  option\n"];
  [self output:@"  shift\n"];

  [[NSApplication sharedApplication] terminate:nil];
}

- (int)main {
  NSArray* arguments = [[NSProcessInfo processInfo] arguments];
  NSString* keycode = nil;
  NSMutableArray* flags = [NSMutableArray new];

  for (NSUInteger i = 1; i < [arguments count]; ++i) {
    if ([arguments[i] isEqualToString:@"--flag"]) {
      ++i;
      if (i < [arguments count]) {
        NSString* f = arguments[i];
        if ([f isEqualToString:@"command"]) {
          [flags addObject:@(NX_COMMANDMASK)];
        } else if ([f isEqualToString:@"control"]) {
          [flags addObject:@(NX_CONTROLMASK)];
        } else if ([f isEqualToString:@"option"]) {
          [flags addObject:@(NX_ALTERNATEMASK)];
        } else if ([f isEqualToString:@"shift"]) {
          [flags addObject:@(NX_SHIFTMASK)];
        } else {
          [self output:[NSString stringWithFormat:@"Unknown flag: %@\n", f]];
          return 1;
        }
      }
    } else {
      keycode = arguments[i];
    }
  }

  if (!keycode) {
    [self usage];
  } else {
    for (NSArray* a in [self keycodes]) {
      NSString* name = a[0];
      if ([name isEqualToString:keycode]) {
        IOHIDPostEventWrapper* wrapper = [IOHIDPostEventWrapper new];

        for (NSNumber* flag in flags) {
          [wrapper postModifierKey:[flag intValue] keydown:YES];
        }

        [wrapper postKey:[a[1] intValue]];

        for (NSNumber* flag in flags) {
          [wrapper postModifierKey:[flag intValue] keydown:NO];
        }

        return 0;
      }
    }

    [self output:[NSString stringWithFormat:@"Unknown keycode: %@\n", keycode]];
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
