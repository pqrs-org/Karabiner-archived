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

- (NSString*) keycodeToString:(unsigned short)keycode
{
  if (keycode == 0x36) return @"Command_R";
  if (keycode == 0x37) return @"Command_L";
  if (keycode == 0x38) return @"Shift_L";
  if (keycode == 0x3a) return @"Option_L";
  if (keycode == 0x3b) return @"Control_L";
  if (keycode == 0x3c) return @"Shift_R";
  if (keycode == 0x3d) return @"Option_R";
  if (keycode == 0x3e) return @"Control_R";
  if (keycode == 0x3f) return @"FN";

  if (keycode == 0x7a) return @"F1";
  if (keycode == 0x78) return @"F2";
  if (keycode == 0x63) return @"F3";
  if (keycode == 0x76) return @"F4";
  if (keycode == 0x60) return @"F5";
  if (keycode == 0x61) return @"F6";
  if (keycode == 0x62) return @"F7";
  if (keycode == 0x64) return @"F8";
  if (keycode == 0x65) return @"F9";
  if (keycode == 0x6d) return @"F10";
  if (keycode == 0x67) return @"F11";
  if (keycode == 0x6f) return @"F12";

  if (keycode == 0x24) return @"Return";
  if (keycode == 0x31) return @"Space";

  return @"";
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

- (void) outputMouseDownUp:(NSEvent*)event name:(NSString*)name
{
  NSPoint point = [event locationInWindow];
  [self output:[NSString stringWithFormat:@"%@\tpoint:%@\tcount:%d\tbutton:%d\tflags:%@", name, NSStringFromPoint(point), [event clickCount], [event buttonNumber], [self modifierFlagsToString:[event modifierFlags]]]];
}

- (void) outputMouseDragged:(NSEvent*)event name:(NSString*)name
{
  NSPoint point = [event locationInWindow];
  [self output:[NSString stringWithFormat:@"%@\tpoint:%@\tbutton:%d\tflags:%@", name, NSStringFromPoint(point), [event buttonNumber], [self modifierFlagsToString:[event modifierFlags]]]];
}

- (void) keyDown:(NSEvent*)event
{
  [self output:[NSString stringWithFormat:@"keyDown\tkey:0x%02x\tflags:%@\t(%@)", [event keyCode], [self modifierFlagsToString:[event modifierFlags]], [self keycodeToString:[event keyCode]]]];
}

- (void) keyUp:(NSEvent*)event
{
  [self output:[NSString stringWithFormat:@"keyUp\tkey:0x%02x\tflags:%@\t(%@)", [event keyCode], [self modifierFlagsToString:[event modifierFlags]], [self keycodeToString:[event keyCode]]]];
}

- (void) flagsChanged:(NSEvent*)event
{
  [self output:[NSString stringWithFormat:@"flagsChanged\tkey:0x%02x\tflags:%@\t(%@)", [event keyCode], [self modifierFlagsToString:[event modifierFlags]], [self keycodeToString:[event keyCode]]]];
}

- (void) mouseDown:(NSEvent*)event
{
  [self outputMouseDownUp:event name:@"mouseDown"];
}

- (void) mouseUp:(NSEvent*)event
{
  [self outputMouseDownUp:event name:@"mouseUp"];
}

- (void) mouseDragged:(NSEvent*)event
{
  [self outputMouseDragged:event name:@"mouseDragged"];
}

- (void) rightMouseDown:(NSEvent*)event
{
  [self outputMouseDownUp:event name:@"rightMouseDown"];
}

- (void) rightMouseUp:(NSEvent*)event
{
  [self outputMouseDownUp:event name:@"rightMouseUp"];
}

- (void) rightMouseDragged:(NSEvent*)event
{
  [self outputMouseDragged:event name:@"rightMouseDragged"];
}

- (void) otherMouseDown:(NSEvent*)event
{
  [self outputMouseDownUp:event name:@"otherMouseDown"];
}

- (void) otherMouseUp:(NSEvent*)event
{
  [self outputMouseDownUp:event name:@"otherMouseUp"];
}

- (void) otherMouseDragged:(NSEvent*)event
{
  [self outputMouseDragged:event name:@"otherMouseDragged"];
}

- (void) scrollWheel:(NSEvent*)event
{
  [self output:[NSString stringWithFormat:@"scrollWheel\tdeltaX:%.03f\tdeltaY:%.03f\tdeltaZ:%.03f\tflags:%@", [event deltaX], [event deltaY], [event deltaZ], [self modifierFlagsToString:[event modifierFlags]]]];
}

- (void) swipeWithEvent:(NSEvent*)event
{
  [self output:[NSString stringWithFormat:@"swipeWithEvent\tdeltaX:%.03f\tdeltaY:%.03f\tflags:%@", [event deltaX], [event deltaY], [self modifierFlagsToString:[event modifierFlags]]]];
}

@end
