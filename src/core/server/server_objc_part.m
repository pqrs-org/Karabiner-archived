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
  }

  CFRelease(ref);
}


// ----------------------------------------------------------------------
static TISInputSourceRef
copySelectableInputSourceForInputMode(CFStringRef inputmode)
{
  TISInputSourceRef inputsource = NULL;
  CFDictionaryRef filter = NULL;
  CFArrayRef list = NULL;

  if (! inputmode) {
    inputsource = TISCopyCurrentASCIICapableKeyboardInputSource();
    goto finish;
  }

  // ------------------------------------------------------------
  const void* keys[] = {
    kTISPropertyInputSourceIsSelectCapable,
    kTISPropertyInputModeID,
  };
  const void* values[] = {
    kCFBooleanTrue,
    inputmode,
  };

  filter = CFDictionaryCreate(NULL, keys, values, 2, NULL, NULL);
  if (! filter) goto finish;

  list = TISCreateInputSourceList(filter, false);
  if (! list) goto finish;

  for (int i = 0; i < CFArrayGetCount(list); ++i) {
    TISInputSourceRef source = (TISInputSourceRef)(CFArrayGetValueAtIndex(list, i));
    if (! source) continue;

    inputsource = source;
    CFRetain(inputsource);
    break;
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

void
selectInputSource(CFStringRef inputmode)
{
  TISInputSourceRef inputsource = copySelectableInputSourceForInputMode(inputmode);
  if (! inputsource) return;

  TISSelectInputSource(inputsource);
  CFRelease(inputsource);
}


// ======================================================================
void
selectInputSource_ascii(void)
{
  selectInputSource(NULL);
}
void
selectInputSource_japanese(void)
{
  selectInputSource(CFSTR("com.apple.inputmethod.Japanese"));
}
void
selectInputSource_japanese_katakana(void)
{
  selectInputSource(CFSTR("com.apple.inputmethod.Japanese.Katakana"));
}
