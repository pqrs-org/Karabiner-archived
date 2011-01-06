// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-
#import <Cocoa/Cocoa.h>
#import "PreferencesManager.h"

@interface StatusWindow : NSObject {
  IBOutlet NSWindow* statuswindow_;
  IBOutlet NSTextField* label_;
  IBOutlet NSImageView* backgroud_;
  IBOutlet PreferencesManager* preferencesmanager_;

  NSMutableArray* lines_;
  NSViewAnimation* animation_;

  int last_parameter_statuswindow_alpha_font_;
  int last_parameter_statuswindow_alpha_background_;
  int last_parameter_statuswindow_posx_adjustment_;
  int last_parameter_statuswindow_posy_adjustment_;
  int last_parameter_statuswindow_fadeout_duration_;
}

- (void) setupStatusWindow;

- (void) resetStatusMessage;
- (void) setStatusMessage:(NSUInteger)lineIndex message:(NSString*)message;

- (void) refreshWindowPosition:(BOOL)force;

@end
