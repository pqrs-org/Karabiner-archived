//
//  KeyDumpAppDelegate.m
//  KeyDump
//
//  Created by Takayama Fumihiko on 09/10/25.
//

#import "KeyDumpAppDelegate.h"
#import <Carbon/Carbon.h>
#import "server_objc_part.h"

@implementation KeyDumpAppDelegate

@synthesize window;

// ------------------------------------------------------------
- (void) observer_NSWorkspaceDidActivateApplicationNotification:(NSNotification*)notification
{
  NSString* name = [serverobjcpart_ getActiveApplicationName];
  [otherinformationstore_ setApplicationName:name];
}

// ------------------------------------------------------------
- (void) observer_kTISNotifySelectedKeyboardInputSourceChanged:(NSNotification*)notification
{
  NSString* name = [serverobjcpart_ getTISPropertyInputModeID];
  [otherinformationstore_ setInputSourceName:name];
}

// ------------------------------------------------------------
- (void) applicationDidFinishLaunching:(NSNotification*)aNotification {
  [window makeFirstResponder:keyResponder_];

  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceDidActivateApplicationNotification:)
                                                             name:NSWorkspaceDidActivateApplicationNotification
                                                           object:nil];

  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(observer_kTISNotifySelectedKeyboardInputSourceChanged:)
                                                          name:(NSString*)(kTISNotifySelectedKeyboardInputSourceChanged)
                                                        object:nil];

  [otherinformationstore_ setVersion];
  [otherinformationstore_ setApplicationName:nil];

  [self observer_kTISNotifySelectedKeyboardInputSourceChanged:nil];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication {
  return YES;
}

@end
