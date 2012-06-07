#import "InputSource.h"

@implementation InputSource

@synthesize bcp47;
@synthesize inputSourceID;
@synthesize inputModeID;

static NSString* kInputSourceLanguage_canadian                         = @"ca";
static NSString* kInputSourceLanguage_swiss                            = @"ch";
static NSString* kInputSourceLanguage_russian                          = @"ru";

// http://ilyabirman.ru/typography-layout/
static NSString* kInputSourceLanguage_russian_Typographic              = @"ru-Typographic";
static NSString* kInputSourceLanguage_english_Typographic              = @"en-Typographic";
static NSString* kInputSourceLanguage_traditional_chinese_yahoo_keykey = @"zh-Hant.KeyKey";

+ (NSString*) getBcp47:(TISInputSourceRef)source
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
    if ([name hasPrefix:@"com.apple.keylayout.Swiss"]) return kInputSourceLanguage_swiss;
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

- (id) initWithTISInputSourceRef:(TISInputSourceRef)ref
{
  self = [super init];

  if (self) {
    inputSource_ = ref;
    CFRetain(inputSource_);

    bcp47         = [InputSource getBcp47:inputSource_];
    inputSourceID = TISGetInputSourceProperty(inputSource_, kTISPropertyInputSourceID);

    // ----------------------------------------
    // Setting inputModeID
    inputModeID   = TISGetInputSourceProperty(inputSource_, kTISPropertyInputModeID);
    if (! inputModeID) {
      // get detail string
      NSString* detail = @"";

      if (inputSourceID) {
        // Examples:
        //   name == com.apple.keylayout.US
        //   name == com.apple.keylayout.Dvorak
        NSRange dotrange = [inputSourceID rangeOfString:@"." options:NSBackwardsSearch];
        if (dotrange.location != NSNotFound) {
          detail = [inputSourceID substringFromIndex:dotrange.location];
        }
      }

      // ----------------------------------------
      if (bcp47 && [bcp47 length] > 0) {
        inputModeID = [NSString stringWithFormat:@"org.pqrs.inputmode.%@%@", bcp47, detail];
      } else {
        inputModeID = [NSString stringWithFormat:@"org.pqrs.inputmode.unknown%@", detail];
      }
    }
  }

  return self;
}

- (void) dealloc
{
  if (inputSource_) {
    CFRelease(inputSource_);
  }

  [super dealloc];
}

- (void) select
{
  TISSelectInputSource(inputSource_);
}

@end
