#import "InputSource.h"

@implementation InputSource

@synthesize languagecode;
@synthesize inputSourceID;
@synthesize inputModeID;

+ (NSString*) getLanguageCode:(TISInputSourceRef)source
{
  NSArray* languages = TISGetInputSourceProperty(source, kTISPropertyInputSourceLanguages);
  if ([languages count] > 0) {
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
    if (ref) {
      inputSource_ = ref;
      CFRetain(inputSource_);

      languagecode = [[InputSource getLanguageCode:inputSource_] retain];

      inputSourceID = TISGetInputSourceProperty(inputSource_, kTISPropertyInputSourceID);
      [inputSourceID retain];

      inputModeID = TISGetInputSourceProperty(inputSource_, kTISPropertyInputModeID);
      [inputModeID retain];
    }
  }

  return self;
}

- (void) dealloc
{
  [languagecode release];
  [inputSourceID release];
  [inputModeID release];

  if (inputSource_) {
    CFRelease(inputSource_);
  }

  [super dealloc];
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
