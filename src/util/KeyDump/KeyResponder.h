// -*- Mode: objc -*-
//
// KeyResponder.h
//

#import <Cocoa/Cocoa.h>
#import "EventQueue.h"

@interface KeyResponder : NSImageView
{
  IBOutlet EventQueue* eventqueue_;
}
@end
