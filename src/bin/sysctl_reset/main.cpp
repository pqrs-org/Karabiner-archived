#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <fstream>

#include <CoreFoundation/CoreFoundation.h>
#include <SystemConfiguration/SystemConfiguration.h>

namespace {
  bool
  verifyUser(void)
  {
    uid_t consoleUID;
    CFStringRef result = SCDynamicStoreCopyConsoleUser(NULL, &consoleUID, NULL);
    // TRUE if no user is logged in
    if (result == NULL) return true;
    CFRelease(result);

    return (getuid() == consoleUID);
  }

  void
  set(const char *name, int value)
  {
    char entry[512];
    snprintf(entry, sizeof(entry), "keyremap4macbook.%s", name);

    size_t oldlen = 0;
    size_t newlen = sizeof(value);
    if (sysctlbyname(entry, NULL, &oldlen, &value, newlen) == -1) {
      perror("sysctl");
    }
  }

  void
  set_string(const char* name, char* value)
  {
    char entry[512];
    snprintf(entry, sizeof(entry), "keyremap4macbook.%s", name);

    size_t oldlen = 0;
    size_t newlen = strlen(value) + 1;
    if (sysctlbyname(entry, NULL, &oldlen, value, newlen) == -1) {
      perror("sysctl");
    }
  }
}


int
main(int argc, char **argv)
{
  if (! verifyUser()) {
    fprintf(stderr, "Permission denied\n");
    return 1;
  }

  std::ifstream ifs("/Library/org.pqrs/KeyRemap4MacBook/share/reset");
  if (! ifs) return 1;

  while (! ifs.eof()) {
    char line[512];

    ifs.getline(line, sizeof(line));

    char *p = strchr(line, ' ');
    if (! p) continue;
    *p = '\0';

    set(line, atoi(p + 1));
  }

  if (argc == 2 && strcmp(argv[1], "terminate") == 0) {
    char socket_path[] = "";
    set_string("socket_path", socket_path);

    set("initialized", 0);
  }

  return 0;
}
