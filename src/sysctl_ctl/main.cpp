#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>

#include <CoreFoundation/CoreFoundation.h>

namespace {
  CFStringRef applicationID = CFSTR("org.pqrs.KeyRemap4MacBook");
  CFMutableDictionaryRef dict_sysctl = NULL;

  void
  save(const char *name)
  {
    if (! dict_sysctl) return;

    char entry[512];
    snprintf(entry, sizeof(entry), "keyremap4macbook.%s", name);

    int value;
    size_t len = sizeof(value);
    if (sysctlbyname(entry, &value, &len, NULL, 0) == -1) return;

    CFStringRef key = CFStringCreateWithCString(NULL, name, kCFStringEncodingUTF8);
    CFNumberRef val = CFNumberCreate(NULL, kCFNumberIntType, &value);
    CFDictionarySetValue(dict_sysctl, key, val);
  }

  void
  load(const char *name)
  {
    if (! dict_sysctl) return;
    CFStringRef key = CFStringCreateWithCString(NULL, name, kCFStringEncodingUTF8);

    CFNumberRef val = reinterpret_cast<CFNumberRef>(CFDictionaryGetValue(dict_sysctl, key));
    if (! val) return;

    int value;
    if (! CFNumberGetValue(val, kCFNumberIntType, &value)) return;

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
  saveToFile(const char **targetFiles, CFStringRef configKey)
  {
    dict_sysctl = CFDictionaryCreateMutable(NULL, 0, NULL, NULL);
    if (! dict_sysctl) return;

    for (int i = 0; ; ++i) {
      const char *filename = targetFiles[i];
      if (! filename) break;
      scanLines(filename, save);
    }

    CFPreferencesSetAppValue(configKey, dict_sysctl, applicationID);
    CFPreferencesAppSynchronize(applicationID);

    CFRelease(dict_sysctl); dict_sysctl = NULL;
  }

  void
  loadFromFile(const char **targetFiles, CFStringRef configKey) {
    dict_sysctl = reinterpret_cast<CFMutableDictionaryRef>(const_cast<void *>(CFPreferencesCopyAppValue(configKey, applicationID)));
    if (! dict_sysctl) return;

    for (int i = 0; ; ++i) {
      const char *filename = targetFiles[i];
      if (! filename) break;
      scanLines(filename, load);
    }

    CFRelease(dict_sysctl); dict_sysctl = NULL;
  }
}


int
main(int argc, char **argv)
{
  if (argc == 1) {
    fprintf(stderr, "Usage: %s (save|load) [params]\n", argv[0]);
  }

  Boolean isOK;
  CFIndex value = CFPreferencesGetAppIntegerValue(CFSTR("selectedIndex"), applicationID, &isOK);
  if (! isOK) value = 0;
  if (value < 0) value = 0;

  CFStringRef configKey = CFStringCreateWithFormat(NULL, NULL, CFSTR("config_%d"), value);

  const char *targetFiles[] = {
    "/Library/org.pqrs/KeyRemap4MacBook/prefpane/checkbox.xml",
    "/Library/org.pqrs/KeyRemap4MacBook/prefpane/number.xml",
    NULL,
  };

  if (argc > 1 && strcmp(argv[1], "load") == 0) {
    loadFromFile(targetFiles, configKey);
  } else {
    saveToFile(targetFiles, configKey);
  }

  return 0;
}
