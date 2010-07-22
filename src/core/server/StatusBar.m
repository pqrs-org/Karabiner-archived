// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "StatusBar.h"
#import "ConfigControl.h"

@implementation StatusBar

- (void) refresh
{
  if (! [ConfigControl isStatusbarEnable]) {
    if (statusItem_) {
      [[NSStatusBar systemStatusBar] removeStatusItem:statusItem_];
      [statusItem_ release];
      statusItem_ = nil;
    }

  } else {
    if (! statusItem_) {
      statusItem_ = [[NSStatusBar systemStatusBar] statusItemWithLength:24];
      [statusItem_ retain];

      [statusItem_ setTitle:@""];
      [statusItem_ setToolTip:@"KeyRemap4MacBook"];
      [statusItem_ setImage:[NSImage imageNamed:@"icon.statusbar.0"]];
      [statusItem_ setAlternateImage:[NSImage imageNamed:@"icon.statusbar.1"]];
      [statusItem_ setHighlightMode:YES];

      [statusItem_ setMenu:menu_];
    }
  }
}

- (void) statusBarItemSelected:(id)sender
{
  NSString* idx = [sender representedObject];
  [ConfigControl select:idx];
}

- (void) menuNeedsUpdate:(NSMenu*)menu
{
  // --------------------
  // clear
  for (;;) {
    NSMenuItem* item = [menu itemAtIndex:0];
    if (item == nil || [[item title] isEqualToString:@"endoflist"]) break;

    [menu removeItemAtIndex:0];
  }

  // --------------------
  // append
  NSArray* list = [ConfigControl getConfigList];
  int i = 0;
  for (id name in list) {
    if ([name length] == 0) continue;

    NSString* selected = [name substringToIndex:1];
    NSString* title = [name substringFromIndex:1];

    NSMenuItem* newItem = [[[NSMenuItem alloc] initWithTitle:title action:@selector(statusBarItemSelected:) keyEquivalent:@""] autorelease];
    NSString* idx = [[[NSString alloc] initWithFormat:@"%d", i] autorelease];
    [newItem setRepresentedObject:idx];
    if ([selected isEqualToString:@"+"]) {
      [newItem setState:NSOnState];
    } else {
      [newItem setState:NSOffState];
    }
    [menu insertItem:newItem atIndex:i];
    ++i;
  }
}

- (IBAction) openPreferencePane:(id)sender
{
  [[NSWorkspace sharedWorkspace] openFile:@"/Library/PreferencePanes/KeyRemap4MacBook.prefPane"];
}

- (IBAction) launchEventViewer:(id)sender
{
  [[NSWorkspace sharedWorkspace] launchApplication:@"/Library/org.pqrs/KeyRemap4MacBook/app/KeyDump.app"];
}

@end
