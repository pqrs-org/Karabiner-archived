#import "StartAtLoginUtilities.h"

@implementation StartAtLoginUtilities

+ (NSURL*) appURL
{
  return [NSURL fileURLWithPath:[[NSBundle mainBundle] bundlePath]];
}

+ (LSSharedFileListItemRef) getLSSharedFileListItemRef:(LSSharedFileListRef)loginItems
{
  if (! loginItems) return NULL;

  LSSharedFileListItemRef retval = NULL;
  NSURL* appURL = [StartAtLoginUtilities appURL];

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

+ (void) enableStartAtLogin
{
  LSSharedFileListRef loginItems = LSSharedFileListCreate(NULL, kLSSharedFileListSessionLoginItems, NULL);
  if (! loginItems) return;

  NSURL* appURL = [StartAtLoginUtilities appURL];
  LSSharedFileListItemRef item = LSSharedFileListInsertItemURL(loginItems, kLSSharedFileListItemLast, NULL, NULL, (__bridge CFURLRef)(appURL), NULL, NULL);
  if (item) {
    CFRelease(item);
  }
  CFRelease(loginItems);
}

+ (void) disableStartAtLogin
{
  LSSharedFileListRef loginItems = LSSharedFileListCreate(NULL, kLSSharedFileListSessionLoginItems, NULL);
  if (! loginItems) return;

  LSSharedFileListItemRef item = [StartAtLoginUtilities getLSSharedFileListItemRef:loginItems];
  if (item) {
    LSSharedFileListItemRemove(loginItems, item);
  }
  CFRelease(loginItems);
}

// ------------------------------------------------------------
+ (BOOL) isStartAtLogin
{
  LSSharedFileListRef loginItems = LSSharedFileListCreate(NULL, kLSSharedFileListSessionLoginItems, NULL);
  if (! loginItems) return NO;

  LSSharedFileListItemRef item = [StartAtLoginUtilities getLSSharedFileListItemRef:loginItems];
  CFRelease(loginItems);

  return item != NULL;
}

+ (void) setStartAtLogin:(BOOL)newvalue
{
  if ([StartAtLoginUtilities isStartAtLogin] == newvalue) return;

  if (newvalue) {
    [StartAtLoginUtilities enableStartAtLogin];
  } else {
    [StartAtLoginUtilities disableStartAtLogin];
  }
}

@end
