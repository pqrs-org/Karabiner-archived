@import IOKit;
#import "IOHIDPostEventWrapper.h"

@interface IOHIDPostEventWrapper ()

@property mach_port_t eventDriver;
@property IOOptionBits eventFlags;

@end

@implementation IOHIDPostEventWrapper

- (id)init {
  self = [super init];

  if (self) {
    self.eventFlags = 0;

    // ------------------------------------------------------------
    // Getting eventDriver

    mach_port_t masterPort = 0;
    mach_port_t service = 0;
    mach_port_t iter = 0;
    mach_port_t ev = 0;
    kern_return_t kr;

    // Getting master device port
    kr = IOMasterPort(bootstrap_port, &masterPort);
    if (KERN_SUCCESS != kr) goto finish;

    kr = IOServiceGetMatchingServices(masterPort, IOServiceMatching(kIOHIDSystemClass), &iter);
    if (KERN_SUCCESS != kr) goto finish;

    service = IOIteratorNext(iter);
    if (!service) goto finish;

    kr = IOServiceOpen(service, mach_task_self(), kIOHIDParamConnectType, &ev);
    if (KERN_SUCCESS != kr) goto finish;

    self.eventDriver = ev;

  finish:
    if (service) {
      IOObjectRelease(service);
    }
    if (iter) {
      IOObjectRelease(iter);
    }
  }

  return self;
}

- (void)postModifierKey:(IOOptionBits)mask keydown:(BOOL)keydown {
  NXEventData event;
  bzero(&event, sizeof(event));

  if (keydown) {
    self.eventFlags |= mask;
  } else {
    self.eventFlags &= ~mask;
  }

  IOGPoint loc = {0, 0};
  kern_return_t kr = IOHIDPostEvent(self.eventDriver, NX_FLAGSCHANGED, loc, &event, kNXEventDataVersion, self.eventFlags, TRUE);
  if (KERN_SUCCESS != kr) {
    NSLog(@"[ERROR] IOHIDPostEvent returned 0x%x", kr);
  }
}

- (void)postAuxKey:(uint8_t)auxKeyCode {
  if (!self.eventDriver) return;

  uint32_t keydownup[] = {NX_KEYDOWN, NX_KEYUP};

  for (size_t i = 0; i < sizeof(keydownup) / sizeof(keydownup[0]); ++i) {
    NXEventData event;
    bzero(&event, sizeof(event));
    event.compound.subType = NX_SUBTYPE_AUX_CONTROL_BUTTONS;
    event.compound.misc.L[0] = (auxKeyCode << 16 | keydownup[i] << 8);

    IOGPoint loc = {0, 0};
    kern_return_t kr = IOHIDPostEvent(self.eventDriver, NX_SYSDEFINED, loc, &event, kNXEventDataVersion, self.eventFlags, FALSE);
    if (KERN_SUCCESS != kr) {
      NSLog(@"[ERROR] IOHIDPostEvent returned 0x%x", kr);
    }
  }
}

- (void)postPowerKey {
  if (!self.eventDriver) return;

  NXEventData event;
  bzero(&event, sizeof(event));
  event.compound.subType = NX_SUBTYPE_POWER_KEY;

  IOGPoint loc = {0, 0};
  kern_return_t kr = IOHIDPostEvent(self.eventDriver, NX_SYSDEFINED, loc, &event, kNXEventDataVersion, 0, FALSE);
  if (KERN_SUCCESS != kr) {
    NSLog(@"[ERROR] IOHIDPostEvent returned 0x%x", kr);
  }
}

- (void)postKey:(uint8_t)keyCode {
  if (keyCode == NX_POWER_KEY) {
    [self postPowerKey];
  } else {
    [self postAuxKey:keyCode];
  }
}

@end
