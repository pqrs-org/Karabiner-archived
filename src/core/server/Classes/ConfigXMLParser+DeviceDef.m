#import "ConfigXMLParser.h"

@implementation ConfigXMLParserDeviceDefData

@synthesize name = name_;

- (id) init
{
  self = [super init];

  if (self) {
    value_ = 0;
  }

  return self;
}

- (void) dealloc
{
  [name_ release];

  [super dealloc];
}

- (void) setName:(NSString*)newname;
{
  [name_ release];
  name_ = [newname retain];
}

- (void) setValue:(NSInteger)newvalue;
{
  value_ = newvalue;
}

- (NSInteger) value
{
  return value_;
}

@end

@implementation ConfigXMLParser (DeviceDef)

- (ConfigXMLParserDeviceDefData*) parse_devicedef:(NSXMLElement*)devicedefElement
{
  ConfigXMLParserDeviceDefData* newdata = [[ConfigXMLParserDeviceDefData new] autorelease];

  NSUInteger count = [devicedefElement childCount];
  for (NSUInteger i = 0; i < count; ++i) {
    NSXMLElement* e = [self castToNSXMLElement:[devicedefElement childAtIndex:i]];
    if (! e) continue;

    NSString* name = [e name];

    if ([name isEqualToString:@"vendorname"] ||
        [name isEqualToString:@"productname"]) {
      [newdata setName:[e stringValue]];

    } else if ([name isEqualToString:@"vendorid"] ||
               [name isEqualToString:@"productid"]) {
      NSScanner* scanner = [NSScanner scannerWithString:[e stringValue]];
      unsigned int hexvalue = 0;
      NSInteger integervalue = 0;
      if ([scanner scanHexInt:&hexvalue]) {
        [newdata setValue:hexvalue];
      } else if ([scanner scanInteger:&integervalue]) {
        [newdata setValue:integervalue];
      } else {
        @throw [NSException exceptionWithName :[NSString stringWithFormat:@"<%@> error", name]
                reason :[NSString stringWithFormat:@"%@ is invalid value for <%@>", [e stringValue], name]
                userInfo : nil];
      }
    }
  }

  if (! newdata.name) return nil;

  return newdata;
}

- (void) traverse_devicedef:(NSXMLElement*)element
{
  NSUInteger count = [element childCount];
  for (NSUInteger i = 0; i < count; ++i) {
    NSXMLElement* e = [self castToNSXMLElement:[element childAtIndex:i]];
    if (! e) continue;

    if ([[e name] isEqualToString:@"devicevendordef"] ||
        [[e name] isEqualToString:@"deviceproductdef"]) {
      NSString* type = nil;
      if ([[e name] isEqualToString:@"devicevendordef"]) {
        type = @"DeviceVendor";
      } else if ([[e name] isEqualToString:@"deviceproductdef"]) {
        type = @"DeviceProduct";
      }

      if (type) {
        ConfigXMLParserDeviceDefData* newdata = [self parse_devicedef:e];
        if (newdata) {
          [appdefdata_ addObject:newdata];

          // Adding to keycode_ if needed.
          NSString* keycodename = [NSString stringWithFormat:@"%@::%@", type, newdata.name];
          if (! [keycode_ isExists:keycodename]) {
            [keycode_ append:keycodename newvalue:(unsigned int)([newdata value])];
          }
        }
      }

    } else {
      [self traverse_devicedef:e];
    }
  }
}

- (BOOL) reload_devicedef {
  BOOL retval = NO;

  NSArray* paths = [NSArray arrayWithObjects:
                    [self preferencepane_get_private_xml_path],
                    [[NSBundle mainBundle] pathForResource:@"devicevendordef" ofType:@"xml"],
                    [[NSBundle mainBundle] pathForResource:@"deviceproductdef" ofType:@"xml"],
                    nil];
  for (NSString* xmlpath in paths) {
    @try {
      if ([xmlpath length] == 0) continue;

      NSURL* url = [NSURL fileURLWithPath:xmlpath];
      NSError* error = nil;
      NSXMLDocument* xmldocument = [[[NSXMLDocument alloc] initWithContentsOfURL:url options:0 error:&error] autorelease];
      if (! xmldocument) {
        @throw [NSException exceptionWithName :[NSString stringWithFormat:@"%@ is invalid", xmlpath] reason :[error localizedDescription] userInfo : nil];
      }

      [self traverse_devicedef:[xmldocument rootElement]];

      // Set retval to YES if only one XML file is loaded successfully.
      // Unless we do it, all setting becomes disabled by one error.
      // (== If private.xml is invalid, system wide device*def.xml is not loaded.)
      retval = YES;

    } @catch (NSException* exception) {
      [self setErrorMessage:exception xmlpath:xmlpath];
    }
  }

  return retval;
}

@end
