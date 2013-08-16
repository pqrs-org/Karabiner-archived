#import "StartAtLoginController.h"

@implementation StartAtLoginController

+ (NSURL*) appURL
{
  return [NSURL fileURLWithPath:[[NSBundle mainBundle] bundlePath]];
}

+ (LSSharedFileListItemRef) getLSSharedFileListItemRef:(LSSharedFileListRef)loginItems
{
  if (! loginItems) return NULL;

  LSSharedFileListItemRef retval = NULL;
  NSURL* appURL = [StartAtLoginController appURL];

  UInt32 seed = 0U;
  NSArray* currentLoginItems = [NSMakeCollectable(LSSharedFileListCopySnapshot(loginItems, &seed))autorelease];
  for (id itemObject in currentLoginItems) {
    LSSharedFileListItemRef item = (LSSharedFileListItemRef)itemObject;

    UInt32 resolutionFlags = kLSSharedFileListNoUserInteraction | kLSSharedFileListDoNotMountVolumes;
    CFURLRef url = NULL;
    OSStatus err = LSSharedFileListItemResolve(item, resolutionFlags, &url, NULL);
    if (err == noErr) {
      BOOL foundIt = CFEqual(url, appURL);
      CFRelease(url);

      if (foundIt) {
        retval = item;
        break;
      }
    }
  }

  return retval;
}

+ (void) enableStartAtLogin
{
  LSSharedFileListRef loginItems = LSSharedFileListCreate(NULL, kLSSharedFileListSessionLoginItems, NULL);
  if (! loginItems) return;

  NSURL* appURL = [StartAtLoginController appURL];
  LSSharedFileListItemRef item = LSSharedFileListInsertItemURL(loginItems, kLSSharedFileListItemLast, NULL, NULL, (CFURLRef)(appURL), NULL, NULL);
  if (item) {
    CFRelease(item);
  }
  CFRelease(loginItems);
}

+ (void) disableStartAtLogin
{
  LSSharedFileListRef loginItems = LSSharedFileListCreate(NULL, kLSSharedFileListSessionLoginItems, NULL);
  if (! loginItems) return;

  LSSharedFileListItemRef item = [StartAtLoginController getLSSharedFileListItemRef:loginItems];
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

  LSSharedFileListItemRef item = [StartAtLoginController getLSSharedFileListItemRef:loginItems];
  CFRelease(loginItems);

  return item != NULL;
}

+ (void) setStartAtLogin:(BOOL)newvalue
{
  if ([StartAtLoginController isStartAtLogin] == newvalue) return;

  if (newvalue) {
    [StartAtLoginController enableStartAtLogin];
  } else {
    [StartAtLoginController disableStartAtLogin];
  }
}

@end
