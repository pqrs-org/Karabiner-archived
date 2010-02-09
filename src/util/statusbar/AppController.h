// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>

@interface AppController : NSObject {
  NSStatusItem* _statusItem;
  IBOutlet NSMenu* _statusMenu;
}

- (IBAction) openPreferencePane:(id)sender;
- (IBAction) launchEventViewer:(id)sender;

@end
