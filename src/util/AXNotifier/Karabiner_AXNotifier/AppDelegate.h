// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>
#import "KarabinerClient.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property(assign) IBOutlet NSWindow* window;
@property(assign) IBOutlet KarabinerClient* client;

@end
