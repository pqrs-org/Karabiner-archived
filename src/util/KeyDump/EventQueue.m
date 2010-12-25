/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "EventQueue.h"

@implementation EventQueue

enum {
  MAXNUM = 50,
};

- (id) init
{
  self = [super init];
  if (self) {
    queue_ = [[NSMutableArray alloc] init];
  }

  return self;
}

- (NSInteger) numberOfRowsInTableView:(NSTableView*)aTableView
{
  return [queue_ count];
}

- (id) tableView:(NSTableView*)aTableView objectValueForTableColumn:(NSTableColumn*)aTableColumn row:(NSInteger)rowIndex
{
  id identifier = [aTableColumn identifier];

  NSDictionary* dict = [queue_ objectAtIndex:([queue_ count] - 1 - rowIndex)];
  return [dict objectForKey:identifier];
}

- (void) refresh
{
  [view_ reloadData];
  [view_ scrollRowToVisible:([queue_ count] - 1)];
}

- (void) push:(NSString*)eventType code:(NSString*)code name:(NSString*)name flags:(NSString*)flags misc:(NSString*)misc;
{
  NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:eventType, @"eventType",
                        code, @"code",
                        name, @"name",
                        flags, @"flags",
                        misc, @"misc",
                        nil];

  [queue_ insertObject:dict atIndex:0];
  if ([queue_ count] > MAXNUM) {
    [queue_ removeLastObject];
  }
  [self refresh];
}

- (IBAction) clear:(id)sender
{
  [queue_ removeAllObjects];
  [self refresh];
}

- (IBAction) copy:(id)sender
{
  NSPasteboard* pboard = [NSPasteboard generalPasteboard];
  NSMutableString* string = [[NSMutableString new] autorelease];

  for (NSUInteger i = 0; i < [queue_ count]; ++i) {
    NSDictionary* dict = [queue_ objectAtIndex:([queue_ count] - 1 - i)];
    [string appendFormat:@"eventType:%@\tcode:%@\tname:%@\tflags:%@\tmisc:%@\n",
     [dict objectForKey:@"eventType"],
     [dict objectForKey:@"code"],
     [dict objectForKey:@"name"],
     [dict objectForKey:@"flags"],
     [dict objectForKey:@"misc"]];
  }

  if ([string length] > 0) {
    [pboard clearContents];
    [pboard writeObjects:[NSArray arrayWithObject:string]];
  }
}

@end
