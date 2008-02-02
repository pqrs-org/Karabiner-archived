// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <PreferencePanes/PreferencePanes.h>
#import "SysctlWrapper.h"
#import "AdminAction.h"

@interface KeyRemap4MacBookPref : NSPreferencePane
{
  NSXMLDocument *_XMLDocument;
  SysctlWrapper *_sysctlWrapper;
  AdminAction *_adminAction;

  IBOutlet id _outlineView;
  IBOutlet id _versionText;
  IBOutlet id _keyrepeat_initialWait;
  IBOutlet id _keyrepeat_wait;
}

- (void) mainViewDidLoad;

- (IBAction)setKeyRepeat_initialWait:(id)sender;
- (IBAction)setKeyRepeat_wait:(id)sender;

@end
