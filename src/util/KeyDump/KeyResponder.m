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
  return [NSString stringWithFormat:@"%s%s%s%s%s%s%s%s",
          ((flags & NSAlphaShiftKeyMask) ? "Caps ":""),
          ((flags & NSShiftKeyMask)      ? "Shift ":""),
          ((flags & NSControlKeyMask)    ? "Ctrl ":""),
          ((flags & NSAlternateKeyMask)  ? "Opt ":""),
          ((flags & NSCommandKeyMask)    ? "Cmd ":""),
          ((flags & NSNumericPadKeyMask) ? "NumPad ":""),
          ((flags & NSHelpKeyMask)       ? "Help ":""),
          ((flags & NSFunctionKeyMask)   ? "Fn ":"")];
}

- (NSString*) specialKeycodeToString:(unsigned short)keycode
{
  if (keycode == 0x35) return @"Escape";
  if (keycode == 0x36) return @"Command_R";
  if (keycode == 0x37) return @"Command_L";
  if (keycode == 0x38) return @"Shift_L";
  if (keycode == 0x39) return @"CapsLock";
  if (keycode == 0x3a) return @"Option_L";
  if (keycode == 0x3b) return @"Control_L";
  if (keycode == 0x3c) return @"Shift_R";
  if (keycode == 0x3d) return @"Option_R";
  if (keycode == 0x3e) return @"Control_R";
  if (keycode == 0x3f) return @"Fn";

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
  if (keycode == 0x6a) return @"F16";
  if (keycode == 0x40) return @"F17";
  if (keycode == 0x4f) return @"F18";
  if (keycode == 0x50) return @"F19";

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
  if (keycode == 0x6e) return @"Application";
  if (keycode == 0x75) return @"ForwardDelete";

  return nil;
}

- (NSString*) keycodeToString:(NSEvent*)event
{
  unsigned short keycode = [event keyCode];

  NSString* string = [self specialKeycodeToString:keycode];
  if (string) return string;

  // --------------------
  // Difference of "characters" and "charactersIgnoringModifiers".
  //
  // [NSEvent characters]
  //   Option+z => Ω
  //
  // [NSEvent charactersIgnoringModifiers]
  //   Option+z => z
  //
  // We prefer "Shift+Option+z" style than "Shift+Ω".
  // Therefore we use charactersIgnoringModifiers here.
  //
  // --------------------
  // However, there is a problem.
  // When we use "Dvorak - Qwerty ⌘" as Input Source,
  // charactersIgnoringModifiers returns ';'.
  // It's wrong. Input Source does not change Command+z.
  // So, we need to use 'characters' in this case.
  //
  // [NSEvent characters]
  //    Command+z => z
  // [NSEvent charactersIgnoringModifierss]
  //    Command+z => ;
  //
  //
  // --------------------
  // But, we cannot use these properly without information about current Input Source.
  // And this information cannot get by program.
  //
  // So, we use charactersIgnoringModifierss here and
  // display the result of 'characters' in the 'misc' field.

  @try {
    return [event charactersIgnoringModifiers];
  } @catch (NSException* exception) {}
  return @"";
}

- (NSString*) charactersToString:(NSEvent*)event
{
  unsigned short keycode = [event keyCode];

  // We ignore special characters.
  NSString* string = [self specialKeycodeToString:keycode];
  if (string) return @"";

  @try {
    return [event characters];
  } @catch (NSException* exception) {}
  return @"";
}

- (NSString*) buttonToString:(NSEvent*)event
{
  NSInteger number = [event buttonNumber];
  if (number == 0) return @"left";
  if (number == 1) return @"right";
  if (number == 2) return @"middle";

  return [NSString stringWithFormat:@"button%d", number + 1];
}

- (void) outputKeyEvent:(NSEvent*)event eventType:(NSString*)eventType
{
  [eventqueue_ push:eventType
               code:[NSString stringWithFormat:@"0x%x", [event keyCode]]
               name:[self keycodeToString:event]
              flags:[self modifierFlagsToString:[event modifierFlags]]
               misc:[NSString stringWithFormat:@"characters:%@", [self charactersToString:event]]];
}

- (void) outputMouseEvent:(NSEvent*)event eventType:(NSString*)eventType
{
  [eventqueue_ push:eventType
               code:[NSString stringWithFormat:@"0x%x", [event buttonNumber]]
               name:[self buttonToString:event]
              flags:[self modifierFlagsToString:[event modifierFlags]]
               misc:[NSString stringWithFormat:@"%@ %d", NSStringFromPoint([event locationInWindow]), [event clickCount]]];
}

- (void) keyDown:(NSEvent*)event
{
  [self outputKeyEvent:event eventType:@"keyDown"];
}

- (void) keyUp:(NSEvent*)event
{
  [self outputKeyEvent:event eventType:@"keyUp"];
}

- (void) flagsChanged:(NSEvent*)event
{
  [self outputKeyEvent:event eventType:@"keyMod"];
}

- (void) mouseDown:(NSEvent*)event
{
  [self outputMouseEvent:event eventType:@"mouseDown"];
}

- (void) mouseUp:(NSEvent*)event
{
  [self outputMouseEvent:event eventType:@"mouseUp"];
}

- (void) mouseDragged:(NSEvent*)event
{
  [self outputMouseEvent:event eventType:@"mouseDragged"];
}

- (void) rightMouseDown:(NSEvent*)event
{
  [self outputMouseEvent:event eventType:@"mouseDown"];
}

- (void) rightMouseUp:(NSEvent*)event
{
  [self outputMouseEvent:event eventType:@"mouseUp"];
}

- (void) rightMouseDragged:(NSEvent*)event
{
  [self outputMouseEvent:event eventType:@"mouseDragged"];
}

- (void) otherMouseDown:(NSEvent*)event
{
  [self outputMouseEvent:event eventType:@"mouseDown"];
}

- (void) otherMouseUp:(NSEvent*)event
{
  [self outputMouseEvent:event eventType:@"mouseUp"];
}

- (void) otherMouseDragged:(NSEvent*)event
{
  [self outputMouseEvent:event eventType:@"mouseDragged"];
}

- (void) scrollWheel:(NSEvent*)event
{
  [eventqueue_ push:@"scroll"
               code:@""
               name:@""
              flags:[self modifierFlagsToString:[event modifierFlags]]
               misc:[NSString stringWithFormat:@"dx:%.03f dy:%.03f dz:%.03f", [event deltaX], [event deltaY], [event deltaZ]]];
}

- (void) swipeWithEvent:(NSEvent*)event
{
  [eventqueue_ push:@"swipe"
               code:@""
               name:@""
              flags:[self modifierFlagsToString:[event modifierFlags]]
               misc:[NSString stringWithFormat:@"dx:%.03f dy:%.03f", [event deltaX], [event deltaY]]];
}

@end
