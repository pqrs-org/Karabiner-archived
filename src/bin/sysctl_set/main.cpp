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
main(int argc, char** argv)
{
  if (argc != 3) {
    fprintf(stderr, "Usage: %s key value\n", argv[0]);
    return 1;
  }

  if (! verifyUser()) {
    return 1;
  }

  char name[512];
  snprintf(name, sizeof(name), "keyremap4macbook.%s", argv[1]);

  if (strcmp(argv[1], "socket_path") == 0) {
    char* value = argv[2];
    size_t oldlen = 0;
    size_t newlen = strlen(value) + 1;
    int error = sysctlbyname(name, NULL, &oldlen, value, newlen);

    if (error) {
      //perror("sysctl_set socket_path");
      return 1;
    }

  } else if (strcmp(argv[1], "do_reset") == 0 ||
             strcmp(argv[1], "do_reload_xml") == 0 ||
             strcmp(argv[1], "do_reload_only_config") == 0) {
    int value = atoi(argv[2]);

    size_t oldlen = 0;
    size_t newlen = sizeof(value);
    int error = sysctlbyname(name, NULL, &oldlen, &value, newlen);

    if (error) {
      //perror("sysctl_set");
      return 1;
    }
  } else {
    fprintf(stderr, "Can't change keyremap4macbook.%s by sysctl_set\n", argv[1]);
    return 1;
  }

  return 0;
}
