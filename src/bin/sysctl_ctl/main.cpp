#include <sys/time.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <map>

#include <CoreFoundation/CoreFoundation.h>

namespace {
  CFStringRef applicationID = CFSTR("org.pqrs.KeyRemap4MacBook");

  // ============================================================
  bool
  isInitialized(void)
  {
    int value;
    size_t len = sizeof(value);
    if (sysctlbyname("keyremap4macbook.initialized", &value, &len, NULL, 0) == -1) return false;
    return value;
  }

  // ============================================================
  // SAVE & LOAD
  CFMutableDictionaryRef dict_sysctl = NULL;
  std::map<std::string, int> map_reset;

  void
  save(const char* name)
  {
    if (! dict_sysctl) return;
    if (! name) return;

    const char* notsave = "notsave.";
    if (strncmp(name, notsave, strlen(notsave)) == 0) {
      return;
    }

    char entry[512];
    snprintf(entry, sizeof(entry), "keyremap4macbook.%s", name);

    int value;
    size_t len = sizeof(value);
    if (sysctlbyname(entry, &value, &len, NULL, 0) == -1) return;

    CFStringRef key = CFStringCreateWithCString(NULL, name, kCFStringEncodingUTF8);
    CFNumberRef val = CFNumberCreate(NULL, kCFNumberIntType, &value);

    std::map<std::string, int>::iterator it = map_reset.find(name);
    if (it == map_reset.end()) return;
    CFNumberRef defaultval = CFNumberCreate(NULL, kCFNumberIntType, &(it->second));

    if (CFNumberCompare(val, defaultval, NULL) != 0) {
      CFDictionarySetValue(dict_sysctl, key, val);
    }
  }

  void
  load(const char* name)
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
  scanLines(const char* filename, void (* func)(const char*))
  {
    std::ifstream ifs(filename);
    if (! ifs) return;

    while (! ifs.eof()) {
      char line[32 * 1024];

      ifs.getline(line, sizeof(line));

      const char* sysctl_begin = "<sysctl>";
      const char* sysctl_end = "</sysctl>";

      char* begin = strstr(line, "<sysctl>");
      if (! begin) continue;
      char* end = strstr(line, sysctl_end);
      if (! end) continue;

      begin += strlen(sysctl_begin);
      *end = '\0';

      func(begin);
    }
  }

  bool
  makeMapReset(void)
  {
    std::ifstream ifs("/Library/org.pqrs/KeyRemap4MacBook/share/reset");
    if (! ifs) return false;

    while (! ifs.eof()) {
      char line[512];

      ifs.getline(line, sizeof(line));

      char* p = strchr(line, ' ');
      if (! p) continue;
      *p = '\0';

      int value = atoi(p + 1);

      map_reset[line] = value;
    }

    return true;
  }

  bool
  saveToFile(const char** targetFiles, CFStringRef identify)
  {
    if (! isInitialized()) return false;
    if (! makeMapReset()) return false;

    dict_sysctl = CFDictionaryCreateMutable(NULL, 0, NULL, NULL);
    if (! dict_sysctl) return false;

    for (int i = 0;; ++i) {
      const char* filename = targetFiles[i];
      if (! filename) break;
      scanLines(filename, save);
    }
    CFPreferencesSetAppValue(identify, dict_sysctl, applicationID);

    CFRelease(dict_sysctl); dict_sysctl = NULL;
    return true;
  }

  bool
  loadFromFile(const char** targetFiles, CFStringRef identify)
  {
    dict_sysctl = reinterpret_cast<CFMutableDictionaryRef>(const_cast<void*>(CFPreferencesCopyAppValue(identify, applicationID)));
    if (! dict_sysctl) return true;

    for (int i = 0;; ++i) {
      const char* filename = targetFiles[i];
      if (! filename) break;
      scanLines(filename, load);
    }

    CFRelease(dict_sysctl); dict_sysctl = NULL;
    return true;
  }

  // ============================================================
  // ADD & DELETE & SELECT
  void
  setConfigList(CFArrayRef list)
  {
    CFPreferencesSetAppValue(CFSTR("configList"), list, applicationID);
  }

  CFArrayRef
  getConfigList(void)
  {
    CFArrayRef list = reinterpret_cast<CFArrayRef>(CFPreferencesCopyAppValue(CFSTR("configList"), applicationID));

    if (! list || CFArrayGetCount(list) == 0) {
      if (list) CFRelease(list);

      CFMutableDictionaryRef dict[1];
      dict[0] = CFDictionaryCreateMutable(NULL, 0, NULL, NULL);
      CFDictionarySetValue(dict[0], CFSTR("name"), CFSTR("Default"));
      CFDictionarySetValue(dict[0], CFSTR("identify"), CFSTR("config_default"));

      list = CFArrayCreate(NULL, const_cast<const void**>(reinterpret_cast<void**>(dict)), 1, NULL);
      setConfigList(list);
    }

    return list;
  }

  CFDictionaryRef
  getConfigDictionary(CFIndex index)
  {
    CFArrayRef list = getConfigList();
    if (! list) return NULL;

    if (index < 0) return NULL;
    if (index >= CFArrayGetCount(list)) return NULL;

    return reinterpret_cast<CFDictionaryRef>(CFArrayGetValueAtIndex(list, index));
  }

  CFStringRef
  getIdentify(CFIndex index)
  {
    CFDictionaryRef dict = getConfigDictionary(index);
    if (! dict) return NULL;

    return reinterpret_cast<CFStringRef>(CFDictionaryGetValue(dict, CFSTR("identify")));
  }

  CFStringRef
  getConfigName(int index)
  {
    CFDictionaryRef dict = getConfigDictionary(index);
    if (! dict) return NULL;

    return reinterpret_cast<CFStringRef>(CFDictionaryGetValue(dict, CFSTR("name")));
  }

  bool
  appendConfig(void)
  {
    CFArrayRef list = getConfigList();
    if (! list) return false;

    struct timeval tm;
    gettimeofday(&tm, NULL);
    CFStringRef identify = CFStringCreateWithFormat(NULL, NULL, CFSTR("config_%d_%d"), tm.tv_sec, tm.tv_usec);

    CFMutableDictionaryRef dict;
    dict = CFDictionaryCreateMutable(NULL, 0, NULL, NULL);
    CFDictionarySetValue(dict, CFSTR("name"), CFSTR("NewItem"));
    CFDictionarySetValue(dict, CFSTR("identify"), identify);

    CFMutableArrayRef newlist = CFArrayCreateMutableCopy(NULL, 0, list);
    CFArrayAppendValue(newlist, dict);

    setConfigList(newlist);
    return true;
  }

  bool
  removeConfig(int index)
  {
    CFStringRef identify = getIdentify(index);
    if (! identify) return false;

    CFPreferencesSetAppValue(identify, NULL, applicationID);

    CFArrayRef list = getConfigList();
    if (! list) return false;

    CFMutableArrayRef newlist = CFArrayCreateMutableCopy(NULL, 0, list);
    CFArrayRemoveValueAtIndex(newlist, index);

    setConfigList(newlist);
    return true;
  }

  bool
  renameConfig(int index, const char* newname)
  {
    CFArrayRef list = getConfigList();
    if (! list) return false;

    CFDictionaryRef olddict = getConfigDictionary(index);
    if (! olddict) return false;

    CFMutableDictionaryRef newdict = CFDictionaryCreateMutableCopy(NULL, 0, olddict);
    CFDictionarySetValue(newdict, CFSTR("name"), CFStringCreateWithCString(NULL, newname, kCFStringEncodingUTF8));

    CFMutableArrayRef newlist = CFArrayCreateMutableCopy(NULL, 0, list);
    CFArraySetValueAtIndex(newlist, index, newdict);

    setConfigList(newlist);
    return true;
  }

  // ----------------------------------------
  bool
  setSelectedIndex(CFIndex index)
  {
    CFDictionaryRef dict = getConfigDictionary(index);
    if (! dict) return false;

    CFNumberRef val = CFNumberCreate(NULL, kCFNumberIntType, &index);
    CFPreferencesSetAppValue(CFSTR("selectedIndex"), val, applicationID);

    return true;
  }

  CFIndex
  getSelectedIndex(void)
  {
    Boolean isOK;
    CFIndex value = CFPreferencesGetAppIntegerValue(CFSTR("selectedIndex"), applicationID, &isOK);
    if (! isOK || value < 0) {
      value = 0;
      setSelectedIndex(value);
    }
    return value;
  }

  void
  stripString(char* buf, size_t buflen)
  {
    char* p = buf;
    for (;;) {
      if (! isspace(*p)) break;
      ++p;
    }
    char* q = p;
    for (;;) {
      if (*q == '\0' || *q == '\r' || *q == '\n') break;
      ++q;
    }
    *q = '\0';
    snprintf(buf, buflen, "%s", p);
  }

  void
  outputConfigList(void)
  {
    CFArrayRef list = getConfigList();
    if (! list) return;

    CFIndex selected = getSelectedIndex();

    for (int i = 0; i < CFArrayGetCount(list); ++i) {
      CFDictionaryRef d = reinterpret_cast<CFDictionaryRef>(CFArrayGetValueAtIndex(list, i));
      CFStringRef s = reinterpret_cast<CFStringRef>(CFDictionaryGetValue(d, CFSTR("name")));

      std::cout << ((i == selected) ? "+" : "-");

      char buf[512];
      if (CFStringGetCString(s, buf, sizeof(buf), kCFStringEncodingUTF8)) {
        stripString(buf, sizeof(buf));
      } else {
        snprintf(buf, sizeof(buf), "(null)");
      }
      std::cout << buf << std::endl;
    }
  }

  // ----------------------------------------
  bool
  setValue(CFStringRef name, CFIndex enable)
  {
    CFNumberRef val = CFNumberCreate(NULL, kCFNumberIntType, &enable);
    CFPreferencesSetAppValue(name, val, applicationID);
    return true;
  }

  CFIndex
  getValue(CFStringRef name, CFIndex defaultvalue = 1)
  {
    Boolean isOK;
    CFIndex value = CFPreferencesGetAppIntegerValue(name, applicationID, &isOK);
    if (! isOK) {
      value = defaultvalue;
      setValue(name, value);
    }
    return value;
  }
}

int
main(int argc, char** argv)
{
  if (argc == 1) {
    fprintf(stderr, "Usage: %s (save|load|add|delete|rename|select|list|statusbar|toggle_statusbar|statusbar_showname|toggle_statusbar_showname|checkupdate|set_checkupdate) [params]\n", argv[0]);
    return 1;
  }

  bool isSuccess = false;

  if (strcmp(argv[1], "select") == 0) {
    if (argc < 3) {
      fprintf(stderr, "Usage: %s select index\n", argv[0]);
      goto finish;
    }
    int index = atoi(argv[2]);
    isSuccess = setSelectedIndex(index);

  } else if (strcmp(argv[1], "add") == 0) {
    isSuccess = appendConfig();

  } else if (strcmp(argv[1], "delete") == 0) {
    if (argc < 3) {
      fprintf(stderr, "Usage: %s delete index\n", argv[0]);
      goto finish;
    }
    int index = atoi(argv[2]);
    isSuccess = removeConfig(index);

  } else if (strcmp(argv[1], "rename") == 0) {
    // sysctl_ctl rename "index" "newname"
    if (argc < 4) {
      fprintf(stderr, "Usage: %s rename index newname\n", argv[0]);
      goto finish;
    }
    int index = atoi(argv[2]);
    char buf[512];
    snprintf(buf, sizeof(buf), "%s", argv[3]);
    stripString(buf, sizeof(buf));
    if (buf[0] == '\0') {
      snprintf(buf, sizeof(buf), "(null)");
    }
    isSuccess = renameConfig(index, buf);

  } else if (strcmp(argv[1], "list") == 0) {
    outputConfigList();
    return 0;

  } else if (strcmp(argv[1], "statusbar") == 0) {
    printf("%ld\n", getValue(CFSTR("isStatusbarEnable")));
    return 0;

  } else if (strcmp(argv[1], "toggle_statusbar") == 0) {
    CFStringRef name = CFSTR("isStatusbarEnable");
    CFIndex value = getValue(name);
    isSuccess = setValue(name, ! value);

  } else if (strcmp(argv[1], "statusbar_showname") == 0) {
    CFIndex value = getValue(CFSTR("isShowSettingNameInStatusBar"), 0);
    printf("%ld\n", value);
    return 0;

  } else if (strcmp(argv[1], "toggle_statusbar_showname") == 0) {
    CFIndex value = getValue(CFSTR("isShowSettingNameInStatusBar"), 0);
    isSuccess = setValue(name, ! value);

  } else if (strcmp(argv[1], "checkupdate") == 0) {
    printf("%ld\n", getValue(CFSTR("isCheckUpdate")));
    return 0;

  } else if (strcmp(argv[1], "set_checkupdate") == 0) {
    if (argc < 3) {
      fprintf(stderr, "Usage: %s set_checkupdate value\n", argv[0]);
      goto finish;
    }

    CFStringRef name = CFSTR("isCheckUpdate");
    CFIndex value = atoi(argv[2]);
    isSuccess = setValue(name, value);

  } else if ((strcmp(argv[1], "save") == 0) || (strcmp(argv[1], "load") == 0)) {
    CFIndex value = getSelectedIndex();
    CFStringRef identify = getIdentify(value);

    const char* targetFiles[] = {
      "/Library/org.pqrs/KeyRemap4MacBook/prefpane/checkbox.xml",
      "/Library/org.pqrs/KeyRemap4MacBook/prefpane/number.xml",
      NULL,
    };
    if (strcmp(argv[1], "save") == 0) {
      isSuccess = saveToFile(targetFiles, identify);
    }
    if (strcmp(argv[1], "load") == 0) {
      system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_reset");
      isSuccess = loadFromFile(targetFiles, identify);
    }
  }

  CFPreferencesAppSynchronize(applicationID);

finish:
  if (isSuccess) {
    fprintf(stderr, "[DONE]\n");
  } else {
    fprintf(stderr, "[ERROR]\n");
  }

  return 0;
}
