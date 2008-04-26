#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "server.hpp"

#include <CoreFoundation/CoreFoundation.h>
#include <SystemConfiguration/SystemConfiguration.h>

namespace {
  bool
  verifyUser(void)
  {
    uid_t consoleUID;
    CFStringRef result = SCDynamicStoreCopyConsoleUser(NULL, &consoleUID, NULL);
    if (! result) return false;
    CFRelease(result);

    return (getuid() == consoleUID);
  }
}


int
main()
{
  if (! verifyUser()) {
    fprintf(stderr, "Permission denied\n");
    return 1;
  }

  umask(0077);

  KeyRemap4MacBook_server::Server server;
  server.doLoop();

  return 0;
}
