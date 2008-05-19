#include <stdlib.h>
#include <unistd.h>

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
main(int argc, char **argv)
{
  if (! verifyUser()) {
    fprintf(stderr, "Permission denied\n");
    return 1;
  }

  system("/Library/org.pqrs/KeyRemap4MacBook/extra/uninstall.sh");
  return 0;
}
