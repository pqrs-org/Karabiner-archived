#import "WorkSpaceData.h"
#import "XMLCompiler.h"
#import "InputSource.h"

static NSMutableArray* enabledInputSources_ = nil;

@implementation WorkSpaceData (InputSource)

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
+ (void) selectInputSource:(unsigned int)vk_keycode
{
  XMLCompiler* xml_compiler = [XMLCompiler getInstance];

  // ----------------------------------------------------------------------
  // Note for languagecode
  // TISCopyInputSourceForLanguage returns unselectable InputSource.
  // Therefore we get InputSource by ourself.

  // Note for inputSourceID
  //   InputSourceID is not unique.
  //   For example, AquaSKK has 6 Input Sources.
  //   But all of them have same InputSourceID.
  //
  //   Therefore, we need to treat it carefully.
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

  InputSource* matched = nil;
  @synchronized(self) {
    for (InputSource* inputSource in enabledInputSources_) {
      if ([xml_compiler is_vk_change_inputsource_matched:vk_keycode
                                            languagecode:inputSource.languagecode
                                           inputSourceID:inputSource.inputSourceID
                                             inputModeID:inputSource.inputModeID]) {
        matched = [[inputSource retain] autorelease];
        break;
      }
    }
  }
  if (matched) {
    [matched select];
  }
}

+ (void) getInputSourceID:(InputSource*)inputSource
 output_inputSource:(uint32_t*)output_inputSource
 output_inputSourceDetail:(uint32_t*)output_inputSourceDetail
{
  if (output_inputSource)       { *output_inputSource = 0; }
  if (output_inputSourceDetail) { *output_inputSourceDetail = 0; }

  XMLCompiler* xml_compiler = [XMLCompiler getInstance];
  [xml_compiler inputsourceid:output_inputSource
            inputSourceDetail:output_inputSourceDetail
                 languagecode:inputSource.languagecode
                inputSourceID:inputSource.inputSourceID
                  inputModeID:inputSource.inputModeID];
}

@end
