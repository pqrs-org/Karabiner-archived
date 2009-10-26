// -*- Mode: objc -*-
//
// KeyResponder.m
//

#import "KeyResponder.h"

@implementation KeyResponder

//
// Key event handlers
//

- (NSMutableString*) modifierFlagsToString:(NSUInteger)flags
{
  NSMutableArray* array = [[[NSMutableArray alloc] init] autorelease];

#define APPEND_FLAG_NAME(mask, name) if (flags & mask) [array addObject:name];
  APPEND_FLAG_NAME(NSAlphaShiftKeyMask, @"AlphaShift");
  APPEND_FLAG_NAME(NSShiftKeyMask, @"Shift");
  APPEND_FLAG_NAME(NSControlKeyMask, @"Control");
  APPEND_FLAG_NAME(NSAlternateKeyMask, @"Alternate");
  APPEND_FLAG_NAME(NSCommandKeyMask, @"Command");
  APPEND_FLAG_NAME(NSNumericPadKeyMask, @"NumericPad");
  APPEND_FLAG_NAME(NSHelpKeyMask, @"Help");
  APPEND_FLAG_NAME(NSFunctionKeyMask, @"Function");
  //APPEND_FLAG_NAME(NSDeviceIndependentModifierFlagsMask, @"DeviceIndependent");
#undef APPEND_FLAG_NAME

  NSMutableString* string = [NSMutableString stringWithString:@""];
  if ([array count] == 0) {
    return string;
  }
  for (unsigned int i = 0; i < [array count]; ++i) {
    [string appendString:[array objectAtIndex:i]];
    if (i < [array count] - 1) {
      [string appendString:@","];
    }
  }
  return string;
}

- (void) output:(NSString*)text
{
  NSTextView* textview = [_result documentView];
  [textview setSelectedRange:NSMakeRange([[[textview textStorage] mutableString] length], 0)];

  [textview setEditable:YES];

  [textview insertText:text];
  [textview insertText:@"\n"];

  [textview setEditable:NO];
  [textview setSelectable:NO];
}

- (void) keyDown:(NSEvent*)event
{
  [self output:[NSString stringWithFormat:@"keyDown\tkey:0x%02x\tflags:%@", [event keyCode], [self modifierFlagsToString:[event modifierFlags]]]];
}

- (void) keyUp:(NSEvent*)event
{
  [self output:[NSString stringWithFormat:@"keyUp\tkey:0x%02x\tflags:%@", [event keyCode], [self modifierFlagsToString:[event modifierFlags]]]];
}

- (void) flagsChanged:(NSEvent*)event
{
  [self output:[NSString stringWithFormat:@"flagsChanged\tkey:0x%02x\tflags:%@", [event keyCode], [self modifierFlagsToString:[event modifierFlags]]]];
}

@end
