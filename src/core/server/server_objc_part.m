#include <sys/types.h>
#include <stdio.h>
#import <Carbon/Carbon.h>
#import "server_objc_part.h"

@implementation ServerObjcPart

- (NSString*) getActiveApplicationName
{
  // ----------------------------------------
  NSWorkspace* ws = [NSWorkspace sharedWorkspace];
  if (! ws) return nil;

  NSArray* a = [ws runningApplications];
  if (! a) return nil;

  for (NSRunningApplication* app in a) {
    if (! app) return nil;

    if ([app isActive]) {
      NSString* nsappname = [app bundleIdentifier];

      if (nsappname) {
        return [NSString stringWithString:nsappname];

      } else {
        // We use localizedName instead of bundleIdentifier,
        // because "MacSOUP" doesn't have bundleIdentifier.
        // http://www.haller-berlin.de/macsoup/index.html
        NSString* localizedName = [app localizedName];
        if (localizedName) {
          return [NSString stringWithFormat:@"org.pqrs.unknownapp.%@", localizedName];
        }
      }
    }
  }

  return nil;
}

// ======================================================================
static CFStringRef kInputSourceLanguage_canadian = CFSTR("ca");

- (CFStringRef) getInputSourceLanguage:(TISInputSourceRef)source
{
  // Because we cannot distinguish en and ca from kTISPropertyInputSourceLanguages,
  // we use kTISPropertyInputSourceID at first.
  //
  // Note:
  // kTISPropertyInputSourceID is different every IM, and
  // it is desirable as possible to use kTISPropertyInputSourceLanguages because
  // kTISPropertyInputSourceID does not get the correspondence with the input language.
  //
  // Example:
  //   kTISPropertyInputSourceID: jp.sourceforge.inputmethod.aquaskk
  //   kTISPropertyInputSourceLanguages: ja
  //
  //   kTISPropertyInputSourceID: com.apple.inputmethod.Kotoeri.Japanese
  //   kTISPropertyInputSourceLanguages: ja
  //
  //   * These two IM are Japanese input method.

  CFStringRef name = TISGetInputSourceProperty(source, kTISPropertyInputSourceID);
  if (name) {
    if (CFStringCompare(name, CFSTR("com.apple.keylayout.Canadian"), 0) == kCFCompareEqualTo) {
      return kInputSourceLanguage_canadian;
    }
  }

  CFArrayRef languages = TISGetInputSourceProperty(source, kTISPropertyInputSourceLanguages);
  if (languages && CFArrayGetCount(languages) > 0) {
    // U.S. InputSource has many languages (en, de, fr, ...),
    // so we check the first language only to detect real InputSource for French, German, etc.
    CFStringRef lang = CFArrayGetValueAtIndex(languages, 0);
    if (lang) {
      return lang;
    }
  }

  return NULL;
}

- (NSString*) getTISPropertyInputModeID
{
  TISInputSourceRef ref = TISCopyCurrentKeyboardInputSource();
  if (! ref) return nil;

  NSString* retval = nil;
  NSString* inputmodeid = TISGetInputSourceProperty(ref, kTISPropertyInputModeID);
  if (inputmodeid) {
    retval = [NSString stringWithString:inputmodeid];
  } else {
    // ----------------------------------------
    // get detail string
    NSString* detail = @"";

    NSString* name = TISGetInputSourceProperty(ref, kTISPropertyInputSourceID);
    if (name) {
      // Examples:
      //   name == com.apple.keylayout.US
      //   name == com.apple.keylayout.Dvorak
      NSRange dotrange = [name rangeOfString:@"." options:NSBackwardsSearch];
      if (dotrange.location != NSNotFound) {
        detail = [name substringFromIndex:dotrange.location];
      }
    }

    // ----------------------------------------
    CFStringRef lang = [self getInputSourceLanguage:ref];
    if (lang && CFStringGetLength(lang) > 0) {
      retval = [NSString stringWithFormat:@"org.pqrs.inputmode.%@%@", lang, detail];
    } else {
      retval = [NSString stringWithFormat:@"org.pqrs.inputmode.unknown%@", detail];
    }
  }

  CFRelease(ref);
  return retval;
}

// ----------------------------------------------------------------------
// Note:
// TISCopyInputSourceForLanguage returns unselectable InputSource.
// Therefore we get InputSource by ourself.
- (TISInputSourceRef) copySelectableInputSourceForLanguage:(CFStringRef)language
{
  TISInputSourceRef inputsource = NULL;
  CFDictionaryRef filter = NULL;
  CFArrayRef list = NULL;

  if (! language) goto finish;

  // ------------------------------------------------------------
  const void* keys[] = {
    kTISPropertyInputSourceIsSelectCapable,
  };
  const void* values[] = {
    kCFBooleanTrue,
  };

  filter = CFDictionaryCreate(NULL, keys, values, 1, NULL, NULL);
  if (! filter) goto finish;

  list = TISCreateInputSourceList(filter, false);
  if (! list) goto finish;

  for (int i = 0; i < CFArrayGetCount(list); ++i) {
    TISInputSourceRef source = (TISInputSourceRef)(CFArrayGetValueAtIndex(list, i));
    if (! source) continue;

    CFStringRef lang = [self getInputSourceLanguage:source];
    if (! lang) continue;

    if (CFStringCompare(language, lang, 0) == kCFCompareEqualTo) {
      inputsource = source;
      CFRetain(inputsource);
      goto finish;
    }
  }

finish:
  if (filter) {
    CFRelease(filter);
  }
  if (list) {
    CFRelease(list);
  }
  return inputsource;
}

// ----------------------------------------------------------------------
- (void) registerStatusWindow:(NSWindow*)window label:(NSTextField*)label background:(NSImageView*)background
{
  statuswindow_ = window;
  statuswindow_label_ = label;
  statuswindow_backgroud_ = background;
}

- (void) updateStatusMessageWindow
{
  if (! statuswindow_) return;
  if (! statuswindow_label_) return;

  NSMutableString* message = [[NSMutableString alloc] init];
  if (statusmessage_lock_ && [statusmessage_lock_ length] > 0) {
    [message appendString:statusmessage_lock_];
    [message appendString:@"\n"];
  }
  if (statusmessage_extra_ && [statusmessage_extra_ length] > 0) {
    [message appendString:statusmessage_extra_];
  }

  [statuswindow_label_ setStringValue:message];

  if ([message length] > 0) {
    // show
    [statuswindow_ orderFront:nil];
  } else {
    // hide
    [statuswindow_ orderOut:nil];
  }

  [message release];
}

- (void) setStatusMessage:(StatusMessageType)type message:(const char*)message
{
  NSString* s = [NSString stringWithCString:message encoding:NSUTF8StringEncoding];

  switch (type) {
    case STATUSMESSAGETYPE_LOCK:
      statusmessage_lock_ = s;
      break;
    case STATUSMESSAGETYPE_EXTRA:
      statusmessage_extra_ = s;
      break;
    default:
      break;
  }

  [self performSelectorOnMainThread:@selector(updateStatusMessageWindow)
                         withObject:nil
                      waitUntilDone:NO];
}

- (void) setStatusMessageWindowParam:(uint32_t)alpha_font alpha_background:(uint32_t)alpha_background posx_adjustment:(int32_t)posx_adjustment posy_adjustment:(int32_t)posy_adjustment
{
  CGFloat af = (CGFloat)(alpha_font) / (CGFloat)(100.0);
  CGFloat ab = (CGFloat)(alpha_background) / (CGFloat)(100.0);

  if (af < 0) af = 0;
  if (af > 100) af = 100;

  if (ab < 0) ab = 0;
  if (ab > 100) ab = 100;

  [statuswindow_label_ setAlphaValue:af];
  [statuswindow_backgroud_ setAlphaValue:ab];

  [statuswindow_ center];
  NSRect frame = [statuswindow_ frame];
  frame.origin.x += posx_adjustment;
  frame.origin.y += posy_adjustment;
  [statuswindow_ setFrameOrigin:frame.origin];
}

@end

// ======================================================================
static ServerObjcPart * serverobjcpart = nil;

void
registerServerObjcPart(ServerObjcPart* object)
{
  serverobjcpart = object;
}

static void
selectInputSource_language(CFStringRef language)
{
  if (! serverobjcpart) return;

  TISInputSourceRef inputsource = [serverobjcpart copySelectableInputSourceForLanguage:language];
  if (! inputsource) return;

  TISSelectInputSource(inputsource);
  CFRelease(inputsource);
}

// ------------------------------------------------------------
void selectInputSource_canadian(void) { selectInputSource_language(CFSTR("ca")); }
void selectInputSource_english(void) { selectInputSource_language(CFSTR("en")); }
void selectInputSource_french(void) { selectInputSource_language(CFSTR("fr")); }
void selectInputSource_german(void) { selectInputSource_language(CFSTR("de")); }
void selectInputSource_japanese(void) { selectInputSource_language(CFSTR("ja")); }
void selectInputSource_swedish(void) { selectInputSource_language(CFSTR("sv")); }
void selectInputSource_russian(void) { selectInputSource_language(CFSTR("ru")); }

// ------------------------------------------------------------
void
reset_statusmessage(void)
{
  set_statusmessage(STATUSMESSAGETYPE_LOCK, "");
  set_statusmessage(STATUSMESSAGETYPE_EXTRA, "");
}

void
set_statusmessage(StatusMessageType type, const char* message)
{
  if (serverobjcpart) {
    [serverobjcpart setStatusMessage:type message:message];
  }
}

void
set_statusmessageWindowParam(uint32_t alpha_font, uint32_t alpha_background, int32_t posx_adjustment, int32_t posy_adjustment)
{
  if (serverobjcpart) {
    [serverobjcpart setStatusMessageWindowParam:alpha_font alpha_background:alpha_background posx_adjustment:posx_adjustment posy_adjustment:posy_adjustment];
  }
}
