@import Cocoa;
#import "KarabinerClient.h"

@interface KarabinerCLI : NSObject

- (void)main;

@end

@implementation KarabinerCLI

- (void)output:(NSString*)string {
  NSFileHandle* fh = [NSFileHandle fileHandleWithStandardOutput];
  [fh writeData:[string dataUsingEncoding:NSUTF8StringEncoding]];
}

- (void)usage {
  [self output:@"Usage:\n"];
  [self output:@"  Profile operations:\n"];
  [self output:@"    $ karabiner list\n"];
  [self output:@"    $ karabiner select INDEX (INDEX starts at 0)\n"];
  [self output:@"    $ karabiner select_by_name NAME\n"];
  [self output:@"    $ karabiner selected\n"];
  [self output:@"    $ karabiner append NAME\n"];
  [self output:@"    $ karabiner rename INDEX NEWNAME (INDEX starts at 0)\n"];
  [self output:@"    $ karabiner delete INDEX (INDEX starts at 0)\n"];
  [self output:@"  Settings:\n"];
  [self output:@"    $ karabiner set IDENTIFIER VALUE\n"];
  [self output:@"    $ karabiner enable IDENTIFIER (alias of set IDENTIFIER 1)\n"];
  [self output:@"    $ karabiner disable IDENTIFIER (alias of set IDENTIFIER 0)\n"];
  [self output:@"    $ karabiner toggle IDENTIFIER\n"];
  [self output:@"    $ karabiner changed\n"];
  [self output:@"  Others:\n"];
  [self output:@"    $ karabiner export\n"];
  [self output:@"    $ karabiner reloadxml\n"];
  [self output:@"    $ karabiner relaunch\n"];
  [self output:@"    $ karabiner be_careful_to_use__clear_all_values_by_name PROFILE_NAME\n"];
  [self output:@"\n"];
  [self output:@"Examples:\n"];
  [self output:@"  $ karabiner list\n"];
  [self output:@"  $ karabiner select 1\n"];
  [self output:@"  $ karabiner select_by_name NewItem\n"];
  [self output:@"  $ karabiner selected\n"];
  [self output:@"  $ karabiner append \"For external keyboard\"\n"];
  [self output:@"  $ karabiner rename 1 \"Empty Setting\"\n"];
  [self output:@"  $ karabiner delete 1\n"];
  [self output:@"\n"];
  [self output:@"  $ karabiner set repeat.wait 30\n"];
  [self output:@"  $ karabiner enable remap.shiftL2commandL\n"];
  [self output:@"  $ karabiner disable remap.shiftL2commandL\n"];
  [self output:@"  $ karabiner toggle remap.shiftL2commandL\n"];
  [self output:@"  $ karabiner changed\n"];
  [self output:@"\n"];
  [self output:@"  $ karabiner export\n"];
  [self output:@"  $ karabiner reloadxml\n"];
  [self output:@"  $ karabiner relaunch\n"];
  [self output:@"  $ karabiner be_careful_to_use__clear_all_values_by_name NewItem\n"];

  exit(2);
}

- (void)select:(KarabinerClient*)client command:(NSString*)command index:(NSInteger)index {
  [[client proxy] configlist_select:index];

  if ([[client proxy] configlist_selectedIndex] != index) {
    [self output:[NSString stringWithFormat:@"Failed to %@.\n", command]];
    exit(1);
  }
}

- (void)main {
  NSArray* arguments = [[NSProcessInfo processInfo] arguments];

  if ([arguments count] == 1) {
    [self usage];

  } else {
    @try {
      KarabinerClient* client = [KarabinerClient new];
      NSString* command = arguments[1];

      /*  */ if ([command isEqualToString:@"list"]) {
        NSArray* a = [[client proxy] configlist_getConfigList];
        int index = 0;
        for (NSDictionary* dict in a) {
          [self output:[NSString stringWithFormat:@"%d: %@\n", index, dict[@"name"]]];
          ++index;
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
            if (![key hasPrefix:@"notsave."]) {
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
        [self select:client command:command index:[value integerValue]];

      } else if ([command isEqualToString:@"select_by_name"]) {
        if ([arguments count] != 3) {
          [self usage];
        }
        NSString* value = arguments[2];
        int index = 0;
        for (NSDictionary* config in [[client proxy] configlist_getConfigList]) {
          if ([value isEqualToString:config[@"name"]]) {
            [self select:client command:command index:index];
            return;
          }
          ++index;
        }
        [self output:[NSString stringWithFormat:@"\"%@\" is not found.\n", value]];
        exit(1);

      } else if ([command isEqualToString:@"append"]) {
        if ([arguments count] != 3) {
          [self usage];
        }
        NSString* value = arguments[2];
        [[client proxy] configlist_append];
        NSInteger index = [[[client proxy] configlist_getConfigList] count] - 1;
        [[client proxy] configlist_setName:index name:value];

      } else if ([command isEqualToString:@"rename"]) {
        if ([arguments count] != 4) {
          [self usage];
        }
        NSString* index = arguments[2];
        NSString* value = arguments[3];
        [[client proxy] configlist_setName:[index intValue] name:value];

      } else if ([command isEqualToString:@"delete"]) {
        if ([arguments count] != 3) {
          [self usage];
        }
        NSInteger selected = [[client proxy] configlist_selectedIndex];
        NSString* index = arguments[2];
        if (selected == [index integerValue]) {
          [self output:@"You cannot delete selected profile.\n"];
          exit(1);
        } else {
          [[client proxy] configlist_delete:[index intValue]];
        }

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

      } else if ([command isEqualToString:@"toggle"]) {
        if ([arguments count] != 3) {
          [self usage];
        }
        NSString* value = arguments[2];
        int current = [[client proxy] value:value];
        [[client proxy] setValue:(!current) forName:value];

      } else if ([command isEqualToString:@"be_careful_to_use__clear_all_values_by_name"]) {
        if ([arguments count] != 3) {
          [self usage];
        }
        NSString* value = arguments[2];
        int index = 0;
        for (NSDictionary* config in [[client proxy] configlist_getConfigList]) {
          if ([value isEqualToString:config[@"name"]]) {
            [[client proxy] configlist_clear_all_values:index];
            return;
          }
          ++index;
        }
        [self output:[NSString stringWithFormat:@"\"%@\" is not found.\n", value]];
        exit(1);

      } else {
        [self output:[NSString stringWithFormat:@"Unknown argument: %@\n", command]];
        exit(1);
      }
    }
    @catch (NSException* exception) {
      NSLog(@"%@", exception);
    }
  }
}

@end

int main(int argc, const char* argv[]) {
  [[KarabinerCLI new] main];
  return 0;
}
