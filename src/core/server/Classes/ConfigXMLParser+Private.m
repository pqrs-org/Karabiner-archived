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

- (NSArray*) get_xml_paths
{
  return [NSArray arrayWithObjects:
          [NSArray arrayWithObjects:[self get_private_xml_path],
           [NSNumber numberWithInt:CONFIGXMLPARSER_XML_TYPE_CHECKBOX],
           [NSNumber numberWithInt:CONFIGXMLPARSER_XML_OWNER_USER],
           nil],
          [NSArray arrayWithObjects:@"/Library/org.pqrs/KeyRemap4MacBook/prefpane/checkbox.xml",
           [NSNumber numberWithInt:CONFIGXMLPARSER_XML_TYPE_CHECKBOX],
           [NSNumber numberWithInt:CONFIGXMLPARSER_XML_OWNER_SYSTEM],
           nil],
          [NSArray arrayWithObjects:@"/Library/org.pqrs/KeyRemap4MacBook/prefpane/number.xml",
           [NSNumber numberWithInt:CONFIGXMLPARSER_XML_TYPE_NUMBER],
           [NSNumber numberWithInt:CONFIGXMLPARSER_XML_OWNER_SYSTEM],
           nil],
          nil];
}

- (void) setErrorMessage:(NSException*)exception xmlpath:(NSString*)xmlpath
{
  // skip if an error was already reported.
  if (error_message_) return;

  error_message_ = [[NSString stringWithFormat:@"Error in %@\n\n%@\n\n%@", xmlpath, [exception name], [exception reason]] retain];
  NSAlert* alert = [NSAlert alertWithMessageText:@"KeyRemap4MacBook Error"
                                   defaultButton:@"Close"
                                 alternateButton:@"Open PreferencePane"
                                     otherButton:nil
                       informativeTextWithFormat:error_message_];
  NSInteger response = [alert runModal];
  if (response == NSAlertAlternateReturn) {
    [[NSWorkspace sharedWorkspace] openFile:@"/Library/PreferencePanes/KeyRemap4MacBook.prefPane"];
  }
}

- (void) removeErrorMessage
{
  if (error_message_) {
    [error_message_ release];
    error_message_ = nil;
  }
}

@end
