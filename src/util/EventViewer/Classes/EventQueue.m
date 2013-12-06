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


- (NSInteger) numberOfRowsInTableView:(NSTableView*)aTableView
{
  return [queue_ count];
}

- (id) tableView:(NSTableView*)aTableView objectValueForTableColumn:(NSTableColumn*)aTableColumn row:(NSInteger)rowIndex
{
  id identifier = [aTableColumn identifier];

  NSDictionary* dict = queue_[([queue_ count] - 1 - rowIndex)];
  return dict[identifier];
}

- (void) refresh
{
  [view_ reloadData];
  [view_ scrollRowToVisible:([queue_ count] - 1)];
}

- (void) push:(NSString*)eventType code:(NSString*)code name:(NSString*)name flags:(NSString*)flags misc:(NSString*)misc;
{
  NSDictionary* dict = @{ @"eventType": eventType,
                          @"code": code,
                          @"name": name,
                          @"flags": flags,
                          @"misc": misc };

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
  NSMutableString* string = [NSMutableString new];

  for (NSUInteger i = 0; i < [queue_ count]; ++i) {
    NSDictionary* dict = queue_[([queue_ count] - 1 - i)];

    NSString* eventType = [NSString stringWithFormat:@"eventType:%@", dict[@"eventType"]];
    NSString* code      = [NSString stringWithFormat:@"code:%@",      dict[@"code"]];
    NSString* name      = [NSString stringWithFormat:@"name:%@",      dict[@"name"]];
    NSString* flags     = [NSString stringWithFormat:@"flags:%@",     dict[@"flags"]];
    NSString* misc      = [NSString stringWithFormat:@"misc:%@",      dict[@"misc"]];

    [string appendFormat:@"%@ %@ %@ %@ %@\n",
     [eventType stringByPaddingToLength:25 withString:@" " startingAtIndex:0],
     [code stringByPaddingToLength:15 withString:@" " startingAtIndex:0],
     [name stringByPaddingToLength:20 withString:@" " startingAtIndex:0],
     [flags stringByPaddingToLength:40 withString:@" " startingAtIndex:0],
     misc];
  }

  if ([string length] > 0) {
    [pboard clearContents];
    [pboard writeObjects:@[string]];
  }
}

@end
