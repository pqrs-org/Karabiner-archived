// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "NotificationKeys.h"
#import "PreferencesKeys.h"
#import "PreferencesManager.h"
#import "StatusBar.h"

@interface StatusBar ()
{
  NSStatusItem* statusItem_;
}
@end

@implementation StatusBar

- (void) observer_ConfigListChanged:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
    [self refresh];
  });
}

- (id) init
{
  self = [super init];

  if (self) {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_ConfigListChanged:)
                                                 name:kConfigListChangedNotification
                                               object:nil];
  }

  return self;
}

- (void) dealloc
{
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void) refresh
{
  if (! [[NSUserDefaults standardUserDefaults] boolForKey:kIsStatusBarEnabled]) {
    if (statusItem_) {
      [[NSStatusBar systemStatusBar] removeStatusItem:statusItem_];
      statusItem_ = nil;
    }

  } else {
    if (! statusItem_) {
      statusItem_ = [[NSStatusBar systemStatusBar] statusItemWithLength:24];

      [statusItem_ setTitle:@""];
      [statusItem_ setToolTip:@"KeyRemap4MacBook"];
      [statusItem_ setImage:[NSImage imageNamed:@"icon.statusbar.0"]];
      [statusItem_ setAlternateImage:[NSImage imageNamed:@"icon.statusbar.1"]];
      [statusItem_ setHighlightMode:YES];

      [statusItem_ setMenu:menu_];
    }

    // setTitle
    if (! [[NSUserDefaults standardUserDefaults] boolForKey:kIsShowSettingNameInStatusBar]) {
      [statusItem_ setTitle:@""];
      [statusItem_ setLength:24];

    } else {
      NSString* title = [preferencesManager_ configlist_selectedName];
      if (title) {
        NSAttributedString* attributedtitle = [[NSAttributedString alloc] initWithString:title attributes:nil];
        [statusItem_ setAttributedTitle:attributedtitle];
        [statusItem_ setLength:(24 + [attributedtitle size].width)];
      }
    }
  }
}

- (void) statusBarItemSelected:(id)sender
{
  NSNumber* idx = [sender representedObject];
  [preferencesManager_ configlist_select:[idx intValue]];
  [self refresh];
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
  NSArray* list = [preferencesManager_ configlist_getConfigList];
  int i = 0;
  NSInteger selectedIndex = [preferencesManager_ configlist_selectedIndex];
  for (NSDictionary* dict in list) {
    if (! dict) continue;

    NSString* title = dict[@"name"];
    NSMenuItem* newItem = [[NSMenuItem alloc] initWithTitle:title action:@selector(statusBarItemSelected:) keyEquivalent:@""];

    [newItem setTarget:self];
    [newItem setRepresentedObject:@(i)];

    if (selectedIndex == i) {
      [newItem setState:NSOnState];
    } else {
      [newItem setState:NSOffState];
    }
    [menu insertItem:newItem atIndex:i];
    ++i;
  }
}

@end
