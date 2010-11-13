// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>

@interface StatusBar : NSObject {
  NSStatusItem* statusItem_;
  IBOutlet NSMenu* menu_;
}

- (void) refresh;
- (void) statusBarItemSelected:(id)sender;

- (IBAction) openPreferencePane:(id)sender;
- (IBAction) launchEventViewer:(id)sender;

@end
