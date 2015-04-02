// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@interface StatusMessageManager : NSObject

- (void)setupStatusMessageManager;

- (void)resetStatusMessage;
- (void)setStatusMessage:(NSUInteger)lineIndex message:(NSString *)message;

- (IBAction)refresh:(id)sender;

@end
