#include <sys/types.h>
#include <stdio.h>
#import <Carbon/Carbon.h>
#import "server_objc_part.h"

void
getActiveApplicationName(char* buffer, size_t len)
{
  if (! buffer) return;
  if (len <= 0) return;
  buffer[0] = '\0';

  // ----------------------------------------
  NSWorkspace* ws = [NSWorkspace sharedWorkspace];
  if (! ws) return;

  NSArray* a = [ws runningApplications];
  NSEnumerator* e = [a objectEnumerator];
  for (;;) {
    NSRunningApplication* app = [e nextObject];
    if (! app) return;

    if ([app isActive]) {
      NSString* nsappname = [app bundleIdentifier];

      if (nsappname) {
        snprintf(buffer, len, "%s", [nsappname UTF8String]);

      } else {
        // We use localizedName instead of bundleIdentifier,
        // because "MacSOUP" doesn't have bundleIdentifier.
        // http://www.haller-berlin.de/macsoup/index.html
        NSString* localizedName = [app localizedName];
        if (localizedName) {
          snprintf(buffer, len, "org.pqrs.unknownapp.%s", [localizedName UTF8String]);
        }
      }

      return;
    }
  }
}

static CFStringRef kInputSourceLanguage_canadian = CFSTR("ca");

static CFStringRef
getInputSourceLanguage(TISInputSourceRef source)
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

void
getTISPropertyInputModeID(char* buffer, size_t len)
{
  if (! buffer) return;
  if (len <= 0) return;
  buffer[0] = '\0';

  // ----------------------------------------
  TISInputSourceRef ref = TISCopyCurrentKeyboardInputSource();
  if (! ref) return;

  NSString* inputmodeid = TISGetInputSourceProperty(ref, kTISPropertyInputModeID);
  if (inputmodeid) {
    snprintf(buffer, len, "%s", [inputmodeid UTF8String]);
  } else {
    CFStringRef lang = getInputSourceLanguage(ref);
    if (lang) {
      snprintf(buffer, len, "org.pqrs.inputmode.%s", [(NSString*)(lang) UTF8String]);
    }
  }
  //NSLog(@"buffer: %s", buffer);

  CFRelease(ref);
}


// ----------------------------------------------------------------------
// Note:
// TISCopyInputSourceForLanguage returns unselectable InputSource.
// Therefore we get InputSource by ourself.
static TISInputSourceRef
copySelectableInputSourceForLanguage(CFStringRef language)
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

    CFStringRef lang = getInputSourceLanguage(source);
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

static void
selectInputSource_language(CFStringRef language)
{
  TISInputSourceRef inputsource = copySelectableInputSourceForLanguage(language);
  if (! inputsource) return;

  TISSelectInputSource(inputsource);
  CFRelease(inputsource);
}

// ======================================================================
void selectInputSource_canadian(void) { selectInputSource_language(CFSTR("ca")); }
void selectInputSource_english(void) { selectInputSource_language(CFSTR("en")); }
void selectInputSource_french(void) { selectInputSource_language(CFSTR("fr")); }
void selectInputSource_german(void) { selectInputSource_language(CFSTR("de")); }
void selectInputSource_japanese(void) { selectInputSource_language(CFSTR("ja")); }
void selectInputSource_swedish(void) { selectInputSource_language(CFSTR("sv")); }
