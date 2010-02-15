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
@synthesize serverobjcpart_;
@synthesize otherinformationstore_;

// ------------------------------------------------------------
- (void) observer_NSWorkspaceDidActivateApplicationNotification:(NSNotification*)notification
{
  NSString* name = [serverobjcpart_ getActiveApplicationName];
  [otherinformationstore_ setApplicationName:name];
}

// ------------------------------------------------------------
static void observer_kTISNotifySelectedKeyboardInputSourceChanged(CFNotificationCenterRef center,
                                                                  void* observer,
                                                                  CFStringRef name,
                                                                  const void* object,
                                                                  CFDictionaryRef userInfo)
{
  KeyDumpAppDelegate* app = observer;

  NSString* inputsourcename = [[app serverobjcpart_] getTISPropertyInputModeID];
  [[app otherinformationstore_] setInputSourceName:inputsourcename];
}

// ------------------------------------------------------------
- (void) applicationDidFinishLaunching:(NSNotification*)aNotification {
  [window makeFirstResponder:keyResponder_];

  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceDidActivateApplicationNotification:)
                                                             name:NSWorkspaceDidActivateApplicationNotification
                                                           object:nil];

  CFNotificationCenterAddObserver(CFNotificationCenterGetDistributedCenter(),
                                  self,
                                  observer_kTISNotifySelectedKeyboardInputSourceChanged,
                                  kTISNotifySelectedKeyboardInputSourceChanged,
                                  NULL,
                                  CFNotificationSuspensionBehaviorCoalesce);

  [otherinformationstore_ setVersion];
  [otherinformationstore_ setApplicationName:nil];
  observer_kTISNotifySelectedKeyboardInputSourceChanged(NULL, self, NULL, NULL, NULL);
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication {
  return YES;
}

@end
