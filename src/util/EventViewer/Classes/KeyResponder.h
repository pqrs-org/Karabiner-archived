// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>

@class EventQueue;

@interface KeyResponder : NSView {
  IBOutlet EventQueue* eventQueue_;
}
@end
