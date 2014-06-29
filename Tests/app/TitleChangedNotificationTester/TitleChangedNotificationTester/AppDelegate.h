// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow* window1;
@property (assign) IBOutlet NSWindow* window2;
@property (assign) IBOutlet NSWindow* window3;

- (IBAction) refreshWindow1Title:(id)sender;

@end
