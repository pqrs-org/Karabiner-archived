#import "AppController.h"
#import "ConfigList.h"

@implementation AppController

- (void) configItemSelected:(id)sender
{
  NSString *idx = [sender representedObject];
  [ConfigList select:idx];
}

- (void) menuWillOpen:(NSMenu *)menu
{
  // --------------------
  // clear
  for (;;) {
    NSMenuItem *item = [_statusMenu itemAtIndex:0];
    if (item == nil || [[item title] isEqualToString:@"endoflist"]) break;

    [_statusMenu removeItemAtIndex:0];
  }

  // --------------------
  // append
  int selectedIndex = [ConfigList getSelectedIndex];

  int size = [ConfigList getSize];
  int i = 0;
  for (i = 0; i < size; ++i) {
    NSMenuItem *newItem = [[[NSMenuItem alloc] initWithTitle:[ConfigList getName:i] action:@selector(configItemSelected:) keyEquivalent:@""] autorelease];
    NSString *idx = [[[NSString alloc] initWithFormat:@"%d", i] autorelease];
    [newItem setRepresentedObject:idx];
    if (selectedIndex == i) {
      [newItem setState:NSOnState];
    } else {
      [newItem setState:NSOffState];
    }

    [_statusMenu insertItem:newItem atIndex:i];
  }
}

- (void) applicationDidFinishLaunching:(NSNotification *)notification
{
  if (! [ConfigList isStatusbarEnable]) {
    [[NSApplication sharedApplication] terminate:self];
  }

  NSStatusBar *statusBar = [NSStatusBar systemStatusBar];

  _statusItem = [statusBar statusItemWithLength:24];
  [_statusItem retain];

  [_statusItem setTitle:@""];
  [_statusItem setImage:[NSImage imageNamed:@"icon.statusbar.0"]];
  [_statusItem setAlternateImage:[NSImage imageNamed:@"icon.statusbar.1"]];
  [_statusItem setHighlightMode:YES];

  [_statusItem setMenu:_statusMenu];
}

- (IBAction) openPreferencePane:(id)sender
{
  [[NSWorkspace sharedWorkspace] openFile:@"/Library/PreferencePanes/KeyRemap4MacBook.prefPane"];
}

@end
