#include <sys/types.h>
#include <stdio.h>
#import <AppKit/AppKit.h>
#import <Carbon/Carbon.h>
#import "server_objc_part.h"

static NSAutoreleasePool *pool = nil;

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
getActiveApplicationName(char *buffer, size_t len)
{
  if (! buffer) return;
  if (len <= 0) return;
  buffer[0] = '\0';

  // ----------------------------------------
  NSWorkspace *ws = [NSWorkspace sharedWorkspace];
  if (! ws) return;

  NSDictionary *app = [ws activeApplication];
  if (! app) return;

  NSString *nsappname = [app objectForKey:@"NSApplicationBundleIdentifier"];
  if (nsappname) {
    snprintf(buffer, len, "%s", [nsappname UTF8String]);
  }
}

void
getTISPropertyInputModeID(char *buffer, size_t len)
{
  if (! buffer) return;
  if (len <= 0) return;
  buffer[0] = '\0';

  // ----------------------------------------
  TISInputSourceRef ref = TISCopyCurrentKeyboardInputSource();
  if (! ref) return;

  NSString *inputmodeid = TISGetInputSourceProperty(ref, kTISPropertyInputModeID);
  if (inputmodeid) {
    snprintf(buffer, len, "%s", [inputmodeid UTF8String]);
  }

  CFRelease(ref);
}
