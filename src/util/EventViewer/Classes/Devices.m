#import "Devices.h"
#import "ServerClient.h"
#include "bridge.h"

@interface Devices ()

@property NSMutableArray* devices;
@property(weak) IBOutlet NSTableView* view;
@property(weak) IBOutlet ServerClient* client;

@end

@implementation Devices

- (id)init {
  self = [super init];

  if (self) {
    self.devices = [NSMutableArray new];
  }

  return self;
}

- (NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView {
  return [self.devices count];
}

- (id)tableView:(NSTableView*)aTableView objectValueForTableColumn:(NSTableColumn*)aTableColumn row:(NSInteger)rowIndex {
  NSDictionary* dict = self.devices[rowIndex];
  return dict[[aTableColumn identifier]];
}

- (void)refresh {
  @synchronized(self) {
    [self.devices removeAllObjects];

    for (NSArray* a in @[ @[ @"Keyboard", @(BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_KEYBOARD) ],
                          @[ @"Consumer", @(BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_CONSUMER) ],
                          @[ @"Pointing", @(BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_POINTING) ] ]) {
      NSInteger type = [a[1] integerValue];
      @try {
        for (NSDictionary* d in [[self.client proxy] device_information:type]) {
          NSMutableDictionary* newdict = [NSMutableDictionary dictionaryWithDictionary:d];
          newdict[@"deviceType"] = a[0];
          [self.devices addObject:newdict];
        }
      }
      @catch (NSException* exception) {
        NSLog(@"%@", exception);
      }
    }
  }

  [self.view reloadData];
}

- (IBAction)refresh:(id)sender {
  [self refresh];
}

- (IBAction)copy:(id)sender {
  NSPasteboard* pboard = [NSPasteboard generalPasteboard];
  NSMutableString* string = [NSMutableString new];

  for (NSDictionary* dict in self.devices) {
    [string appendFormat:@"%@\n"
                         @"    %@ (%@)\n"
                         @"\n"
                         @"    DeviceVendor::RawValue::%@,\n"
                         @"    DeviceProduct::RawValue::%@,\n"
                         @"\n"
                         @"    DeviceLocation::RawValue::%@,\n\n",
                         dict[@"deviceType"],
                         dict[@"product"],
                         dict[@"manufacturer"],
                         dict[@"vendorID"],
                         dict[@"productID"],
                         dict[@"locationID"]];
  }

  if ([string length] > 0) {
    [pboard clearContents];
    [pboard writeObjects:@[ string ]];
  }
}

@end
