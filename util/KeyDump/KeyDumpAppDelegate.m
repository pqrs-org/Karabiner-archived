//
//  KeyDumpAppDelegate.m
//  KeyDump
//
//  Created by Takayama Fumihiko on 09/10/25.
//

#import "KeyDumpAppDelegate.h"

@implementation KeyDumpAppDelegate

@synthesize window;

- (void) applicationDidFinishLaunching:(NSNotification*)aNotification {
  NSTextView* textview = [_result documentView];
  [textview setSelectable:NO];
  [textview setEditable:NO];

  [window makeFirstResponder:_keyResponder];
}

- (IBAction) clear:(id)sender
{
  NSTextStorage* storage = [[_result documentView] textStorage];
  NSMutableString* originalstring = [storage mutableString];
  [storage beginEditing];
  [originalstring setString:@""];
  [storage endEditing];
}

- (IBAction) copy:(id)sender
{
  NSPasteboard* pboard = [NSPasteboard generalPasteboard];
  NSMutableString* string = [[[_result documentView] textStorage] mutableString];

  if ([string length] > 0) {
    [pboard clearContents];
    [pboard writeObjects:[NSArray arrayWithObject:string]];
  }
}

- (IBAction) quit:(id)sender
{
  [NSApp terminate:self];
}

@end
