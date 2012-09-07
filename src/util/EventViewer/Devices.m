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

  for (NSDictionary* d in devices_) {
    [string appendFormat:@"Vendor ID:%@\tProduct ID:%@\tManufacturer:%@\tDevice Name:%@\tLocation ID:%@\n",
     [d objectForKey:@"vendorID"],
     [d objectForKey:@"productID"],
     [d objectForKey:@"manufacturer"],
     [d objectForKey:@"product"],
     [d objectForKey:@"locationID"]];
  }

  if ([string length] > 0) {
    [pboard clearContents];
    [pboard writeObjects:[NSArray arrayWithObject:string]];
  }
}

@end
