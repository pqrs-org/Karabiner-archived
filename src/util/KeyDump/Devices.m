/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "Devices.h"

@implementation Devices

- (id) init
{
  self = [super init];

  if (self) {
    devices_ = [[NSDictionary alloc] initWithObjectsAndKeys:
                [[NSMutableArray new] autorelease], @"keyboard",
                [[NSMutableArray new] autorelease], @"consumer",
                [[NSMutableArray new] autorelease], @"pointing",
                nil];
  }

  return self;
}

- (void) dealloc
{
  [devices_ release];

  [super dealloc];
}

- (NSArray*) getDevicesFromSegmentedControl
{
  NSString* key = @"keyboard";
  switch ([segment_ selectedSegment]) {
    case 1: key = @"consumer"; break;
    case 2: key = @"pointing"; break;
  }
  return [devices_ objectForKey:key];
}

- (NSInteger) numberOfRowsInTableView:(NSTableView*)aTableView
{
  return [[self getDevicesFromSegmentedControl] count];
}

- (id) tableView:(NSTableView*)aTableView objectValueForTableColumn:(NSTableColumn*)aTableColumn row:(NSInteger)rowIndex
{
  NSDictionary* dict = [[self getDevicesFromSegmentedControl] objectAtIndex:rowIndex];
  return [dict objectForKey:[aTableColumn identifier]];
}

- (IBAction) refresh:(id)sender
{
  NSLog(@"refresh");
  [view_ reloadData];
}

- (IBAction) copy:(id)sender
{}

@end
