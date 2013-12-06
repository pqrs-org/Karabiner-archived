/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "Devices.h"
#include "bridge.h"

@implementation Devices

- (id) init
{
  self = [super init];

  if (self) {
    devices_ = [NSMutableArray new];
  }

  return self;
}

- (void) dealloc
{
  [devices_ release];

  [super dealloc];
}

- (NSInteger) numberOfRowsInTableView:(NSTableView*)aTableView
{
  return [devices_ count];
}

- (id) tableView:(NSTableView*)aTableView objectValueForTableColumn:(NSTableColumn*)aTableColumn row:(NSInteger)rowIndex
{
  NSDictionary* dict = devices_[rowIndex];
  return dict[[aTableColumn identifier]];
}

- (IBAction) refresh:(id)sender
{
  @synchronized(self) {
    [devices_ removeAllObjects];

    for (NSArray* a in @[@[@"Keyboard", @(BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_KEYBOARD)],
                         @[@"Consumer", @(BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_CONSUMER)],
                         @[@"Pointing", @(BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_POINTING)]]) {
      NSInteger type = [a[1] integerValue];
      @try {
        for (NSDictionary* d in [[client_ proxy] device_information : type]) {
          NSMutableDictionary* newdict = [NSMutableDictionary dictionaryWithDictionary:d];
          newdict[@"deviceType"] = a[0];
          [devices_ addObject:newdict];
        }
      } @catch (NSException* exception) {
        NSLog(@"%@", exception);
      }
    }
  }

  [view_ reloadData];
}

- (IBAction) copy:(id)sender
{
  NSPasteboard* pboard = [NSPasteboard generalPasteboard];
  NSMutableString* string = [[NSMutableString new] autorelease];

  for (NSDictionary* dict in devices_) {
    [string appendFormat:@"%@\n    %@ (%@)\n    Vendor ID:%@\n    Product ID:%@\n    Location ID:%@\n\n",
     dict[@"deviceType"],
     dict[@"product"],
     dict[@"manufacturer"],
     dict[@"vendorID"],
     dict[@"productID"],
     dict[@"locationID"]];
  }

  if ([string length] > 0) {
    [pboard clearContents];
    [pboard writeObjects:@[string]];
  }
}

@end
