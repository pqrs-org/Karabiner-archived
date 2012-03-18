//
//  KeyRemap4MacBookTest.m
//  KeyRemap4MacBookTest
//
//  Created by Takayama Fumihiko on 11/12/02.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import "KeyRemap4MacBookTest.h"
#import "NSString+HashBraces.h"

@implementation KeyRemap4MacBookTest

- (void) setUp
{
  [super setUp];

  // Set-up code here.
}

- (void) tearDown
{
  // Tear-down code here.

  [super tearDown];
}

- (void) testNSStringstringByReplacingHashBracesOccurrencesOfDictionary
{
  NSDictionary* replacementDictionary = [NSDictionary dictionaryWithObjectsAndKeys:
                                         @"AAAAAAAA", @"{{AAA}}",
                                         @"XXXXXX", @"{{XXX}}",              // same length
                                         @"Y", @"{{YYY}}",
                                         @"", @"{{ZZZ}}",
                                         @"{{LOOP1}}", @"{{LOOP1}}",
                                         @"   {{LOOP2}}   ", @"{{LOOP2}}",
                                         nil];
  // no replacing
  {
    NSString* string = @"hello world.";
    NSString* expect = string;
    NSString* actual = [string stringByReplacingHashBracesOccurrencesOfDictionary:replacementDictionary];
    STAssertEqualObjects(actual, expect, @"no replacing");
  }

  // normal replacing
  {
    NSString* string = @"{{AAA}} {{XXX}} {{YYY}} {{ZZZ}}";
    NSString* expect = @"AAAAAAAA XXXXXX Y ";
    NSString* actual = [string stringByReplacingHashBracesOccurrencesOfDictionary:replacementDictionary];
    STAssertEqualObjects(actual, expect, @"normal replacing");
  }

  // unknown replacing
  {
    NSString* string = @"{{AAA}} {{UNKNOWN}} {{YYY}} {{ZZZ}}";
    NSString* expect = @"AAAAAAAA {{UNKNOWN}} Y ";
    NSString* actual = [string stringByReplacingHashBracesOccurrencesOfDictionary:replacementDictionary];
    STAssertEqualObjects(actual, expect, @"unknown replacing");
  }

  // no space replacing
  {
    NSString* string = @"{{AAA}}{{XXX}}{{YYY}}{{ZZZ}}{{ZZZ}}{{XXX}}";
    NSString* expect = @"AAAAAAAAXXXXXXYXXXXXX";
    NSString* actual = [string stringByReplacingHashBracesOccurrencesOfDictionary:replacementDictionary];
    STAssertEqualObjects(actual, expect, @"no space replacing");
  }

  // looped replacing
  {
    NSString* string = @"{{LOOP1}}{{LOOP2}}";
    NSString* expect = @"{{LOOP1}}   {{LOOP2}}   ";
    NSString* actual = [string stringByReplacingHashBracesOccurrencesOfDictionary:replacementDictionary];
    STAssertEqualObjects(actual, expect, @"looped replacing");
  }

  // performance test
  {
    NSString* string = [NSString stringWithContentsOfFile:@"../../files/prefpane/output/checkbox.xml"];
    NSMutableDictionary* dict = [NSMutableDictionary new];
    for (int i = 0; i < 1000; ++i) {
      [dict setObject:[NSString stringWithFormat:@"VALUE%d", i] forKey:[NSString stringWithFormat:@"{{NAME%d}}", i]];
    }
    [string stringByReplacingHashBracesOccurrencesOfDictionary:dict];
  }
}

@end
