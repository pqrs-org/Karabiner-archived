// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>
#import "KeyRemap4MacBookClient.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSTextField* text;
@property (assign) IBOutlet NSWindow* window;
@property (assign) IBOutlet KeyRemap4MacBookClient* client;

@end
