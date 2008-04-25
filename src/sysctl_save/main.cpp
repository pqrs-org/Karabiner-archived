#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>

#include <CoreFoundation/CoreFoundation.h>

namespace {
  CFStringRef applicationID = CFSTR("org.pqrs.KeyRemap4MacBook");

  void
  save(const char *name)
  {
    char entry[512];
    snprintf(entry, sizeof(entry), "keyremap4macbook.%s", name);

    int value;
    size_t len = sizeof(value);
    if (sysctlbyname(entry, &value, &len, NULL, 0) == -1) return;

    CFStringRef key = CFStringCreateWithCString(NULL, name, kCFStringEncodingUTF8);
    CFNumberRef val = CFNumberCreate(NULL, kCFNumberIntType, &value);
    CFPreferencesSetAppValue(key, val, applicationID);
  }

  void
  load(const char *name)
  {
    CFStringRef key = CFStringCreateWithCString(NULL, name, kCFStringEncodingUTF8);

    Boolean isOK;
    CFIndex value = CFPreferencesGetAppIntegerValue(key, applicationID, &isOK);
    if (! isOK) return;

    char cmd[512];
    snprintf(cmd, sizeof(cmd), "/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set %s %d", name, value);
    system(cmd);
  }

  void
  scanLines(const char *filename, void (*func)(const char *))
  {
    std::ifstream ifs(filename);
    if (! ifs) return;

    while (! ifs.eof()) {
      char line[512];

      ifs.getline(line, sizeof(line));

      const char *sysctl_begin = "<sysctl>";
      const char *sysctl_end = "</sysctl>";

      char *begin = strstr(line, "<sysctl>");
      if (! begin) continue;
      char *end = strstr(line, sysctl_end);
      if (! end) continue;

      begin += strlen(sysctl_begin);
      *end = '\0';

      func(begin);
    }
  }

  void
  saveToFile(const char *filename)
  {
    scanLines(filename, save);
    CFPreferencesAppSynchronize(applicationID);
  }

  void
  loadFromFile(const char *filename) {
    scanLines(filename, load);
  }
}


int
main(int argc, char **argv)
{
  if (argc > 1 && strcmp(argv[1], "--load") == 0) {
    loadFromFile("/Library/org.pqrs/KeyRemap4MacBook/prefpane/checkbox.xml");
    loadFromFile("/Library/org.pqrs/KeyRemap4MacBook/prefpane/number.xml");
  } else {
    saveToFile("/Library/org.pqrs/KeyRemap4MacBook/prefpane/checkbox.xml");
    saveToFile("/Library/org.pqrs/KeyRemap4MacBook/prefpane/number.xml");
  }

  return 0;
}
