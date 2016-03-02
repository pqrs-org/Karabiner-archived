// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "NotificationKeys.h"
#import "PreferencesKeys.h"
#import "PreferencesManager.h"
#import "StatusBar.h"

@interface StatusBar ()

@property(weak) IBOutlet NSMenu* menu;
@property(weak) IBOutlet PreferencesManager* preferencesManager;
@property NSStatusItem* statusItem;

@end

@implementation StatusBar

- (void)observer_ConfigListChanged:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self refresh];
  });
}

- (void)observer_StatusBarConfigurationChangedNotification:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self refresh];
  });
}

- (id)init {
  self = [super init];

  if (self) {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_ConfigListChanged:)
                                                 name:kConfigListChangedNotification
                                               object:nil];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_StatusBarConfigurationChangedNotification:)
                                                 name:kStatusBarConfigurationChangedNotification
                                               object:nil];
  }

  return self;
}

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)refresh {
  if (![[NSUserDefaults standardUserDefaults] boolForKey:kIsStatusBarEnabled]) {
    if (self.statusItem) {
      [[NSStatusBar systemStatusBar] removeStatusItem:self.statusItem];
      self.statusItem = nil;
    }

  } else {
    if (!self.statusItem) {
      NSImage* image = [NSImage imageNamed:@"icon.statusbar"];
      [image setTemplate:YES];

      self.statusItem = [[NSStatusBar systemStatusBar] statusItemWithLength:NSSquareStatusItemLength];

      [self.statusItem setToolTip:@"Karabiner"];
      [self.statusItem setImage:image];
      [self.statusItem setHighlightMode:YES];

      [self.statusItem setMenu:self.menu];
    }

    // setTitle
    if (![[NSUserDefaults standardUserDefaults] boolForKey:kIsShowSettingNameInStatusBar]) {
      [self.statusItem setTitle:@""];
      [self.statusItem setLength:NSSquareStatusItemLength];

    } else {
      NSString* title = [self.preferencesManager configlist_selectedName];
      if (title) {
        NSAttributedString* attributedtitle = [[NSAttributedString alloc] initWithString:title attributes:nil];
        [self.statusItem setAttributedTitle:attributedtitle];
        // We need to set length manually to avoid a bug of OS X 10.10.
        // (Don't use NSVariableStatusItemLength.)
        [self.statusItem setLength:([[NSStatusBar systemStatusBar] thickness] + [attributedtitle size].width) + 2];
      }
    }
  }
}

- (void)statusBarItemSelected:(id)sender {
  NSNumber* idx = [sender representedObject];
  [self.preferencesManager configlist_select:[idx intValue]];
  [self refresh];
}

- (void)menuNeedsUpdate:(NSMenu*)menu {
  // --------------------
  // clear
  for (;;) {
    NSMenuItem* item = [menu itemAtIndex:0];
    if (item == nil || [[item title] isEqualToString:@"endoflist"]) break;

    [menu removeItemAtIndex:0];
  }

  // --------------------
  // append
  NSArray* list = [self.preferencesManager configlist_getConfigList];
  int i = 0;
  NSInteger selectedIndex = [self.preferencesManager configlist_selectedIndex];
  for (NSDictionary* dict in list) {
    if (!dict) continue;

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
