#import <IOKit/hidsystem/IOHIDLib.h>
#import "IOHIDPostEventWrapper.h"

@implementation IOHIDPostEventWrapper

- (id) init
{
  self = [super init];

  if (self) {
    // ------------------------------------------------------------
    // Getting eventDriver_

    mach_port_t masterPort = 0;
    mach_port_t service = 0;
    mach_port_t iter = 0;
    kern_return_t kr;

    // Getting master device port
    kr = IOMasterPort(bootstrap_port, &masterPort);
    if (KERN_SUCCESS != kr) goto finish;

    kr = IOServiceGetMatchingServices(masterPort, IOServiceMatching(kIOHIDSystemClass), &iter);
    if (KERN_SUCCESS != kr) goto finish;

    service = IOIteratorNext(iter);
    if (! service) goto finish;

    kr = IOServiceOpen(service, mach_task_self(), kIOHIDParamConnectType, &eventDriver_);
    if (KERN_SUCCESS != kr) goto finish;

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

- (void) postAuxKey:(uint8_t)auxKeyCode
{
  if (! eventDriver_) return;

  uint32_t keydownup[] = { NX_KEYDOWN, NX_KEYUP };

  for (size_t i = 0; i < sizeof(keydownup) / sizeof(keydownup[0]); ++i) {
    NXEventData event;
    bzero(&event, sizeof(event));
    event.compound.subType = NX_SUBTYPE_AUX_CONTROL_BUTTONS;
    event.compound.misc.L[0] = (auxKeyCode << 16 | keydownup[i] << 8);

    IOGPoint loc = { 0, 0 };
    kern_return_t kr = IOHIDPostEvent(eventDriver_, NX_SYSDEFINED, loc, &event, kNXEventDataVersion, 0, FALSE);
    if (KERN_SUCCESS != kr) {
      NSLog(@"[ERROR] IOHIDPostEvent returned 0x%x", kr);
    }
  }
}

@end
