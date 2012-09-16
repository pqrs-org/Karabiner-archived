/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "Devices.h"
#include "bridge.h"

@implementation Devices

- (id) init
{
  self = [super init];

  if (self) {
    devices_ = [NSArray new];
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
  NSInteger type = BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_KEYBOARD;

  switch ([segment_ selectedSegment]) {
    case 1: type = BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_CONSUMER; break;
    case 2: type = BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_POINTING; break;
  }

  [devices_ release];
  devices_ = [[[client_ proxy] device_information:type] retain];

  [view_ reloadData];
}

- (IBAction) copy:(id)sender
{
  NSPasteboard* pboard = [NSPasteboard generalPasteboard];
  NSMutableString* string = [[NSMutableString new] autorelease];

  for (NSDictionary* dict in devices_) {
    [string appendFormat:@"%@ (%@)\n    Vendor ID:%@\n    Product ID:%@\n    Location ID:%@\n\n",
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
