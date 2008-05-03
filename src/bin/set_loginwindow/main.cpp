#include <stdio.h>
#include <string.h>

#include <CoreFoundation/CoreFoundation.h>

namespace {
  CFStringRef identify = CFSTR("AutoLaunchedApplicationDictionary");
  CFStringRef applicationID = CFSTR("loginwindow");

  Boolean
  isExist(CFStringRef app, CFArrayRef list)
  {
    // check already registered.
    for (int i = 0; i < CFArrayGetCount(list); ++i) {
      CFDictionaryRef dict = reinterpret_cast<CFDictionaryRef>(CFArrayGetValueAtIndex(list, i));
      CFStringRef path = reinterpret_cast<CFStringRef>(CFDictionaryGetValue(dict, CFSTR("Path")));
      if (CFEqual(path, app)) return TRUE;
    }

    return FALSE;
  }

  Boolean
  do_set(CFStringRef app)
  {
    CFArrayRef list = reinterpret_cast<CFArrayRef>(CFPreferencesCopyAppValue(identify, applicationID));

    if (isExist(app, list)) return TRUE;

    CFMutableArrayRef newlist = CFArrayCreateMutableCopy(NULL, 0, list);
    CFMutableDictionaryRef newdict = CFDictionaryCreateMutable(NULL, 0, NULL, NULL);
    CFDictionarySetValue(newdict, CFSTR("Hide"), kCFBooleanTrue);
    CFDictionarySetValue(newdict, CFSTR("Path"), app);
    CFArrayAppendValue(newlist, newdict);

    CFPreferencesSetAppValue(identify, newlist, applicationID);
    return CFPreferencesAppSynchronize(applicationID);
  }

  Boolean
  do_unset(CFStringRef app)
  {
    CFArrayRef list = reinterpret_cast<CFArrayRef>(CFPreferencesCopyAppValue(identify, applicationID));
    CFMutableArrayRef newlist = CFArrayCreateMutableCopy(NULL, 0, list);

    for (;;) {
      bool found = false;
      for (int i = 0; i < CFArrayGetCount(newlist); ++i) {
        CFDictionaryRef dict = reinterpret_cast<CFDictionaryRef>(CFArrayGetValueAtIndex(newlist, i));
        CFStringRef path = reinterpret_cast<CFStringRef>(CFDictionaryGetValue(dict, CFSTR("Path")));
        if (CFEqual(path, app)) {
          CFArrayRemoveValueAtIndex(newlist, i);
          found = true;
          break;
        }
      }
      if (! found) break;
    }

    CFPreferencesSetAppValue(identify, newlist, applicationID);
    return CFPreferencesAppSynchronize(applicationID);
  }

  Boolean
  do_exist(CFStringRef app)
  {
    CFArrayRef list = reinterpret_cast<CFArrayRef>(CFPreferencesCopyAppValue(identify, applicationID));
    return isExist(app, list);
  }

  void
  usage(char *arg0)
  {
    fprintf(stderr, "Usage: %s (set|unset|exist) application.app\n", arg0);
    exit(1);
  }
}

int
main(int argc, char **argv)
{
  if (argc != 3) {
    usage(argv[0]);
  }

  CFStringRef app = CFStringCreateWithCString(NULL, argv[2], kCFStringEncodingUTF8);

  Boolean isSuccess = false;
  if (strcmp(argv[1], "set") == 0) {
    isSuccess = do_set(app);
  } else if (strcmp(argv[1], "unset") == 0) {
    isSuccess = do_unset(app);
  } else if (strcmp(argv[1], "exist") == 0) {
    isSuccess = do_exist(app);
    printf("%d\n", isSuccess);
    return 0;

  } else {
    usage(argv[0]);
  }

  if (isSuccess) {
    fprintf(stderr, "[DONE] %s %s\n", argv[1], argv[2]);
  } else {
    fprintf(stderr, "[ERROR] %s %s\n", argv[1], argv[2]);
  }

  return 0;
}
