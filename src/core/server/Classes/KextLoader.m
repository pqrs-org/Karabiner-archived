// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "KextLoader.h"

@implementation KextLoader

+ (void) load
{
  @synchronized(self) {
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
      NSLog(@"kextload");
      system("/Applications/Karabiner.app/Contents/Library/bin/kextload load");
      [NSThread sleepForTimeInterval:0.5];
    });
  }
}

@end
