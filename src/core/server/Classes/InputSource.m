#import "InputSource.h"

@interface InputSource ()
{
  TISInputSourceRef inputSource_;
}
@end

@implementation InputSource

@synthesize languagecode = languagecode_;
@synthesize inputSourceID = inputSourceID_;
@synthesize inputModeID = inputModeID_;

+ (NSString*) getLanguageCode:(TISInputSourceRef)source
{
  NSArray* languages = (__bridge NSArray*)(TISGetInputSourceProperty(source, kTISPropertyInputSourceLanguages));
  if ([languages count] > 0) {
    // U.S. InputSource has many languages (en, de, fr, ...),
    // so we check the first language only to detect real InputSource for French, German, etc.
    NSString* lang = languages[0];
    if (lang) return lang;
  }

  return nil;
}

- (id) initWithTISInputSourceRef:(TISInputSourceRef)ref
{
  self = [super init];

  if (self) {
    if (ref) {
      inputSource_ = ref;
      CFRetain(inputSource_);

      languagecode_  = [InputSource getLanguageCode:inputSource_];
      inputSourceID_ = (__bridge NSString*)(TISGetInputSourceProperty(inputSource_, kTISPropertyInputSourceID));
      inputModeID_   = (__bridge NSString*)(TISGetInputSourceProperty(inputSource_, kTISPropertyInputModeID));
    }
  }

  return self;
}

- (void) dealloc
{
  if (inputSource_) {
    CFRelease(inputSource_);
  }
}

- (void) select
{
  if (inputSource_) {
    TISSelectInputSource(inputSource_);
  }
}

- (Boolean) selected
{
  if (! inputSource_) {
    return NO;
  }

  CFBooleanRef selected = TISGetInputSourceProperty(inputSource_, kTISPropertyInputSourceIsSelected);
  if (! selected) {
    return NO;
  }

  return CFBooleanGetValue(selected);
}

@end
