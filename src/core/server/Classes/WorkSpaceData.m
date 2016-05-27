#import "WorkSpaceData.h"
#import "InputSource.h"
#import "XMLCompiler.h"
#import "weakify.h"

@interface WorkSpaceData ()

@property(weak) IBOutlet XMLCompiler* xmlCompiler;

@end

@implementation WorkSpaceData

- (unsigned int)getUIElementRole:(NSString*)name {
  if (!name) return 0;

  return [self.xmlCompiler uielementroleid:name];
}

// InputSource

static dispatch_queue_t enabledInputSourcesQueue_;
static NSMutableArray* enabledInputSources_;

+ (void)initialize {
  static dispatch_once_t once;
  dispatch_once(&once, ^{
    enabledInputSourcesQueue_ = dispatch_queue_create("org.pqrs.Karabiner.WorkSpaceData.enabledInputSourcesQueue_", NULL);
    enabledInputSources_ = nil;
  });
}

+ (void)refreshEnabledInputSources {
  dispatch_sync(enabledInputSourcesQueue_, ^{
    CFDictionaryRef filter = NULL;
    CFArrayRef list = NULL;

    enabledInputSources_ = [NSMutableArray new];

    // ----------------------------------------
    // Making filter
    const void* keys[] = {
        kTISPropertyInputSourceIsSelectCapable,
        kTISPropertyInputSourceCategory,
    };
    const void* values[] = {
        kCFBooleanTrue,
        kTISCategoryKeyboardInputSource,
    };
    filter = CFDictionaryCreate(NULL, keys, values, sizeof(keys) / sizeof(keys[0]), NULL, NULL);
    if (!filter) goto finish;

    // ----------------------------------------
    // Making list
    list = TISCreateInputSourceList(filter, false);
    if (!list) goto finish;

    // ----------------------------------------
    for (int i = 0; i < CFArrayGetCount(list); ++i) {
      TISInputSourceRef source = (TISInputSourceRef)(CFArrayGetValueAtIndex(list, i));
      if (!source) continue;

      // ----------------------------------------
      // Skip inappropriate input sources.
      //
      // - com.apple.inputmethod.ironwood (Voice Input)

      NSString* sourceID = (__bridge NSString*)(TISGetInputSourceProperty(source, kTISPropertyInputSourceID));
      if (sourceID) {
        if ([sourceID isEqualToString:@"com.apple.inputmethod.ironwood"]) {
          continue;
        }
      }

      // ----------------------------------------
      InputSource* inputSource = [[InputSource alloc] initWithTISInputSourceRef:source];
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
  });
}

+ (InputSource*)getCurrentInputSource {
  __block InputSource* result = nil;

  dispatch_sync(enabledInputSourcesQueue_, ^{
    for (InputSource* inputSource in enabledInputSources_) {
      if ([inputSource selected]) {
        result = inputSource;
      }
    }
  });

  return result;
}

// ----------------------------------------------------------------------
- (void)selectInputSource:(unsigned int)vk_keycode {
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

  @weakify(self);

  dispatch_sync(enabledInputSourcesQueue_, ^{
    @strongify(self);
    if (!self) return;

    for (InputSource* inputSource in enabledInputSources_) {
      if ([self.xmlCompiler is_vk_change_inputsource_matched:vk_keycode
                                                languagecode:inputSource.languagecode
                                               inputSourceID:inputSource.inputSourceID
                                                 inputModeID:inputSource.inputModeID]) {
        [inputSource select];
        break;
      }
    }
  });
}

@end
