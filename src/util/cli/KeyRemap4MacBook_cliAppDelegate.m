#import "KeyRemap4MacBook_cliAppDelegate.h"

@implementation KeyRemap4MacBook_cliAppDelegate

@synthesize window;

- (void) output:(NSString*)string
{
  NSFileHandle* fh = [NSFileHandle fileHandleWithStandardOutput];
  [fh writeData:[string dataUsingEncoding:NSUTF8StringEncoding]];
}

- (void) usage
{
  [self output:@"Usage:\n"];
  [self output:@"  KeyRemap4MacBook_cli list\n"];
  [self output:@"  KeyRemap4MacBook_cli selected\n"];
  [self output:@"  KeyRemap4MacBook_cli changed\n"];
  [self output:@"  KeyRemap4MacBook_cli reloadxml\n"];
  [self output:@"  KeyRemap4MacBook_cli export\n"];
  [self output:@"  KeyRemap4MacBook_cli select INDEX\n"];
  [self output:@"  KeyRemap4MacBook_cli set IDENTIFIER VALUE\n"];
  [self output:@"  KeyRemap4MacBook_cli enable IDENTIFIER (alias of set IDENTIFIER 1)\n"];
  [self output:@"  KeyRemap4MacBook_cli disable IDENTIFIER (alias of set IDENTIFIER 0)\n"];
  [self output:@"\n"];
  [self output:@"Example:\n"];
  [self output:@"  KeyRemap4MacBook_cli list\n"];
  [self output:@"  KeyRemap4MacBook_cli selected\n"];
  [self output:@"  KeyRemap4MacBook_cli changed\n"];
  [self output:@"  KeyRemap4MacBook_cli reloadxml\n"];
  [self output:@"  KeyRemap4MacBook_cli export\n"];
  [self output:@"  KeyRemap4MacBook_cli select 1\n"];
  [self output:@"  KeyRemap4MacBook_cli set repeat.wait 30\n"];
  [self output:@"  KeyRemap4MacBook_cli enable remap.shiftL2commandL\n"];
  [self output:@"  KeyRemap4MacBook_cli disable remap.shiftL2commandL\n"];

  [[NSApplication sharedApplication] terminate:nil];
}

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification
{
  NSArray* arguments = [[NSProcessInfo processInfo] arguments];

  if ([arguments count] == 1) {
    [self usage];

  } else {
    NSString* command = [arguments objectAtIndex:1];

    /*  */ if ([command isEqualToString:@"list"]) {
      NSArray* a = [[client_ proxy] configlist_getConfigList];
      for (NSDictionary* dict in a) {
        [self output:[dict objectForKey:@"name"]];
        [self output:@"\n"];
      }

    } else if ([command isEqualToString:@"selected"]) {
      [self output:[NSString stringWithFormat:@"%d\n", (int)([[client_ proxy] configlist_selectedIndex])]];

    } else if ([command isEqualToString:@"changed"]) {
      NSDictionary* dict = [[client_ proxy] changed];
      if (dict) {
        for (NSString* key in [dict allKeys]) {
          [self output:[NSString stringWithFormat:@"%@=%@\n", key, [dict objectForKey:key]]];
        }
      }

    } else if ([command isEqualToString:@"reloadxml"]) {
      [[client_ proxy] configxml_reload];

    } else if ([command isEqualToString:@"export"]) {
      NSDictionary* dict = [[client_ proxy] changed];
      if (dict) {
        [self output:@"#!/bin/sh\n\n"];
        [self output:[NSString stringWithFormat:@"cli=%@\n\n", [arguments objectAtIndex:0]]];

        for (NSString* key in [dict allKeys]) {
          if (! [key hasPrefix:@"notsave."]) {
            [self output:[NSString stringWithFormat:@"$cli set %@ %@\n", key, [dict objectForKey:key]]];
            [self output:@"/bin/echo -n .\n"];
          }
        }
        [self output:@"/bin/echo\n"];
      }

    } else if ([command isEqualToString:@"select"]) {
      if ([arguments count] != 3) {
        [self usage];
      }
      NSString* value = [arguments objectAtIndex:2];
      [[client_ proxy] configlist_select:[value intValue]];

    } else if ([command isEqualToString:@"set"]) {
      if ([arguments count] != 4) {
        [self usage];
      }
      NSString* identifier = [arguments objectAtIndex:2];
      NSString* value = [arguments objectAtIndex:3];
      [[client_ proxy] setValueForName:[value intValue] forName:identifier];

    } else if ([command isEqualToString:@"enable"]) {
      if ([arguments count] != 3) {
        [self usage];
      }
      NSString* value = [arguments objectAtIndex:2];
      [[client_ proxy] setValueForName:1 forName:value];

    } else if ([command isEqualToString:@"disable"]) {
      if ([arguments count] != 3) {
        [self usage];
      }
      NSString* value = [arguments objectAtIndex:2];
      [[client_ proxy] setValueForName:0 forName:value];
    }
  }

  [[NSApplication sharedApplication] terminate:nil];
}

@end
