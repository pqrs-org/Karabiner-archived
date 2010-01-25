#include <sys/types.h>
#include <stdio.h>
#import <AppKit/AppKit.h>
#import <Carbon/Carbon.h>
#import "server_objc_part.h"

static NSAutoreleasePool* pool = nil;

void
autoreleasepool_begin(void)
{
  if (pool) {
    autoreleasepool_end();
  }
  pool = [[NSAutoreleasePool alloc] init];
}

void
autoreleasepool_end(void)
{
  if (pool) {
    [pool drain];
    pool = nil;
  }
}

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
