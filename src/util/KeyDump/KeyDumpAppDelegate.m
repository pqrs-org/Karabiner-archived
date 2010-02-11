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
  char buffer[512];
  getActiveApplicationName(buffer, sizeof(buffer));
  NSString* name = [NSString stringWithCString:buffer encoding:NSUTF8StringEncoding];
  [otherinformationstore_ setApplicationName:name];
}

// ------------------------------------------------------------
static void observer_kTISNotifySelectedKeyboardInputSourceChanged(CFNotificationCenterRef center,
                                                                  void* observer,
                                                                  CFStringRef name,
                                                                  const void* object,
                                                                  CFDictionaryRef userInfo)
{
  char buffer[128];
  getTISPropertyInputModeID(buffer, sizeof(buffer));
  NSString* inputsourcename = [NSString stringWithCString:buffer encoding:NSUTF8StringEncoding];

  OtherInformationStore* ois = observer;
  [ois setInputSourceName:inputsourcename];
}

// ------------------------------------------------------------
- (void) applicationDidFinishLaunching:(NSNotification*)aNotification {
  [window makeFirstResponder:keyResponder_];

  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceDidActivateApplicationNotification:)
                                                             name:NSWorkspaceDidActivateApplicationNotification
                                                           object:nil];

  CFNotificationCenterAddObserver(CFNotificationCenterGetDistributedCenter(),
                                  otherinformationstore_,
                                  observer_kTISNotifySelectedKeyboardInputSourceChanged,
                                  kTISNotifySelectedKeyboardInputSourceChanged,
                                  NULL,
                                  CFNotificationSuspensionBehaviorCoalesce);

  [otherinformationstore_ setVersion];
  [otherinformationstore_ setApplicationName:@"---"];
  observer_kTISNotifySelectedKeyboardInputSourceChanged(NULL, otherinformationstore_, NULL, NULL, NULL);
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication {
  return YES;
}

@end
