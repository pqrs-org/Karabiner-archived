@import Foundation;

@interface Main : NSObject

- (int)main;

@end

@implementation Main

- (void)output:(NSString*)string {
  NSFileHandle* fh = [NSFileHandle fileHandleWithStandardOutput];
  [fh writeData:[string dataUsingEncoding:NSUTF8StringEncoding]];
}

- (int)main {
  NSArray* arguments = [[NSProcessInfo processInfo] arguments];

  if ([arguments count] == 1) {
    [self output:@"Usage:\n"];
    [self output:@"  read-symbolichotkeys number\n"];
    [self output:@"\n"];
    [self output:@"Example:\n"];
    [self output:@"  read-symbolichotkeys 60\n"];
    [self output:@"  read-symbolichotkeys 61\n"];
    return 1;
  }

  NSDictionary* dictionary = [[NSUserDefaults standardUserDefaults] persistentDomainForName:@"com.apple.symbolichotkeys"];
  if (!dictionary) {
    [self output:@"Error: com.apple.symbolichotkeys is not found.\n"];
    return 2;
  }

  NSDictionary* symbolichotkeys = dictionary[@"AppleSymbolicHotKeys"];
  if (!symbolichotkeys) {
    [self output:@"Error: AppleSymbolicHotKeys is not found.\n"];
    return 2;
  }

  NSDictionary* symbolichotkey = symbolichotkeys[arguments[1]];
  if (!symbolichotkey) {
    // No entry
    return 0;
  }

  NSDictionary* value = symbolichotkey[@"value"];
  if (!value) {
    [self output:@"Error: value is not found.\n"];
    return 2;
  }

  NSArray* parameters = value[@"parameters"];
  if (!parameters) {
    [self output:@"Error: parameters is not found.\n"];
    return 2;
  }

  NSNumber* key = parameters[1];
  NSInteger modifiers = [parameters[2] integerValue];
  NSMutableArray* modifierFlags = [NSMutableArray new];

  if (modifiers & 0x20000) {
    [modifierFlags addObject:@"ModifierFlag::SHIFT_L"];
  }
  if (modifiers & 0x40000) {
    [modifierFlags addObject:@"ModifierFlag::CONTROL_L"];
  }
  if (modifiers & 0x80000) {
    [modifierFlags addObject:@"ModifierFlag::OPTION_L"];
  }
  if (modifiers & 0x100000) {
    [modifierFlags addObject:@"ModifierFlag::COMMAND_L"];
  }
  if ([modifierFlags count] == 0) {
    [modifierFlags addObject:@"ModifierFlag::ZERO"];
  }

  [self output:[NSString stringWithFormat:@"KeyCode::RawValue::0x%lx, %@\n", [key integerValue], [modifierFlags componentsJoinedByString:@" | "]]];

  return 0;
}

@end

int main(int argc, const char* argv[]) {
  @autoreleasepool {
    return [[Main new] main];
  }
}
