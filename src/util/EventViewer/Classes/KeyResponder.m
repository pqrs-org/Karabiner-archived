// -*- Mode: objc -*-

#import "KeyRemap4MacBookClient.h"
#import "KeyResponder.h"

@implementation KeyResponder

- (void) drawRect:(NSRect)dirtyRect
{
  [NSGraphicsContext saveGraphicsState];
  {
    NSRect bounds = [self bounds];

    CGFloat w = bounds.size.width / 40;
    NSRect text = NSMakeRect(w * 2,
                             w * 2,
                             bounds.size.width - w * 4,
                             bounds.size.height - w * 4);

    // Draw area
    [[[NSColor whiteColor] colorWithAlphaComponent:0.8] set];
    [[NSBezierPath bezierPathWithRoundedRect:bounds xRadius:10 yRadius:10] fill];

    [[[NSColor blackColor] colorWithAlphaComponent:0.6] set];
    NSBezierPath* path = [NSBezierPath bezierPathWithRoundedRect:bounds xRadius:10 yRadius:10];
    [path setLineWidth:w];
    [path stroke];

    // Draw texts
    NSDictionary* attributes = @{ NSFontAttributeName :[NSFont boldSystemFontOfSize:(text.size.width / 10)],
                                  NSForegroundColorAttributeName :[[NSColor blackColor] colorWithAlphaComponent:0.6], };
    [@"Mouse Area" drawInRect : text withAttributes : attributes];
  }
  [NSGraphicsContext restoreGraphicsState];
}

// ============================================================
//
// Key event handlers
//

- (NSString*) modifierFlagsToString:(NSUInteger)flags
{
  return [NSString stringWithFormat:@"%s%s%s%s%s%s%s%s",
          ((flags & NSAlphaShiftKeyMask) ? "Caps " : ""),
          ((flags & NSShiftKeyMask)      ? "Shift " : ""),
          ((flags & NSControlKeyMask)    ? "Ctrl " : ""),
          ((flags & NSAlternateKeyMask)  ? "Opt " : ""),
          ((flags & NSCommandKeyMask)    ? "Cmd " : ""),
          ((flags & NSNumericPadKeyMask) ? "NumPad " : ""),
          ((flags & NSHelpKeyMask)       ? "Help " : ""),
          ((flags & NSFunctionKeyMask)   ? "Fn " : "")];
}

- (NSString*) specialKeycodeToString:(NSEvent*)event
{
  unsigned short keycode = [event keyCode];

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

  if (keycode == 0x24) return @"Return";
  if (keycode == 0x30) return @"Tab";
  if (keycode == 0x31) return @"Space";
  if (keycode == 0x33) return @"Delete";
  if (keycode == 0x4c) return @"Enter";
  if (keycode == 0x66) return @"JIS_EISUU";
  if (keycode == 0x68) return @"JIS_KANA";
  if (keycode == 0x6e) return @"Application";

  // ------------------------------------------------------------
  @try {
    NSString* characters = [event characters];
    if ([characters length] > 0) {
      unichar unichar = [characters characterAtIndex:0];

      if (unichar == NSUpArrowFunctionKey)    { return @"Up";    }
      if (unichar == NSDownArrowFunctionKey)  { return @"Down";  }
      if (unichar == NSLeftArrowFunctionKey)  { return @"Left";  }
      if (unichar == NSRightArrowFunctionKey) { return @"Right"; }

      if (unichar == NSF1FunctionKey)  { return @"F1";  }
      if (unichar == NSF2FunctionKey)  { return @"F2";  }
      if (unichar == NSF3FunctionKey)  { return @"F3";  }
      if (unichar == NSF4FunctionKey)  { return @"F4";  }
      if (unichar == NSF5FunctionKey)  { return @"F5";  }
      if (unichar == NSF6FunctionKey)  { return @"F6";  }
      if (unichar == NSF7FunctionKey)  { return @"F7";  }
      if (unichar == NSF8FunctionKey)  { return @"F8";  }
      if (unichar == NSF9FunctionKey)  { return @"F9";  }
      if (unichar == NSF10FunctionKey) { return @"F10"; }
      if (unichar == NSF11FunctionKey) { return @"F11"; }
      if (unichar == NSF12FunctionKey) { return @"F12"; }
      if (unichar == NSF13FunctionKey) { return @"F13"; }
      if (unichar == NSF14FunctionKey) { return @"F14"; }
      if (unichar == NSF15FunctionKey) { return @"F15"; }
      if (unichar == NSF16FunctionKey) { return @"F16"; }
      if (unichar == NSF17FunctionKey) { return @"F17"; }
      if (unichar == NSF18FunctionKey) { return @"F18"; }
      if (unichar == NSF19FunctionKey) { return @"F19"; }
      if (unichar == NSF20FunctionKey) { return @"F20"; }
      if (unichar == NSF21FunctionKey) { return @"F21"; }
      if (unichar == NSF22FunctionKey) { return @"F22"; }
      if (unichar == NSF23FunctionKey) { return @"F23"; }
      if (unichar == NSF24FunctionKey) { return @"F24"; }
      if (unichar == NSF25FunctionKey) { return @"F25"; }
      if (unichar == NSF26FunctionKey) { return @"F26"; }
      if (unichar == NSF27FunctionKey) { return @"F27"; }
      if (unichar == NSF28FunctionKey) { return @"F28"; }
      if (unichar == NSF29FunctionKey) { return @"F29"; }
      if (unichar == NSF30FunctionKey) { return @"F30"; }
      if (unichar == NSF31FunctionKey) { return @"F31"; }
      if (unichar == NSF32FunctionKey) { return @"F32"; }
      if (unichar == NSF33FunctionKey) { return @"F33"; }
      if (unichar == NSF34FunctionKey) { return @"F34"; }
      if (unichar == NSF35FunctionKey) { return @"F35"; }

      if (unichar == NSInsertFunctionKey)       { return @"Insert";           }
      if (unichar == NSDeleteFunctionKey)       { return @"Forward Delete";   }
      if (unichar == NSHomeFunctionKey)         { return @"Home";             }
      if (unichar == NSBeginFunctionKey)        { return @"Begin";            }
      if (unichar == NSEndFunctionKey)          { return @"End";              }
      if (unichar == NSPageUpFunctionKey)       { return @"Page Up";          }
      if (unichar == NSPageDownFunctionKey)     { return @"Page Down";        }
      if (unichar == NSPrintScreenFunctionKey)  { return @"Print Screen";     }
      if (unichar == NSScrollLockFunctionKey)   { return @"Scroll Lock";      }
      if (unichar == NSPauseFunctionKey)        { return @"Pause";            }
      if (unichar == NSSysReqFunctionKey)       { return @"System Request";   }
      if (unichar == NSBreakFunctionKey)        { return @"Break";            }
      if (unichar == NSResetFunctionKey)        { return @"Reset";            }
      if (unichar == NSStopFunctionKey)         { return @"Stop";             }
      if (unichar == NSMenuFunctionKey)         { return @"Menu";             }
      if (unichar == NSUserFunctionKey)         { return @"User";             }
      if (unichar == NSSystemFunctionKey)       { return @"System";           }
      if (unichar == NSPrintFunctionKey)        { return @"Print";            }
      if (unichar == NSClearLineFunctionKey)    { return @"Clear/Num Lock";   }
      if (unichar == NSClearDisplayFunctionKey) { return @"Clear Display";    }
      if (unichar == NSInsertLineFunctionKey)   { return @"Insert Line";      }
      if (unichar == NSDeleteLineFunctionKey)   { return @"Delete Line";      }
      if (unichar == NSInsertCharFunctionKey)   { return @"Insert Character"; }
      if (unichar == NSDeleteCharFunctionKey)   { return @"Delete Character"; }
      if (unichar == NSPrevFunctionKey)         { return @"Previous";         }
      if (unichar == NSNextFunctionKey)         { return @"Next";             }
      if (unichar == NSSelectFunctionKey)       { return @"Select";           }
      if (unichar == NSExecuteFunctionKey)      { return @"Execute";          }
      if (unichar == NSUndoFunctionKey)         { return @"Undo";             }
      if (unichar == NSRedoFunctionKey)         { return @"Redo";             }
      if (unichar == NSFindFunctionKey)         { return @"Find";             }
      if (unichar == NSHelpFunctionKey)         { return @"Help";             }
      if (unichar == NSModeSwitchFunctionKey)   { return @"Mode Switch";      }
    }
  } @catch (NSException* exception) {}

  return nil;
}

- (NSString*) keycodeToString:(NSEvent*)event
{
  NSString* string = [self specialKeycodeToString:event];
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
  // We ignore special characters.
  NSString* string = [self specialKeycodeToString:event];
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

  return [NSString stringWithFormat:@"button%d", (int)(number + 1)];
}

- (void) outputKeyEvent:(NSEvent*)event eventType:(NSString*)eventType
{
  NSString* keyCodeName = [[client_ proxy] symbolMapName:@"KeyCode" value:(int)([event keyCode])];
  NSString* misc = [NSString stringWithFormat:@"characters:%@\t%@",
                    [[self charactersToString:event] stringByPaddingToLength:4 withString:@" " startingAtIndex:0],
                    keyCodeName ? keyCodeName:@""];

  [eventqueue_ push:eventType
               code:[NSString stringWithFormat:@"0x%x", (int)([event keyCode])]
               name:[self keycodeToString:event]
              flags:[self modifierFlagsToString:[event modifierFlags]]
               misc:misc];
}

- (void) outputMouseEvent:(NSEvent*)event eventType:(NSString*)eventType
{
  [eventqueue_ push:eventType
               code:[NSString stringWithFormat:@"0x%x", (int)([event buttonNumber])]
               name:[self buttonToString:event]
              flags:[self modifierFlagsToString:[event modifierFlags]]
               misc:[NSString stringWithFormat:@"%@ %d", NSStringFromPoint([event locationInWindow]), (int)([event clickCount])]];
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
