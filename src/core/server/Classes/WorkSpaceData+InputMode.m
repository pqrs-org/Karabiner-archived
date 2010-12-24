#import "WorkSpaceData.h"
#import <Carbon/Carbon.h>

@implementation WorkSpaceData (InputMode)

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

- (void) getInputMode:(NSString*)name output_inputmode:(unsigned int*)output_inputmode output_inputmodedetail:(unsigned int*)output_inputmodedetail
{
  if (! name || ! output_inputmode || ! output_inputmodedetail) return;

  // get data from KeyDump.app

  if ([name isEqualToString:@"com.apple.inputmethod.Japanese.Hiragana"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::JAPANESE"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::JAPANESE_HIRAGANA"];
    return;
  }

  if ([name isEqualToString:@"com.apple.inputmethod.Japanese.Katakana"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::JAPANESE"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::JAPANESE_KATAKANA"];
    return;
  }

  if ([name isEqualToString:@"com.apple.inputmethod.Japanese.FullWidthRoman"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::JAPANESE"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::JAPANESE_FULLWIDTH_ROMAN"];
    return;
  }

  if ([name isEqualToString:@"com.apple.inputmethod.Japanese.HalfWidthKana"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::JAPANESE"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::JAPANESE_HALFWIDTH_KANA"];
    return;
  }

  if ([name isEqualToString:@"com.apple.inputmethod.Japanese"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::JAPANESE"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::JAPANESE"];
    return;
  }

  // TradChinese
  if ([name hasPrefix:@"com.apple.inputmethod.TCIM"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::CHINESE_TRADITIONAL"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::CHINESE_TRADITIONAL"];
    return;
  }

  // SimpChinese
  if ([name hasPrefix:@"com.apple.inputmethod.SCIM"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::CHINESE_SIMPLIFIED"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::CHINESE_SIMPLIFIED"];
    return;
  }

  if ([name hasPrefix:@"com.apple.inputmethod.Korean"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::KOREAN"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::KOREAN"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.sv."]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::SWEDISH"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::SWEDISH"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.ca."]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::CANADIAN"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::CANADIAN"];
    return;
  }

  if ([name isEqualToString:@"com.apple.kotoeri.Ainu"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::AINU"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::AINU"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.ru."] ||
      [name isEqualToString:@"org.pqrs.inputmode.unknown.RussianWin"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::RUSSIAN"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::RUSSIAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.Russian-IlyaBirmanTypography"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::RUSSIAN_TYPOGRAPHIC"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::RUSSIAN_TYPOGRAPHIC"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.English-IlyaBirmanTypography"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::ENGLISH_TYPOGRAPHIC"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::ENGLISH_TYPOGRAPHIC"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.fr."]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::FRENCH"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::FRENCH"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::BEPO"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::BEPO"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-AzertyCmd"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::BEPO"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::BEPO_AZERTYCMD"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-AzertyCmdRoman"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::BEPO"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::BEPO_AZERTYCMDROMAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-QwertyCmd"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::BEPO"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::BEPO_QWERTYCMD"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-QwertyCmdRoman"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::BEPO"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::BEPO_QWERTYCMDROMAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-QwertzCmd"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::BEPO"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::BEPO_QWERTZCMD"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-QwertzCmdRoman"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::BEPO"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::BEPO_QWERTZCMDROMAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorakRoman"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::BEPO"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::BEPO_ROMAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.en.Dvorak"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::DVORAK"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::DVORAK"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.en.Dvorak-Left"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::DVORAK"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::DVORAK_LEFT"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.en.DVORAK-QWERTYCMD"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::DVORAK"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::DVORAK_QWERTYCMD"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.en.Dvorak-Right"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::DVORAK"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::DVORAK_RIGHT"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.JANSI"]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::ROMAN"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::ROMAN_JANSI"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.unknown."]) {
    *output_inputmode       = [configxmlparser_ keycode:@"InputMode::UNKNOWN"];
    *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::UNKNOWN"];
    return;
  }

  *output_inputmode       = [configxmlparser_ keycode:@"InputMode::ROMAN"];
  *output_inputmodedetail = [configxmlparser_ keycode:@"InputModeDetail::ROMAN"];
}

@end
