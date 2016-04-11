#import "InputSource.h"

@interface InputSource ()

@property TISInputSourceRef inputSource;
@property(copy, readwrite) NSString* languagecode;
@property(copy, readwrite) NSString* inputSourceID;
@property(copy, readwrite) NSString* inputModeID;

@end

@implementation InputSource

+ (NSString*)getLanguageCode:(TISInputSourceRef)source {
  NSArray* languages = (__bridge NSArray*)(TISGetInputSourceProperty(source, kTISPropertyInputSourceLanguages));
  if ([languages count] > 0) {
    // U.S. InputSource has many languages (en, de, fr, ...),
    // so we check the first language only to detect real InputSource for French, German, etc.
    NSString* lang = languages[0];
    if (lang) return lang;
  }

  return nil;
}

- (instancetype)initWithTISInputSourceRef:(TISInputSourceRef)ref {
  self = [super init];

  if (self) {
    if (ref) {
      self.inputSource = ref;
      CFRetain(self.inputSource);

      self.languagecode = [InputSource getLanguageCode:self.inputSource];
      self.inputSourceID = (__bridge NSString*)(TISGetInputSourceProperty(self.inputSource, kTISPropertyInputSourceID));
      self.inputModeID = (__bridge NSString*)(TISGetInputSourceProperty(self.inputSource, kTISPropertyInputModeID));
    }
  }

  return self;
}

- (void)dealloc {
  if (self.inputSource) {
    CFRelease(self.inputSource);
  }
}

- (void)select {
  if (self.inputSource) {
    TISSelectInputSource(self.inputSource);
  }
}

- (Boolean)selected {
  if (!self.inputSource) {
    return NO;
  }

  CFBooleanRef selected = TISGetInputSourceProperty(self.inputSource, kTISPropertyInputSourceIsSelected);
  if (!selected) {
    return NO;
  }

  return CFBooleanGetValue(selected);
}

@end
