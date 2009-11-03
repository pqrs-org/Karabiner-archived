#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include "server.hpp"
#include "util.h"

#include <CoreFoundation/CoreFoundation.h>
#include <SystemConfiguration/SystemConfiguration.h>

int
verifyUser(void)
{
  uid_t consoleUID;
  CFStringRef result = SCDynamicStoreCopyConsoleUser(NULL, &consoleUID, NULL);
  if (! result) return false;
  CFRelease(result);

  return (getuid() == consoleUID);
}

void
runServer(void)
{
  umask(0077);

  KeyRemap4MacBook_server::Server server;
  server.doLoop();
}
