#import "ConfigXMLParser.h"

@implementation ConfigXMLParser (Private)

- (NSString*) get_private_xml_path
{
  NSFileManager* filemanager = [NSFileManager defaultManager];
  NSArray* paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
  NSString* path = [paths objectAtIndex:0];
  path = [path stringByAppendingPathComponent:@"KeyRemap4MacBook"];
  if (! [filemanager fileExistsAtPath:path]) {
    [filemanager createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:NULL];
  }
  if ([filemanager fileExistsAtPath:path]) {
    path = [path stringByAppendingPathComponent:@"private.xml"];
  }

  BOOL isDirectory;
  if ([filemanager fileExistsAtPath:path isDirectory:&isDirectory] && ! isDirectory) {
    return path;
  }

  return @"";
}

- (void) setErrorMessage:(NSException*)exception
{
  // skip if an error was already reported.
  if (error_message_) return;

  error_message_ = [NSString stringWithFormat:@"%@ %@", [exception name], [exception reason]];
  [[NSAlert alertWithMessageText:@"KeyRemap4MacBook" defaultButton:nil alternateButton:nil otherButton:nil informativeTextWithFormat:error_message_] runModal];
}

@end
