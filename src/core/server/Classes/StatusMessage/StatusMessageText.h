// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@interface StatusMessageText : NSObject

@property(copy, readonly) NSString* extraText;
@property(copy, readonly) NSString* modifierLockText;
@property(copy, readonly) NSString* modifierStickyText;
@property(copy, readonly) NSString* pointingButtonLockText;

- (void)reset;
- (void)updateText:(NSInteger)index text:(NSString*)text;

@end
