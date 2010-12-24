#include <sys/types.h>
#include <stdio.h>
#import <Carbon/Carbon.h>
#import "server_objc_part.h"

@implementation ServerObjcPart

- (NSArray*) getEssentialConfig
{
  return [preferencesmanager_ essential_config];
}

- (NSUInteger) getConfigCount
{
  return [configxmlparser_ count];
}

- (NSUInteger) getConfigInitializeVectorSize:(unsigned int)configindex
{
  return [configxmlparser_ initialize_vector_size:configindex];
}

- (NSArray*) getConfigInitializeVector:(unsigned int)configindex
{
  return [configxmlparser_ initialize_vector:configindex];
}

- (int) getConfigValue:(unsigned int)configindex
{
  NSString* name = [configxmlparser_ configname:configindex];
  if (! name) return 0;

  return [preferencesmanager_ value:name];
}

- (NSString*) getActiveApplicationName
{
  // ----------------------------------------
  NSWorkspace* ws = [NSWorkspace sharedWorkspace];
  if (! ws) return nil;

  NSArray* a = [ws runningApplications];
  if (! a) return nil;

  for (NSRunningApplication* app in a) {
    if (! app) return nil;

    if ([app isActive]) {
      NSString* nsappname = [app bundleIdentifier];

      if (nsappname) {
        return [NSString stringWithString:nsappname];

      } else {
        // We use localizedName instead of bundleIdentifier,
        // because "MacSOUP" doesn't have bundleIdentifier.
        // http://www.haller-berlin.de/macsoup/index.html
        NSString* localizedName = [app localizedName];
        if (localizedName) {
          return [NSString stringWithFormat:@"org.pqrs.unknownapp.%@", localizedName];
        }
      }
    }
  }

  return nil;
}

// ======================================================================
static NSString* kInputSourceLanguage_canadian                         = @"ca";
static NSString* kInputSourceLanguage_russian                          = @"ru";
// http://ilyabirman.ru/typography-layout/
static NSString* kInputSourceLanguage_russian_Typographic              = @"ru-Typographic";
static NSString* kInputSourceLanguage_english_Typographic              = @"en-Typographic";
static NSString* kInputSourceLanguage_traditional_chinese_yahoo_keykey = @"zh-Hant.KeyKey";

- (NSString*) getInputSourceLanguage:(TISInputSourceRef)source
{
  // Because we cannot distinguish en and ca from kTISPropertyInputSourceLanguages,
  // we use kTISPropertyInputSourceID at first.
  //
  // Note:
  // kTISPropertyInputSourceID is different every IM, and
  // it is desirable as possible to use kTISPropertyInputSourceLanguages because
  // kTISPropertyInputSourceID does not get the correspondence with the input language.
  //
  // Example:
  //   kTISPropertyInputSourceID: jp.sourceforge.inputmethod.aquaskk
  //   kTISPropertyInputSourceLanguages: ja
  //
  //   kTISPropertyInputSourceID: com.apple.inputmethod.Kotoeri.Japanese
  //   kTISPropertyInputSourceLanguages: ja
  //
  //   * These two IM are Japanese input method.

  NSString* name = TISGetInputSourceProperty(source, kTISPropertyInputSourceID);
  if (name) {
    if ([name isEqualToString:@"com.apple.keylayout.Canadian"]) return kInputSourceLanguage_canadian;
    if ([name isEqualToString:@"org.unknown.keylayout.RussianWin"]) return kInputSourceLanguage_russian;
    if ([name isEqualToString:@"org.unknown.keylayout.Russian-IlyaBirmanTypography"]) return kInputSourceLanguage_russian_Typographic;
    if ([name isEqualToString:@"org.unknown.keylayout.English-IlyaBirmanTypography"]) return kInputSourceLanguage_english_Typographic;
    if ([name isEqualToString:@"com.yahoo.inputmethod.KeyKey"]) return kInputSourceLanguage_traditional_chinese_yahoo_keykey;
  }

  NSArray* languages = TISGetInputSourceProperty(source, kTISPropertyInputSourceLanguages);
  if (languages && [languages count] > 0) {
    // U.S. InputSource has many languages (en, de, fr, ...),
    // so we check the first language only to detect real InputSource for French, German, etc.
    NSString* lang = [languages objectAtIndex:0];
    if (lang) return lang;
  }

  return nil;
}

- (NSString*) getTISPropertyInputModeID
{
  TISInputSourceRef ref = TISCopyCurrentKeyboardInputSource();
  if (! ref) return nil;

  NSString* retval = nil;
  NSString* inputmodeid = TISGetInputSourceProperty(ref, kTISPropertyInputModeID);
  if (inputmodeid) {
    retval = [NSString stringWithString:inputmodeid];
  } else {
    // ----------------------------------------
    // get detail string
    NSString* detail = @"";

    NSString* name = TISGetInputSourceProperty(ref, kTISPropertyInputSourceID);
    if (name) {
      // Examples:
      //   name == com.apple.keylayout.US
      //   name == com.apple.keylayout.Dvorak
      NSRange dotrange = [name rangeOfString:@"." options:NSBackwardsSearch];
      if (dotrange.location != NSNotFound) {
        detail = [name substringFromIndex:dotrange.location];
      }
    }

    // ----------------------------------------
    NSString* lang = [self getInputSourceLanguage:ref];
    if (lang && [lang length] > 0) {
      retval = [NSString stringWithFormat:@"org.pqrs.inputmode.%@%@", lang, detail];
    } else {
      retval = [NSString stringWithFormat:@"org.pqrs.inputmode.unknown%@", detail];
    }
  }

  CFRelease(ref);
  return retval;
}

// ----------------------------------------------------------------------
// Note:
// TISCopyInputSourceForLanguage returns unselectable InputSource.
// Therefore we get InputSource by ourself.
- (TISInputSourceRef) copySelectableInputSourceForLanguage:(NSString*)language
{
  TISInputSourceRef inputsource = NULL;
  CFDictionaryRef filter = NULL;
  CFArrayRef list = NULL;

  if (! language) goto finish;

  // ------------------------------------------------------------
  const void* keys[] = {
    kTISPropertyInputSourceIsSelectCapable,
  };
  const void* values[] = {
    kCFBooleanTrue,
  };

  filter = CFDictionaryCreate(NULL, keys, values, 1, NULL, NULL);
  if (! filter) goto finish;

  list = TISCreateInputSourceList(filter, false);
  if (! list) goto finish;

  for (int i = 0; i < CFArrayGetCount(list); ++i) {
    TISInputSourceRef source = (TISInputSourceRef)(CFArrayGetValueAtIndex(list, i));
    if (! source) continue;

    NSString* lang = [self getInputSourceLanguage:source];
    if (! lang) continue;

    if ([language isEqualToString:lang]) {
      inputsource = source;
      CFRetain(inputsource);
      goto finish;
    }
  }

finish:
  if (filter) {
    CFRelease(filter);
  }
  if (list) {
    CFRelease(list);
  }
  return inputsource;
}

// ----------------------------------------------------------------------
- (void) selectInputSource:(unsigned int)vk_keycode
{
  NSString* language = nil;

  /*  */ if (vk_keycode == [configxmlparser_ keycode:@"KeyCode::VK_CHANGE_INPUTMODE_CANADIAN"]) {
    language = kInputSourceLanguage_canadian;
  } else if (vk_keycode == [configxmlparser_ keycode:@"KeyCode::VK_CHANGE_INPUTMODE_ENGLISH"]) {
    language = @"en";
  } else if (vk_keycode == [configxmlparser_ keycode:@"KeyCode::VK_CHANGE_INPUTMODE_FRENCH"]) {
    language = @"fr";
  } else if (vk_keycode == [configxmlparser_ keycode:@"KeyCode::VK_CHANGE_INPUTMODE_GERMAN"]) {
    language = @"de";
  } else if (vk_keycode == [configxmlparser_ keycode:@"KeyCode::VK_CHANGE_INPUTMODE_JAPANESE"]) {
    language = @"ja";
  } else if (vk_keycode == [configxmlparser_ keycode:@"KeyCode::VK_CHANGE_INPUTMODE_SWEDISH"]) {
    language = @"sv";
  } else if (vk_keycode == [configxmlparser_ keycode:@"KeyCode::VK_CHANGE_INPUTMODE_RUSSIAN"]) {
    language = kInputSourceLanguage_russian;
  } else if (vk_keycode == [configxmlparser_ keycode:@"KeyCode::VK_CHANGE_INPUTMODE_RUSSIAN_TYPOGRAPHIC"]) {
    language = kInputSourceLanguage_russian_Typographic;
  } else if (vk_keycode == [configxmlparser_ keycode:@"KeyCode::VK_CHANGE_INPUTMODE_ENGLISH_TYPOGRAPHIC"]) {
    language = kInputSourceLanguage_english_Typographic;
  } else if (vk_keycode == [configxmlparser_ keycode:@"KeyCode::VK_CHANGE_INPUTMODE_TRADITIONAL_CHINESE_YAHOO_KEYKEY"]) {
    language = kInputSourceLanguage_traditional_chinese_yahoo_keykey;
  } else if (vk_keycode == [configxmlparser_ keycode:@"KeyCode::VK_CHANGE_INPUTMODE_ESTONIAN"]) {
    language = @"et";
  }

  if (! language) return;

  // ----------------------------------------
  TISInputSourceRef inputsource = [self copySelectableInputSourceForLanguage:language];
  if (! inputsource) return;

  TISSelectInputSource(inputsource);
  CFRelease(inputsource);
}

// ----------------------------------------------------------------------
- (void) setStatusMessage:(StatusMessageType)type message:(const char*)message
{
  NSString* s = [NSString stringWithCString:message encoding:NSUTF8StringEncoding];
  [statuswindow_ setStatusMessage:type message:s];
}

@end

// ======================================================================
static ServerObjcPart * serverobjcpart = nil;

void
registerServerObjcPart(ServerObjcPart* object)
{
  serverobjcpart = object;
}

// ------------------------------------------------------------
void
selectInputSource(uint32_t vk_keycode)
{
  if (! serverobjcpart) {
    NSLog(@"[WARNING] selectInputSource_language serverobjcpart == nil");
    return;
  }

  [serverobjcpart selectInputSource:vk_keycode];
}

// ------------------------------------------------------------
int
getEssentialConfig(int32_t* value, size_t len)
{
  if (! serverobjcpart) {
    NSLog(@"[WARNING] getEssentialConfig serverobjcpart == nil");
    return 1;
  }

  NSArray* essential_config = [serverobjcpart getEssentialConfig];
  if (! essential_config) {
    NSLog(@"[WARNING] getEssentialConfig essential_config == nil");
    return 1;
  }

  size_t i = 0;
  for (NSNumber* number in essential_config) {
    if (i < len) {
      value[i] = [number intValue];
      ++i;
    }
  }

  return 0;
}

uint32_t
getConfigCount(void)
{
  if (serverobjcpart) {
    return (uint32_t)([serverobjcpart getConfigCount]);
  } else {
    NSLog(@"[WARNING] getConfigCount serverobjcpart == nil");
    return 0;
  }
}

uint32_t
getConfigInitializeVectorSize(uint32_t configindex)
{
  if (serverobjcpart) {
    return (uint32_t)([serverobjcpart getConfigInitializeVectorSize:configindex]);
  } else {
    NSLog(@"[WARNING] getConfigInitializeVectorSize serverobjcpart == nil");
    return 0;
  }
}

int
getConfigInitializeVector(uint32_t* value, size_t len, uint32_t configindex)
{
  if (! serverobjcpart) {
    NSLog(@"[WARNING] getConfigInitializeVector serverobjcpart == nil");
    return 1;
  }

  NSArray* initialize_vector = [serverobjcpart getConfigInitializeVector:configindex];
  if (! initialize_vector) {
    NSLog(@"[WARNING] getConfigInitializeVector initialize_vector == nil");
    return 1;
  }

  size_t i = 0;
  for (NSNumber* number in initialize_vector) {
    if (i < len) {
      value[i] = [number unsignedIntValue];
      ++i;
    }
  }

  return 0;
}

int
getConfigValue(uint32_t configindex)
{
  if (serverobjcpart) {
    return (uint32_t)([serverobjcpart getConfigValue:configindex]);
  } else {
    NSLog(@"[WARNING] getConfigValue serverobjcpart == nil");
    return 0;
  }
}

void
set_statusmessage(StatusMessageType type, const char* message)
{
  if (serverobjcpart) {
    [serverobjcpart setStatusMessage:type message:message];
  } else {
    NSLog(@"[WARNING] set_statusmessage serverobjcpart == nil");
  }
}
