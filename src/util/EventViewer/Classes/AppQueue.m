/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "AppQueue.h"

@implementation AppQueue

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
  if ([queue_ count] == 0) {
    return 1;
  }

  return [queue_ count];
}

- (id) tableView:(NSTableView*)aTableView objectValueForTableColumn:(NSTableColumn*)aTableColumn row:(NSInteger)rowIndex
{
  NSString* identifier = [aTableColumn identifier];

  if ([queue_ count] == 0) {
    return @"Please activate the other applications.";
  }

  NSDictionary* dict = queue_[([queue_ count] - 1 - rowIndex)];
  return dict[identifier];
}

- (void) refresh
{
  [view_ reloadData];
  [view_ scrollRowToVisible:([queue_ count] - 1)];
}

- (void) push:(NSString*)applicationBundleIdentifier
{
  NSDictionary* dict = @{ @"applicationBundleIdentifier": applicationBundleIdentifier,
                          @"date": [[NSDate date] description] };

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
    NSDictionary* dict = queue_[([queue_ count] - 1 - i)];

    [string appendFormat:@"%@\n", dict[@"applicationBundleIdentifier"]];
  }

  if ([string length] > 0) {
    [pboard clearContents];
    [pboard writeObjects:@[string]];
  }
}

@end
