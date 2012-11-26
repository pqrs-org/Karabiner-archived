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
  NSDictionary* dict = [devices_ objectAtIndex:rowIndex];
  return [dict objectForKey:[aTableColumn identifier]];
}

- (IBAction) refresh:(id)sender
{
  @synchronized(self) {
    [devices_ removeAllObjects];

    for (NSArray* a in @[@[@"Keyboard", [NSNumber numberWithInt:BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_KEYBOARD]],
                         @[@"Consumer", [NSNumber numberWithInt:BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_CONSUMER]],
                         @[@"Pointing", [NSNumber numberWithInt:BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_POINTING]]]) {
      NSInteger type = [[a objectAtIndex:1] integerValue];
      for (NSDictionary* d in [[client_ proxy] device_information : type]) {
        NSMutableDictionary* newdict = [NSMutableDictionary dictionaryWithDictionary:d];
        [newdict setObject:[a objectAtIndex:0] forKey:@"deviceType"];
        [devices_ addObject:newdict];
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
     [dict objectForKey:@"deviceType"],
     [dict objectForKey:@"product"],
     [dict objectForKey:@"manufacturer"],
     [dict objectForKey:@"vendorID"],
     [dict objectForKey:@"productID"],
     [dict objectForKey:@"locationID"]];
  }

  if ([string length] > 0) {
    [pboard clearContents];
    [pboard writeObjects:[NSArray arrayWithObject:string]];
  }
}

@end
