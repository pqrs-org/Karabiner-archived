// -*- Mode: objc -*-
//
// KeyResponder.m
//

#import "KeyResponder.h"

@implementation KeyResponder

//
// Key event handlers
//

- (NSString*) modifierFlagsToString:(NSUInteger)flags
{
  return [NSString stringWithFormat:@"%4s %5s %4s %3s %3s %6s %4s %2s",
                   ((flags & NSAlphaShiftKeyMask) ? "Caps"   : ""),
                   ((flags & NSShiftKeyMask)      ? "Shift"  : ""),
                   ((flags & NSControlKeyMask)    ? "Ctrl"   : ""),
                   ((flags & NSAlternateKeyMask)  ? "Opt"    : ""),
                   ((flags & NSCommandKeyMask)    ? "Cmd"    : ""),
                   ((flags & NSNumericPadKeyMask) ? "NumPad" : ""),
                   ((flags & NSHelpKeyMask)       ? "Help"   : ""),
                   ((flags & NSFunctionKeyMask)   ? "FN"     : "")];
}

- (NSString*) keycodeToString:(NSEvent*)event
{
  unsigned short keycode = [event keyCode];

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
  if (keycode == 0x69) return @"F13";
  if (keycode == 0x6b) return @"F14";
  if (keycode == 0x71) return @"F15";

  if (keycode == 0x74) return @"PageUp";
  if (keycode == 0x79) return @"PageDown";
  if (keycode == 0x73) return @"Home";
  if (keycode == 0x77) return @"End";

  if (keycode == 0x7e) return @"Up";
  if (keycode == 0x7d) return @"Down";
  if (keycode == 0x7b) return @"Left";
  if (keycode == 0x7c) return @"Right";

  if (keycode == 0x24) return @"Return";
  if (keycode == 0x30) return @"Tab";
  if (keycode == 0x31) return @"Space";
  if (keycode == 0x33) return @"Delete";
  if (keycode == 0x47) return @"Clear";
  if (keycode == 0x4c) return @"Enter";
  if (keycode == 0x66) return @"JIS_EISUU";
  if (keycode == 0x68) return @"JIS_KANA";
  if (keycode == 0x75) return @"ForwardDelete";

  return [event charactersIgnoringModifiers];
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

- (void) outputKeyEvent:(NSEvent*)event eventType:(const char*)eventType
{
  [self output:[NSString stringWithFormat:@"%-7s %-14s (0x%02x) [%@]",
                         eventType,
                         [[self keycodeToString:event] UTF8String],
                         [event keyCode],
                         [self modifierFlagsToString:[event modifierFlags]]]];
}

- (void) keyDown:(NSEvent*)event
{
  [self outputKeyEvent:event eventType:"keyDown"];
}

- (void) keyUp:(NSEvent*)event
{
  [self outputKeyEvent:event eventType:"keyUp"];
}

- (void) flagsChanged:(NSEvent*)event
{
  [self outputKeyEvent:event eventType:"keyMod"];
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
