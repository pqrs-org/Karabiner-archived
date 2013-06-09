#include <IOKit/IOKitLib.h>
#import "AppDelegate.h"

enum { MAX_FINGERS = 4 };
static int current_status_[MAX_FINGERS];
static FingerStatus* lastFingerStatus_ = nil;
static BOOL has_last_device = NO;
static int last_device = 0;

@implementation AppDelegate

- (id) init
{
  self = [super init];

  if (self) {
    lastFingerStatus_ = [FingerStatus new];

    for (int i = 0; i < MAX_FINGERS; ++i) {
      current_status_[i] = 0;
    }
  }

  return self;
}

- (void) dealloc
{
  [mtdevices_ release];
  [lastFingerStatus_ release];

  [super dealloc];
}

// ------------------------------------------------------------
typedef struct {
  float x;
  float y;
} mtPoint;

typedef struct {
  mtPoint position;
  mtPoint velocity;
} mtReadout;

typedef struct {
  int frame;
  double timestamp;
  int identifier;
  int state;
  int unknown1;
  int unknown2;
  mtReadout normalized;
  float size;
  int unknown3;
  float angle;
  float majorAxis;
  float minorAxis;
  mtReadout unknown4;
  int unknown5[2];
  float unknown6;
} Finger;

typedef void* MTDeviceRef;
typedef int (* MTContactCallbackFunction)(int, Finger*, int, double, int);

CFMutableArrayRef MTDeviceCreateList(void);
void MTRegisterContactFrameCallback(MTDeviceRef, MTContactCallbackFunction);
void MTUnregisterContactFrameCallback(MTDeviceRef, MTContactCallbackFunction);
void MTDeviceStart(MTDeviceRef, int);
void MTDeviceStop(MTDeviceRef, int);

IgnoredAreaView* global_ignoredAreaView_ = nil;
KeyRemap4MacBookClient* global_client_ = nil;

static void setPreference(int fingers, int newvalue) {
  NSAutoreleasePool* pool = [NSAutoreleasePool new];
  {
    NSString* name = [PreferencesController getSettingName:fingers];
    if ([name length] > 0) {
      @try {
        [[global_client_ proxy] setValueForName:newvalue forName:name];
      } @catch (NSException* exception) {
        NSLog(@"%@", exception);
      }
    }
  }
  [pool drain];
}

- (void) resetPreferences
{
  for (int i = 0; i < MAX_FINGERS; ++i) {
    setPreference(i + 1, 0);
  }
}

// ------------------------------------------------------------
// Multitouch callback
static int callback(int device, Finger* data, int fingers, double timestamp, int frame) {
  dispatch_async(dispatch_get_main_queue(), ^{
                   // ------------------------------------------------------------
                   // If there are multiple devices (For example, Trackpad and Magic Mouse),
                   // we handle only one device at the same time.
                   if (has_last_device) {
                     // ignore other devices.
                     if (device != last_device) {
                       return;
                     }
                   }

                   if (fingers == 0) {
                     has_last_device = NO;
                   } else {
                     has_last_device = YES;
                     last_device = device;
                   }

                   // ------------------------------------------------------------
                   [global_ignoredAreaView_ clearFingers];

                   int valid_fingers = 0;

                   FingerStatus* fingerStatus = [FingerStatus new];

                   for (int i = 0; i < fingers; ++i) {
                     // state values:
                     //   4: touched
                     //   1-3,5-7: near
                     if (data[i].state != 4) {
                       continue;
                     }

                     int identifier = data[i].identifier;
                     NSPoint point = NSMakePoint (data[i].normalized.position.x, data[i].normalized.position.y);

                     BOOL ignored = NO;
                     if ([IgnoredAreaView isIgnoredArea:point]) {
                       ignored = YES;

                       // Finding FingerStatus by identifier.
                       if ([lastFingerStatus_ isActive:identifier]) {
                         // If a finger is already active, we should not ignore this finger.
                         // (This finger has been moved into ignored area from active area.)
                         ignored = NO;
                       }
                     }

                     [fingerStatus add:identifier active:(! ignored)];

                     if (! ignored) {
                       ++valid_fingers;
                     }

                     [global_ignoredAreaView_ addFinger:point ignored:ignored];
                   }

                   FingerStatus* l = lastFingerStatus_;
                   lastFingerStatus_ = fingerStatus;
                   [l release];

                   // ----------------------------------------
                   // deactivating settings first.
                   for (int i = 0; i < MAX_FINGERS; ++i) {
                     if (current_status_[i] && valid_fingers != i + 1) {
                       current_status_[i] = 0;
                       setPreference (i + 1, 0);
                     }
                   }

                   // activating setting.
                   //
                   // Note: Set current_status_ only if the targeted setting is enabled.
                   // If not, unintentional deactivation is called in above.
                   //
                   // - one finger: disabled
                   // - two fingers: enabled
                   //
                   // In this case,
                   // we must not call "setPreference" if only one finger is touched/released on multi-touch device.
                   // If we don't check [PreferencesController isSettingEnabled],
                   // setPreference is called in above when we release one finger from device.
                   //
                   if (valid_fingers > 0 && current_status_[valid_fingers - 1] == 0 &&
                       [PreferencesController isSettingEnabled:valid_fingers]) {
                     current_status_[valid_fingers - 1] = 1;
                     setPreference (valid_fingers, 1);
                   }
                 });
  return 0;
}

- (void) setcallback:(BOOL)isset {
  @synchronized(self) {
    // ------------------------------------------------------------
    // unset callback (even if isset is YES.)
    if (mtdevices_) {
      for (NSUInteger i = 0; i < [mtdevices_ count]; ++i) {
        MTDeviceRef device = [mtdevices_ objectAtIndex:i];
        if (! device) continue;

        MTDeviceStop(device, 0);
        MTUnregisterContactFrameCallback(device, callback);
      }
      [mtdevices_ release];
      mtdevices_ = nil;
    }

    // ------------------------------------------------------------
    // set callback if needed
    if (isset) {
      mtdevices_ = (NSArray*)(MTDeviceCreateList());
      if (mtdevices_) {
        for (NSUInteger i = 0; i < [mtdevices_ count]; ++i) {
          MTDeviceRef device = [mtdevices_ objectAtIndex:i];
          if (! device) continue;

          MTRegisterContactFrameCallback(device, callback);
          MTDeviceStart(device, 0);
        }
      }
    }

    [self resetPreferences];
  }
}

// ------------------------------------------------------------
// IONotification
- (void) release_iterator:(io_iterator_t)iterator {
  for (;;) {
    io_object_t obj = IOIteratorNext(iterator);
    if (! obj) break;

    IOObjectRelease(obj);
  }
}

static void observer_IONotification(void* refcon, io_iterator_t iterator) {
  dispatch_async(dispatch_get_main_queue(), ^{
                   // Relaunch when devices are plugged/unplugged.
                   NSLog (@"observer_IONotification");
                   [NSTask launchedTaskWithLaunchPath:[[NSBundle mainBundle] executablePath] arguments:[NSArray array]];
                   [NSApp terminate:nil];
                 });
}

- (void) unregisterIONotification {
  NSLog(@"unregisterIONotification");

  @synchronized(self) {
    if (notifyport_) {
      if (loopsource_) {
        CFRunLoopSourceInvalidate(loopsource_);
        loopsource_ = nil;
      }
      IONotificationPortDestroy(notifyport_);
      notifyport_ = nil;
    }
  }
}

- (void) registerIONotification {
  NSLog(@"registerIONotification");

  @synchronized(self) {
    if (notifyport_) {
      [self unregisterIONotification];
    }

    notifyport_ = IONotificationPortCreate(kIOMasterPortDefault);
    if (! notifyport_) {
      NSLog(@"[ERROR] IONotificationPortCreate");
      return;
    }

    // ------------------------------------------------------------
    NSMutableDictionary* match = [(NSMutableDictionary*)(IOServiceMatching("AppleMultitouchDevice")) autorelease];

    // ----------------------------------------------------------------------
    io_iterator_t it;
    kern_return_t kr;

    [match retain]; // for kIOTerminatedNotification
    kr = IOServiceAddMatchingNotification(notifyport_,
                                          kIOTerminatedNotification,
                                          (CFMutableDictionaryRef)(match),
                                          &observer_IONotification,
                                          self,
                                          &it);
    if (kr != kIOReturnSuccess) {
      NSLog(@"[ERROR] IOServiceAddMatchingNotification");
      return;
    }
    [self release_iterator:it];

    [match retain]; // for kIOMatchedNotification
    kr = IOServiceAddMatchingNotification(notifyport_,
                                          kIOMatchedNotification,
                                          (CFMutableDictionaryRef)(match),
                                          &observer_IONotification,
                                          self,
                                          &it);
    if (kr != kIOReturnSuccess) {
      NSLog(@"[ERROR] IOServiceAddMatchingNotification");
      return;
    }
    [self release_iterator:it];

    // ----------------------------------------------------------------------
    loopsource_ = IONotificationPortGetRunLoopSource(notifyport_);
    if (! loopsource_) {
      NSLog(@"[ERROR] IONotificationPortGetRunLoopSource");
      return;
    }
    CFRunLoopAddSource(CFRunLoopGetCurrent(), loopsource_, kCFRunLoopDefaultMode);
  }
}

// ------------------------------------------------------------
- (void) observer_NSWorkspaceDidWakeNotification:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
                   NSLog (@"observer_NSWorkspaceDidWakeNotification");

                   // sleep until devices are settled.
                   [NSThread sleepForTimeInterval:1.0];

                   if ([[NSUserDefaults standardUserDefaults] boolForKey:@"relaunchAfterWakeUpFromSleep"]) {
                     double wait = [[[NSUserDefaults standardUserDefaults] stringForKey:@"relaunchWait"] doubleValue];
                     if (wait > 0) {
                       [NSThread sleepForTimeInterval:wait];
                     }

                     [NSTask launchedTaskWithLaunchPath:[[NSBundle mainBundle] executablePath] arguments:[NSArray array]];
                     [NSApp terminate:self];
                   }

                   has_last_device = NO;

                   [self setcallback:YES];
                 });
}

- (void) registerWakeNotification
{
  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceDidWakeNotification:)
                                                             name:NSWorkspaceDidWakeNotification
                                                           object:nil];
}

- (void) unregisterWakeNotification
{
  [[[NSWorkspace sharedWorkspace] notificationCenter] removeObserver:self
                                                                name:NSWorkspaceDidWakeNotification
                                                              object:nil];
}

// ----------------------------------------
- (void) observer_NSWorkspaceSessionDidBecomeActiveNotification:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
                   NSLog (@"observer_NSWorkspaceSessionDidBecomeActiveNotification");
                   [self registerIONotification];
                   [self registerWakeNotification];

                   // sleep until devices are settled.
                   [NSThread sleepForTimeInterval:1.0];

                   [self setcallback:YES];
                 });
}

- (void) observer_NSWorkspaceSessionDidResignActiveNotification:(NSNotification*)notification
{
  dispatch_async(dispatch_get_main_queue(), ^{
                   NSLog (@"observer_NSWorkspaceSessionDidResignActiveNotification");
                   [self unregisterIONotification];
                   [self unregisterWakeNotification];
                   [self setcallback:NO];
                 });
}

// ------------------------------------------------------------
- (void) applicationDidFinishLaunching:(NSNotification*)aNotification {
  [preferences_ load];

  if (! [[NSUserDefaults standardUserDefaults] boolForKey:@"hideIconInDock"]) {
    ProcessSerialNumber psn = { 0, kCurrentProcess };
    TransformProcessType(&psn, kProcessTransformToForegroundApplication);
  }

  global_ignoredAreaView_ = ignoredAreaView_;
  global_client_ = client_;

  [self registerIONotification];
  [self registerWakeNotification];

  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceSessionDidBecomeActiveNotification:)
                                                             name:NSWorkspaceSessionDidBecomeActiveNotification
                                                           object:nil];

  [[[NSWorkspace sharedWorkspace] notificationCenter] addObserver:self
                                                         selector:@selector(observer_NSWorkspaceSessionDidResignActiveNotification:)
                                                             name:NSWorkspaceSessionDidResignActiveNotification
                                                           object:nil];

  [self setcallback:YES];
}

- (void) applicationWillTerminate:(NSNotification*)aNotification {
  [self setcallback:NO];
}

- (BOOL) applicationShouldHandleReopen:(NSApplication*)theApplication hasVisibleWindows:(BOOL)flag
{
  [preferences_ show];
  return YES;
}

@end
