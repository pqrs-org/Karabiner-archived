// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>
#import "EventQueue.h"

@interface KeyResponder : NSView {
  IBOutlet EventQueue* eventqueue_;
  IBOutlet KarabinerClient* client_;
}
@end
