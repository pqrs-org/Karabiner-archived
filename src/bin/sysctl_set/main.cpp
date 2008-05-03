#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdio.h>
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
  if (argc != 3) {
    fprintf(stderr, "Usage: %s key value\n", argv[0]);
    return 1;
  }

  if (! verifyUser()) {
    fprintf(stderr, "Permission denied\n");
    return 1;
  }

  char name[512];
  snprintf(name, sizeof(name), "keyremap4macbook.%s", argv[1]);

  int value = atoi(argv[2]);

  size_t oldlen = 0;
  size_t newlen = sizeof(value);
  int error = sysctlbyname(name, NULL, &oldlen, &value, newlen);

  if (error) {
    perror("sysctl");
    return 1;
  }

  fprintf(stderr, "%s -> %d\n", name, value);

  return 0;
}
