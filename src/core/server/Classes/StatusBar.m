// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "StatusBar.h"
#import "NotificationKeys.h"
#import "PreferencesKeys.h"
#import "PreferencesManager.h"
#import "PreferencesModel.h"
#import "weakify.h"

@interface StatusBar ()

@property(weak) IBOutlet NSMenu* menu;
@property(weak) IBOutlet PreferencesManager* preferencesManager;
@property(weak) IBOutlet PreferencesModel* preferencesModel;
@property NSStatusItem* statusItem;

@end

@implementation StatusBar

- (void)observer_PreferencesChanged:(NSNotification*)notification {
  @weakify(self);

  dispatch_async(dispatch_get_main_queue(), ^{
    @strongify(self);
    if (!self) return;

    [self refresh];
  });
}

- (void)observer_StatusBarConfigurationChangedNotification:(NSNotification*)notification {
  @weakify(self);

  dispatch_async(dispatch_get_main_queue(), ^{
    @strongify(self);
    if (!self) return;

    [self refresh];
  });
}

- (instancetype)init {
  self = [super init];

  if (self) {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_PreferencesChanged:)
                                                 name:kPreferencesChangedNotification
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
  if (!self.preferencesModel.statusBarEnabled) {
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
    if (!self.preferencesModel.showProfileNameInStatusBar) {
      [self.statusItem setTitle:@""];
      [self.statusItem setLength:NSSquareStatusItemLength];

    } else {
      NSString* title = self.preferencesModel.currentProfileName;
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

  self.preferencesModel.currentProfileIndex = [idx intValue];
  [self.preferencesManager save];

  [[NSNotificationCenter defaultCenter] postNotificationName:kProfileChangedNotification object:nil];
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
  int i = 0;
  for (ProfileModel* profileModel in self.preferencesModel.profiles) {
    NSMenuItem* newItem = [[NSMenuItem alloc] initWithTitle:profileModel.name action:@selector(statusBarItemSelected:) keyEquivalent:@""];

    [newItem setTarget:self];
    [newItem setRepresentedObject:@(i)];

    if (self.preferencesModel.currentProfileIndex == i) {
      [newItem setState:NSOnState];
    } else {
      [newItem setState:NSOffState];
    }
    [menu insertItem:newItem atIndex:i];
    ++i;
  }
}

@end
