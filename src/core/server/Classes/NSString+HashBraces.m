#import "NSString+HashBraces.h"

@implementation NSString (HashBraces)

- (NSString*) stringByReplacingHashBracesOccurrencesOfDictionary:(NSDictionary*)replacementDictionary
{
  // withDictionary is {"#{XXX}" => "111", "#{YYY}" => "222"}.
  // Then replace "#{XXX}" to "111", "#{YYY}" to "222".

  // A simple way:
  //   Calling stringByReplacingOccurrencesOfString to each item in replacementDictionary.
  //
  // Above method is too slow if target string (self) is too big and replacementDictionary has many items.
  // (Because full scan of target string happens many times (replacementDictionary keys size).)
  //
  // So, we scan "#{" from target string and replacing them.
  // This way needs full scan of target string only once.
  // It reduces processing time dramatically if target string is too big.

  NSString* string = [NSString stringWithString:self];

  NSRange searchRange = NSMakeRange(0, [string length]);

  for (;;) {
    // ------------------------------------------------------------
    // Searching "#{"
    NSRange replacementBegin = [string rangeOfString:@"#{" options:NSLiteralSearch range:searchRange];
    if (replacementBegin.location == NSNotFound) break;

    // Setting length to 0 here becuase we adjust it after replacing.
    searchRange.location = replacementBegin.location + 1;
    searchRange.length = 0;

    // ------------------------------------------------------------
    // Replacing "#{...}"
    NSRange range = NSMakeRange(replacementBegin.location,
                                [string length] - replacementBegin.location);
    for (NSString* replacementTarget in replacementDictionary) {
      NSRange replacementRange = [string rangeOfString:replacementTarget
                                               options:(NSLiteralSearch | NSAnchoredSearch)
                                                 range:range];
      if (replacementRange.location != NSNotFound) {
        NSString* replacementString = [replacementDictionary objectForKey:replacementTarget];

        string = [string stringByReplacingCharactersInRange:replacementRange
                                                 withString:replacementString];

        // Setting next searchRange.location to replacementBegin.location for when replacementString is empty.
        // But, we need to avoid infinity replacing loop.
        // If replacementString is begun with "#{", we skip it from next searchRange.
        if (! [replacementString hasPrefix:@"#{"]) {
          searchRange.location = replacementBegin.location;
        }
        break;
      }
    }

    searchRange.length = [string length] - searchRange.location;
  }

  return string;
}

@end
