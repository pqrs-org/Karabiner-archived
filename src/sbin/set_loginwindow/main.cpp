#include <stdio.h>
#include <string.h>

#include <CoreFoundation/CoreFoundation.h>

namespace {
  CFStringRef identify = CFSTR("AutoLaunchedApplicationDictionary");
  CFStringRef applicationID = CFSTR("loginwindow");

  Boolean
  set(CFStringRef app)
  {
    CFArrayRef list = reinterpret_cast<CFArrayRef>(CFPreferencesCopyAppValue(identify, applicationID));

    // check already registered.
    for (int i = 0; i < CFArrayGetCount(list); ++i) {
      CFDictionaryRef dict = reinterpret_cast<CFDictionaryRef>(CFArrayGetValueAtIndex(list, i));
      CFStringRef path = reinterpret_cast<CFStringRef>(CFDictionaryGetValue(dict, CFSTR("Path")));
      if (CFEqual(path, app)) return TRUE;
    }

    CFMutableArrayRef newlist = CFArrayCreateMutableCopy(NULL, 0, list);
    CFMutableDictionaryRef newdict = CFDictionaryCreateMutable(NULL, 0, NULL, NULL);
    CFDictionarySetValue(newdict, CFSTR("Hide"), kCFBooleanFalse);
    CFDictionarySetValue(newdict, CFSTR("Path"), app);
    CFArrayAppendValue(newlist, newdict);

    CFPreferencesSetAppValue(identify, newlist, applicationID);
    return CFPreferencesAppSynchronize(applicationID);
  }

  Boolean
  unset(CFStringRef app)
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

  void
  usage(char *arg0)
  {
    fprintf(stderr, "Usage: %s (set|unset) application.app\n", arg0);
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

  if (strcmp(argv[1], "set") == 0) {
    if (set(app)) {
      fprintf(stderr, "[DONE]");
    } else {
      fprintf(stderr, "[ERROR]");
    }
  } else if (strcmp(argv[1], "unset") == 0) {
    if (unset(app)) {
      fprintf(stderr, "[DONE]");
    } else {
      fprintf(stderr, "[ERROR]");
    }
  } else {
    usage(argv[0]);
  }

  return 0;
}
