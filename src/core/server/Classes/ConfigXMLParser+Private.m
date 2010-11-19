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

- (void) setErrorMessageFromNSError:(NSError*)error
{
  error_message_ = [error localizedDescription];
  NSLog(@"%@", error_message_);
}

- (void) setErrorMessageFromNSString:(NSString*)string
{
  error_message_ = string;
  NSLog(@"%@", error_message_);
}

@end
