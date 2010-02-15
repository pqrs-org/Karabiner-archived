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
  NSEnumerator* e = [a objectEnumerator];
  for (;;) {
    NSRunningApplication* app = [e nextObject];
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
    CFStringRef lang = [self getInputSourceLanguage:ref];
    if (lang) {
      retval = [NSString stringWithFormat:@"org.pqrs.inputmode.%@", lang];
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

@end

// ======================================================================
static ServerObjcPart* serverobjcpart = nil;

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


// ======================================================================
static NSWindow* statuswindow = nil;
static NSTextField* statuswindow_label = nil;
static NSString* statusmessage_lock = nil;
static NSString* statusmessage_extra = nil;

void
registerStatusWindow(NSWindow* window, NSTextField* label)
{
  statuswindow = window;
  statuswindow_label = label;
}

static void
updateStatusMessageWindow(void) {
  if (! statuswindow) return;
  if (! statuswindow_label) return;

  NSMutableString* message = [[NSMutableString alloc] init];
  if (statusmessage_lock && [statusmessage_lock length] > 0) {
    [message appendString:statusmessage_lock];
    [message appendString:@"\n"];
  }
  if (statusmessage_extra && [statusmessage_extra length] > 0) {
    [message appendString:statusmessage_extra];
  }

  [statuswindow_label setStringValue:message];

  if ([message length] > 0) {
    // show
    [statuswindow orderFront:nil];
  } else {
    // hide
    [statuswindow orderOut:nil];
  }
}

void
set_statusmessage(StatusMessageType type, const char* message)
{
  NSString* s = [NSString stringWithCString:message encoding:NSUTF8StringEncoding];

  switch (type) {
  case STATUSMESSAGETYPE_LOCK:
    statusmessage_lock = s;
    break;
  case STATUSMESSAGETYPE_EXTRA:
    statusmessage_extra = s;
    break;
  default:
    break;
  }

  // XXX: FIXME: don't touch UI from no-main-thread!!!
  //[s performSelectorOnMainThread:@selector(updateStatusMessageWindow) withObject:nil waitUntilDone:NO];
  updateStatusMessageWindow();
}
