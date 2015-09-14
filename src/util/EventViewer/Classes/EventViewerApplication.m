/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "EventViewerApplication.h"
#import "EventQueue.h"

@implementation EventViewerApplication

- (void)sendEvent:(NSEvent *)event {
  [eventQueue_ pushFromNSApplication:event];

  [super sendEvent:event];
}

@end
