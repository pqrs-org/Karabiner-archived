@import Cocoa;
#import "KarabinerClient.h"

@interface KarabinerCLI : NSObject

- (void) main;

@end

@implementation KarabinerCLI

- (void) output:(NSString*)string
{
  NSFileHandle* fh = [NSFileHandle fileHandleWithStandardOutput];
  [fh writeData:[string dataUsingEncoding:NSUTF8StringEncoding]];
}

- (void) usage
{
  [self output:@"Usage:\n"];
  [self output:@"  karabiner list\n"];
  [self output:@"  karabiner selected\n"];
  [self output:@"  karabiner changed\n"];
  [self output:@"  karabiner reloadxml\n"];
  [self output:@"  karabiner export\n"];
  [self output:@"  karabiner relaunch\n"];
  [self output:@"  karabiner select INDEX\n"];
  [self output:@"  karabiner set IDENTIFIER VALUE\n"];
  [self output:@"  karabiner enable IDENTIFIER (alias of set IDENTIFIER 1)\n"];
  [self output:@"  karabiner disable IDENTIFIER (alias of set IDENTIFIER 0)\n"];
  [self output:@"\n"];
  [self output:@"Example:\n"];
  [self output:@"  karabiner list\n"];
  [self output:@"  karabiner selected\n"];
  [self output:@"  karabiner changed\n"];
  [self output:@"  karabiner reloadxml\n"];
  [self output:@"  karabiner export\n"];
  [self output:@"  karabiner relaunch\n"];
  [self output:@"  karabiner select 1\n"];
  [self output:@"  karabiner set repeat.wait 30\n"];
  [self output:@"  karabiner enable remap.shiftL2commandL\n"];
  [self output:@"  karabiner disable remap.shiftL2commandL\n"];

  [[NSApplication sharedApplication] terminate:nil];
}

- (void) main
{
  NSArray* arguments = [[NSProcessInfo processInfo] arguments];

  if ([arguments count] == 1) {
    [self usage];

  } else {
    @try {
      KarabinerClient* client = [KarabinerClient new];
      NSString* command = arguments[1];

      /*  */ if ([command isEqualToString:@"list"]) {
        NSArray* a = [[client proxy] configlist_getConfigList];
        for (NSDictionary* dict in a) {
          [self output:dict[@"name"]];
          [self output:@"\n"];
        }

      } else if ([command isEqualToString:@"selected"]) {
        [self output:[NSString stringWithFormat:@"%d\n", (int)([[client proxy] configlist_selectedIndex])]];

      } else if ([command isEqualToString:@"changed"]) {
        NSDictionary* dict = [[client proxy] changed];
        if (dict) {
          for (NSString* key in [dict allKeys]) {
            [self output:[NSString stringWithFormat:@"%@=%@\n", key, dict[key]]];
          }
        }

      } else if ([command isEqualToString:@"reloadxml"]) {
        [[client proxy] configxml_reload];

      } else if ([command isEqualToString:@"export"]) {
        NSDictionary* dict = [[client proxy] changed];
        if (dict) {
          [self output:@"#!/bin/sh\n\n"];
          [self output:[NSString stringWithFormat:@"cli=%@\n\n", arguments[0]]];

          for (NSString* key in [dict allKeys]) {
            if (! [key hasPrefix:@"notsave."]) {
              [self output:[NSString stringWithFormat:@"$cli set %@ %@\n", key, dict[key]]];
              [self output:@"/bin/echo -n .\n"];
            }
          }
          [self output:@"/bin/echo\n"];
        }

      } else if ([command isEqualToString:@"relaunch"]) {
        [[client proxy] relaunch];

      } else if ([command isEqualToString:@"select"]) {
        if ([arguments count] != 3) {
          [self usage];
        }
        NSString* value = arguments[2];
        [[client proxy] configlist_select:[value intValue]];

      } else if ([command isEqualToString:@"set"]) {
        if ([arguments count] != 4) {
          [self usage];
        }
        NSString* identifier = arguments[2];
        NSString* value = arguments[3];
        [[client proxy] setValue:[value intValue] forName:identifier];

      } else if ([command isEqualToString:@"enable"]) {
        if ([arguments count] != 3) {
          [self usage];
        }
        NSString* value = arguments[2];
        [[client proxy] setValue:1 forName:value];

      } else if ([command isEqualToString:@"disable"]) {
        if ([arguments count] != 3) {
          [self usage];
        }
        NSString* value = arguments[2];
        [[client proxy] setValue:0 forName:value];
      }
    } @catch (NSException* exception) {
      NSLog(@"%@", exception);
    }
  }

  [[NSApplication sharedApplication] terminate:nil];
}

@end

int
main(int argc, const char* argv[])
{
  [[KarabinerCLI new] main];
  return 0;
}
