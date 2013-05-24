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
    queue_ = [NSMutableArray new];
  }

  return self;
}

- (void) dealloc
{
  [queue_ release];

  [super dealloc];
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

    NSString* eventType = [NSString stringWithFormat:@"eventType:%@", [dict objectForKey:@"eventType"]];
    NSString* code      = [NSString stringWithFormat:@"code:%@",      [dict objectForKey:@"code"]];
    NSString* name      = [NSString stringWithFormat:@"name:%@",      [dict objectForKey:@"name"]];
    NSString* flags     = [NSString stringWithFormat:@"flags:%@",     [dict objectForKey:@"flags"]];
    NSString* misc      = [NSString stringWithFormat:@"misc:%@",      [dict objectForKey:@"misc"]];

    [string appendFormat:@"%@ %@ %@ %@ %@\n",
     [eventType stringByPaddingToLength:25 withString:@" " startingAtIndex:0],
     [code stringByPaddingToLength:15 withString:@" " startingAtIndex:0],
     [name stringByPaddingToLength:20 withString:@" " startingAtIndex:0],
     [flags stringByPaddingToLength:40 withString:@" " startingAtIndex:0],
     misc];
  }

  if ([string length] > 0) {
    [pboard clearContents];
    [pboard writeObjects:[NSArray arrayWithObject:string]];
  }
}

@end
