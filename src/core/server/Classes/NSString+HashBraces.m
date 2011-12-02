#import "NSString+StringByReplacingOccurrencesOfDictionary.h"

@implementation NSString (HashBraces)

- (NSString*) stringByReplacingHashBracesOccurrencesOfDictionary:(NSDictionary*)replacementDictionary options:(NSStringCompareOptions)options
{
  NSString* string = [NSString stringWithString:self];

  NSRange searchRange = NSMakeRange(0, [string length]);

  // withDictionary is {"#{XXX}" => "111", "#{YYY}" => "222"}.
  // Then replace "#{XXX}" to "111", "#{YYY}" to "222".

  for (;;) {
    // ------------------------------------------------------------
    // Searching "#{"
    NSRange replacementBegin = [string rangeOfString:@"#{" options:options range:searchRange];
    if (replacementBegin.location == NSNotFound) break;

    // Setting length to 0 here becuase we adjust it after replacing.
    searchRange.location = replacementBegin.location + 1;
    searchRange.length = 0;

    // ------------------------------------------------------------
    // Replacing "#{...}"
    NSRange range = NSMakeRange(replacementBegin.location,
                                [string length] - replacementBegin.location);
    for (NSString* target in replacementDictionary) {
      NSRange replacementRange = [string rangeOfString:target
                                               options:(options | NSAnchoredSearch)
                                                 range:range];
      if (replacementRange.location != NSNotFound) {
        string = [string stringByReplacingCharactersInRange:replacementRange
                                                 withString:[replacementDictionary objectForKey:target]];
        searchRange.location = replacementBegin.location;
        break;
      }
    }

    searchRange.length = [string length] - searchRange.location;
  }

  return string;
}

@end
