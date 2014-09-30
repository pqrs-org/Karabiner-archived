#import "StartAtLoginUtilities.h"

@implementation StartAtLoginUtilities

+ (NSURL*)appURL {
  return [NSURL fileURLWithPath:[[NSBundle mainBundle] bundlePath]];
}

+ (LSSharedFileListItemRef)getLSSharedFileListItemRef:(LSSharedFileListRef)loginItems appURL:(NSURL*)appURL {
  if (!loginItems) return NULL;

  LSSharedFileListItemRef retval = NULL;

  UInt32 seed = 0U;
  CFArrayRef currentLoginItemsRef = LSSharedFileListCopySnapshot(loginItems, &seed);
  NSArray* currentLoginItems = CFBridgingRelease(currentLoginItemsRef);
  for (id itemObject in currentLoginItems) {
    LSSharedFileListItemRef item = (__bridge LSSharedFileListItemRef)itemObject;

    UInt32 resolutionFlags = kLSSharedFileListNoUserInteraction | kLSSharedFileListDoNotMountVolumes;
    CFURLRef urlRef = NULL;
    OSStatus err = LSSharedFileListItemResolve(item, resolutionFlags, &urlRef, NULL);
    if (err == noErr) {
      NSURL* url = CFBridgingRelease(urlRef);
      BOOL foundIt = [url isEqual:appURL];

      if (foundIt) {
        retval = item;
        break;
      }
    }
  }

  if (retval) {
    CFRetain(retval);
  }

  return retval;
}

+ (void)enableStartAtLogin:(NSURL*)appURL {
  LSSharedFileListRef loginItems = LSSharedFileListCreate(NULL, kLSSharedFileListSessionLoginItems, NULL);
  if (!loginItems) return;

  LSSharedFileListItemRef item = LSSharedFileListInsertItemURL(loginItems, kLSSharedFileListItemLast, NULL, NULL, (__bridge CFURLRef)(appURL), NULL, NULL);
  if (item) {
    CFRelease(item);
  }
  CFRelease(loginItems);
}

+ (void)disableStartAtLogin:(NSURL*)appURL {
  LSSharedFileListRef loginItems = LSSharedFileListCreate(NULL, kLSSharedFileListSessionLoginItems, NULL);
  if (!loginItems) return;

  LSSharedFileListItemRef item = [StartAtLoginUtilities getLSSharedFileListItemRef:loginItems appURL:appURL];
  if (item) {
    LSSharedFileListItemRemove(loginItems, item);
  }
  CFRelease(loginItems);
}

// ------------------------------------------------------------
+ (BOOL)isStartAtLogin:(NSURL*)appURL {
  LSSharedFileListRef loginItems = LSSharedFileListCreate(NULL, kLSSharedFileListSessionLoginItems, NULL);
  if (!loginItems) return NO;

  LSSharedFileListItemRef item = [StartAtLoginUtilities getLSSharedFileListItemRef:loginItems appURL:appURL];
  CFRelease(loginItems);

  return item != NULL;
}

+ (BOOL)isStartAtLogin {
  return [StartAtLoginUtilities isStartAtLogin:[StartAtLoginUtilities appURL]];
}

+ (void)setStartAtLogin:(BOOL)newvalue appURL:(NSURL*)appURL {
  if ([StartAtLoginUtilities isStartAtLogin:appURL] == newvalue) return;

  if (newvalue) {
    [StartAtLoginUtilities enableStartAtLogin:appURL];
  } else {
    [StartAtLoginUtilities disableStartAtLogin:appURL];
  }
}

+ (void)setStartAtLogin:(BOOL)newvalue {
  [StartAtLoginUtilities setStartAtLogin:newvalue appURL:[StartAtLoginUtilities appURL]];
}

@end
