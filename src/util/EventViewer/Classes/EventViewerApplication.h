/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */
#import <Cocoa/Cocoa.h>

@class EventQueue;

@interface EventViewerApplication : NSApplication {
  IBOutlet EventQueue* eventQueue_;
}

@end
