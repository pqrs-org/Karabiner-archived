// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>
#import "EventQueue.h"

@interface KeyResponder : NSView
{
  IBOutlet EventQueue* eventqueue_;
  IBOutlet KeyRemap4MacBookClient* client_;
}
@end
