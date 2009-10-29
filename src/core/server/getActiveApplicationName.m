#include <sys/types.h>
#include <stdio.h>
#import <AppKit/AppKit.h>
#import "getActiveApplicationName.h"

void
getActiveApplicationName(char *buffer, size_t len)
{
  if (! buffer) return;
  if (len <= 0) return;
  buffer[0] = '\0';

  // ----------------------------------------
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

  NSWorkspace *ws = [NSWorkspace sharedWorkspace];
  if (! ws) goto finish;

  NSDictionary *app = [ws activeApplication];
  if (! app) goto finish;

  NSString *nsappname = [app objectForKey:@"NSApplicationBundleIdentifier"];
  snprintf(buffer, len, "%s", [nsappname UTF8String]);

 finish:
  [pool drain];
}
