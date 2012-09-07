/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#import "OtherInformationStore.h"
#include "bridge.h"

@implementation OtherInformationStore

- (IBAction) copy:(id)sender
{
  NSPasteboard* pboard = [NSPasteboard generalPasteboard];
  NSMutableString* string = [[NSMutableString new] autorelease];

  [string appendFormat:@"version: %@\n", version_];
  [string appendFormat:@"applicationName: %@\n", applicationname_];
  [string appendFormat:@"inputsourceName: %@\n", inputsourcename_];
  [string appendFormat:@"inputmodeName: %@\n", inputmodename_];

  [pboard clearContents];
  [pboard writeObjects:[NSArray arrayWithObject:string]];
}

- (void) setVersion
{
  version_ = [[client_ proxy] preferencepane_version];
  if (! version_) {
    version_ = @"-.-.-";
  }
  [label_version_ setStringValue:version_];
}

- (void) setApplicationName:(NSString*)name
{
  if (! name) {
    name = @"---";
  }

  applicationname_ = name;
  [label_applicationname_ setStringValue:name];
}

- (void) setInputSourceName:(NSString*)name
{
  if (! name) {
    name = @"---";
  }

  inputsourcename_ = name;
  [label_inputsourcename_ setStringValue:name];
}

- (void) setInputModeName:(NSString*)name
{
  if (! name) {
    name = @"---";
  }

  inputmodename_ = name;
  [label_inputmodename_ setStringValue:name];
}

- (void) setDeviceInformation
{
  NSInteger types[] = {
    BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_KEYBOARD,
    BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_CONSUMER,
    BRIDGE_USERCLIENT_TYPE_GET_DEVICE_INFORMATION_POINTING,
  };
  for (size_t i = 0; i < sizeof(types) / sizeof(types[0]); ++i) {
    NSArray* information = [[client_ proxy] device_information:types[i]];
    for (NSDictionary* d in information) {
      NSLog(@"%@ %@ %@ %@ %@",
            [d objectForKey:@"manufacturer"],
            [d objectForKey:@"product"],
            [d objectForKey:@"vendorID"],
            [d objectForKey:@"productID"],
            [d objectForKey:@"locationID"]);

    }
  }
}

@end
