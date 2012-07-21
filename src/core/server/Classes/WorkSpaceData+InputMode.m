#import "WorkSpaceData.h"
#import "XMLCompiler.h"
#import "InputSource.h"

static NSMutableArray* enabledInputSources_ = nil;

@implementation WorkSpaceData (InputMode)

+ (void) refreshEnabledInputSources
{
  @synchronized(self) {
    CFDictionaryRef filter = NULL;
    CFArrayRef list = NULL;

    [enabledInputSources_ release];
    enabledInputSources_ = [NSMutableArray new];

    // ----------------------------------------
    // Making filter
    const void* keys[] = {
      kTISPropertyInputSourceIsSelectCapable,
    };
    const void* values[] = {
      kCFBooleanTrue,
    };
    filter = CFDictionaryCreate(NULL, keys, values, 1, NULL, NULL);
    if (! filter) goto finish;

    // ----------------------------------------
    // Making list
    list = TISCreateInputSourceList(filter, false);
    if (! list) goto finish;

    // ----------------------------------------
    for (int i = 0; i < CFArrayGetCount(list); ++i) {
      TISInputSourceRef source = (TISInputSourceRef)(CFArrayGetValueAtIndex(list, i));
      if (! source) continue;

      // ----------------------------------------
      // Skip if sourceID is follows.
      // - com.apple.PressAndHold
      // - com.apple.CharacterPaletteIM
      // - com.apple.KeyboardViewer
      // - or others which have "com.apple." prefix and
      //      don't have "com.apple.keylayout." prefix.
      NSString* sourceID = TISGetInputSourceProperty(source, kTISPropertyInputSourceID);
      if (sourceID) {
        if ([sourceID hasPrefix:@"com.apple."] &&
            // com.apple.keylayout.French
            ! [sourceID hasPrefix:@"com.apple.keylayout."] &&
            // com.apple.inputmethod.Kotoeri.Roman
            ! [sourceID hasPrefix:@"com.apple.inputmethod."] &&
            // com.apple.keyboardlayout.fr-dvorak-bepo.keylayout.FrenchDvorak
            ! [sourceID hasPrefix:@"com.apple.keyboardlayout."]) {
          continue;
        }
      }

      // ----------------------------------------
      InputSource* inputSource = [[[InputSource alloc] initWithTISInputSourceRef:source] autorelease];
      if (inputSource) {
        [enabledInputSources_ addObject:inputSource];
      }
    }

  finish:
    if (filter) {
      CFRelease(filter);
    }
    if (list) {
      CFRelease(list);
    }
  }
}

+ (InputSource*) getCurrentInputSource
{
  @synchronized(self) {
    for (InputSource* inputSource in enabledInputSources_) {
      if ([inputSource selected]) {
        return [[inputSource retain] autorelease];
      }
    }
  }
  return nil;
}

// ----------------------------------------------------------------------
// Note:
// TISCopyInputSourceForLanguage returns unselectable InputSource.
// Therefore we get InputSource by ourself.
+ (BOOL) selectInputSourceByBcp47:(NSString*)bcp47
{
  if (! bcp47) return NO;

  @synchronized(self) {
    for (InputSource* inputSource in enabledInputSources_) {
      if (! inputSource.bcp47) continue;

      if ([bcp47 isEqualToString:inputSource.bcp47]) {
        [inputSource select];
        return YES;
      }
    }
  }

  return NO;
}

// ----------------------------------------------------------------------
+ (BOOL) selectInputSourceByInputSourceID:(NSString*)inputSourceID
{
  if (! inputSourceID) return NO;

  @synchronized(self) {
    // Note:
    //   InputSourceID is not unique.
    //   For example, AquaSKK has 6 Input Sources.
    //   But all of them have same InputSourceID.
    //
    //   Therefore, use this method carefully.
    //
    //     * kTISPropertyInputSourceID: jp.sourceforge.inputmethod.aquaskk
    //     * kTISPropertyInputModeID:   com.apple.inputmethod.Japanese
    //
    //     * kTISPropertyInputSourceID: jp.sourceforge.inputmethod.aquaskk
    //     * kTISPropertyInputModeID:   com.apple.inputmethod.Japanese.HalfWidthKana
    //
    //     * kTISPropertyInputSourceID: jp.sourceforge.inputmethod.aquaskk
    //     * kTISPropertyInputModeID:   com.apple.inputmethod.Roman
    //
    //     * kTISPropertyInputSourceID: jp.sourceforge.inputmethod.aquaskk
    //     * kTISPropertyInputModeID:   com.apple.inputmethod.Japanese.FullWidthRoman
    //
    //     * kTISPropertyInputSourceID: jp.sourceforge.inputmethod.aquaskk
    //     * kTISPropertyInputModeID:   com.apple.inputmethod.Japanese.Hiragana
    //
    //     * kTISPropertyInputSourceID: jp.sourceforge.inputmethod.aquaskk
    //     * kTISPropertyInputModeID:   com.apple.inputmethod.Japanese.Katakana

    for (InputSource* inputSource in enabledInputSources_) {
      if (! inputSource.inputSourceID) continue;

      if ([inputSourceID isEqualToString:inputSource.inputSourceID]) {
        [inputSource select];
        return YES;
      }
    }
  }

  return NO;
}

// ----------------------------------------------------------------------
+ (void) selectInputSource:(unsigned int)vk_keycode
{
  XMLCompiler* xml_compiler = [XMLCompiler getInstance];

  // ------------------------------------------------------------
  // Select from Language
  {
    NSString* language = nil;

    /*  */ if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTMODE_CANADIAN"]) {
      language = @"ca";
    } else if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTMODE_ENGLISH"]) {
      language = @"en";
    } else if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTMODE_FRENCH"]) {
      language = @"fr";
    } else if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTMODE_GERMAN"]) {
      language = @"de";
    } else if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTMODE_JAPANESE"]) {
      language = @"ja";
    } else if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTMODE_SWEDISH"]) {
      language = @"sv";
    } else if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTMODE_RUSSIAN"]) {
      language = @"ru";
    } else if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTMODE_RUSSIAN_TYPOGRAPHIC"]) {
      language = @"ru-Typographic";
    } else if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTMODE_ENGLISH_TYPOGRAPHIC"]) {
      language = @"en-Typographic";
    } else if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTMODE_TRADITIONAL_CHINESE_YAHOO_KEYKEY"]) {
      language = @"zh-Hant.KeyKey";
    } else if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTMODE_ESTONIAN"]) {
      language = @"et";
    } else if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTMODE_FINNISH"]) {
      language = @"fi";
    } else if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTMODE_THAI"]) {
      language = @"th";
    }

    if (language) {
      [self selectInputSourceByBcp47:language];
    }
  }

  // ------------------------------------------------------------
  // Select from InputSourceID
  {
    NSString* inputSourceID = nil;

    /*  */ if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTSOURCE_DVORAK"]) {
      inputSourceID = @"com.apple.keylayout.Dvorak";
    } else if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTSOURCE_DVORAK_DEVANAGARI_PAUL"]) {
      inputSourceID = @"org.unknown.keylayout.DvorakDevanagariPaul";
    } else if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTSOURCE_DVORAK_TRANSLIT_NEW"]) {
      inputSourceID = @"org.unknown.keylayout.DvorakTranslitNew";
    } else if (vk_keycode == [xml_compiler keycode:@"KeyCode::VK_CHANGE_INPUTSOURCE_COLEMAK"]) {
      inputSourceID = @"com.apple.keylayout.Colemak";
    }

    if (inputSourceID) {
      [self selectInputSourceByInputSourceID:inputSourceID];
    }
  }
}

+ (void) getInputMode:(NSString*)name output_inputmode:(unsigned int*)output_inputmode output_inputmodedetail:(unsigned int*)output_inputmodedetail
{
  if (! name || ! output_inputmode || ! output_inputmodedetail) return;

  // get data from KeyDump.app
  XMLCompiler* xml_compiler = [XMLCompiler getInstance];

  if ([name isEqualToString:@"com.apple.inputmethod.Japanese.Hiragana"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::JAPANESE"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::JAPANESE_HIRAGANA"];
    return;
  }

  if ([name isEqualToString:@"com.apple.inputmethod.Japanese.Katakana"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::JAPANESE"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::JAPANESE_KATAKANA"];
    return;
  }

  if ([name isEqualToString:@"com.apple.inputmethod.Japanese.FullWidthRoman"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::JAPANESE"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::JAPANESE_FULLWIDTH_ROMAN"];
    return;
  }

  if ([name isEqualToString:@"com.apple.inputmethod.Japanese.HalfWidthKana"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::JAPANESE"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::JAPANESE_HALFWIDTH_KANA"];
    return;
  }

  if ([name isEqualToString:@"com.apple.inputmethod.Japanese"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::JAPANESE"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::JAPANESE"];
    return;
  }

  // TradChinese
  if ([name hasPrefix:@"com.apple.inputmethod.TCIM"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::CHINESE_TRADITIONAL"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::CHINESE_TRADITIONAL"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.zh-Hant.KeyKey.KeyKey"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::CHINESE_TRADITIONAL"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::CHINESE_TRADITIONAL_YAHOO_KEYKEY"];
    return;
  }

  // SimpChinese
  if ([name hasPrefix:@"com.apple.inputmethod.SCIM"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::CHINESE_SIMPLIFIED"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::CHINESE_SIMPLIFIED"];
    return;
  }

  if ([name hasPrefix:@"com.apple.inputmethod.Korean"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::KOREAN"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::KOREAN"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.sv."]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::SWEDISH"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::SWEDISH"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.fi."]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::FINNISH"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::FINNISH"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.ca."]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::CANADIAN"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::CANADIAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.ch.SwissFrench"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::SWISS"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::SWISS_FRENCH"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.ch.SwissGerman"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::SWISS"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::SWISS_GERMAN"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.ch."]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::SWISS"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::SWISS"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.hr."]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::CROATIAN"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::CROATIAN"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.hu."]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::HUNGARIAN"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::HUNGARIAN"];
    return;
  }

  if ([name isEqualToString:@"com.apple.kotoeri.Ainu"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::AINU"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::AINU"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.ru."] ||
      [name isEqualToString:@"org.pqrs.inputmode.unknown.RussianWin"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::RUSSIAN"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::RUSSIAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.ru-Typographic.Russian-IlyaBirmanTypography"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::RUSSIAN_TYPOGRAPHIC"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::RUSSIAN_TYPOGRAPHIC"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.en-Typographic.English-IlyaBirmanTypography"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::ENGLISH_TYPOGRAPHIC"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::ENGLISH_TYPOGRAPHIC"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.fr."]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::FRENCH"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::FRENCH"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.pl.Polish"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::POLISH"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::POLISH"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.pl.PolishPro"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::POLISH"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::POLISH_PRO"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.es.Spanish"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::SPANISH"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::SPANISH"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.es.Spanish-ISO"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::SPANISH"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::SPANISH_ISO"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.th.Thai"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::THAI"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::THAI"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.de.German"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::GERMAN"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::GERMAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.sl.Slovenian"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::SLOVENIAN"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::SLOVENIAN"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.cs."]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::CZECH"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::CZECH"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.en.British"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::BRITISH"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::BRITISH"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.bg."]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::BULGARIAN"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::BULGARIAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::BEPO"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::BEPO"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-AzertyCmd"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::BEPO"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::BEPO_AZERTYCMD"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-AzertyCmdRoman"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::BEPO"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::BEPO_AZERTYCMDROMAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-QwertyCmd"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::BEPO"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::BEPO_QWERTYCMD"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-QwertyCmdRoman"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::BEPO"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::BEPO_QWERTYCMDROMAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-QwertzCmd"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::BEPO"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::BEPO_QWERTZCMD"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-QwertzCmdRoman"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::BEPO"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::BEPO_QWERTZCMDROMAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorakRoman"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::BEPO"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::BEPO_ROMAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.en.Dvorak"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::DVORAK"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::DVORAK"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.en.Dvorak-Left"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::DVORAK"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::DVORAK_LEFT"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.en.DVORAK-QWERTYCMD"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::DVORAK"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::DVORAK_QWERTYCMD"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.en.Dvorak-Right"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::DVORAK"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::DVORAK_RIGHT"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.JANSI"]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::ROMAN"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::ROMAN_JANSI"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.unknown."]) {
    *output_inputmode       = [xml_compiler keycode:@"InputMode::UNKNOWN"];
    *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::UNKNOWN"];
    return;
  }

  *output_inputmode       = [xml_compiler keycode:@"InputMode::ROMAN"];
  *output_inputmodedetail = [xml_compiler keycode:@"InputModeDetail::ROMAN"];
}

@end
