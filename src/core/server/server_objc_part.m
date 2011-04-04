#import "server_objc_part.h"

@implementation ServerObjcPart

- (NSUInteger) getConfigCount
{
  return [[ConfigXMLParser getInstance] count];
}

- (NSUInteger) getConfigInitializeVectorSize:(unsigned int)configindex
{
  return [[ConfigXMLParser getInstance] initialize_vector_size:configindex];
}

- (NSArray*) getConfigInitializeVector:(unsigned int)configindex
{
  return [[ConfigXMLParser getInstance] initialize_vector:configindex];
}

- (int) getConfigValue:(unsigned int)configindex
{
  NSString* name = [[ConfigXMLParser getInstance] configname:configindex];
  if (! name) return 0;

  return [[PreferencesManager getInstance] value:name];
}

@end

// ======================================================================
static ServerObjcPart * serverobjcpart = nil;

void
registerServerObjcPart(ServerObjcPart* object)
{
  serverobjcpart = object;
}

// ------------------------------------------------------------
uint32_t
getConfigCount(void)
{
  if (serverobjcpart) {
    return (uint32_t)([serverobjcpart getConfigCount]);
  } else {
    NSLog(@"[WARNING] getConfigCount serverobjcpart == nil");
    return 0;
  }
}

uint32_t
getConfigInitializeVectorSize(uint32_t configindex)
{
  if (serverobjcpart) {
    return (uint32_t)([serverobjcpart getConfigInitializeVectorSize:configindex]);
  } else {
    NSLog(@"[WARNING] getConfigInitializeVectorSize serverobjcpart == nil");
    return 0;
  }
}

int
getConfigInitializeVector(uint32_t* value, size_t len, uint32_t configindex)
{
  if (! serverobjcpart) {
    NSLog(@"[WARNING] getConfigInitializeVector serverobjcpart == nil");
    return 1;
  }

  NSArray* initialize_vector = [serverobjcpart getConfigInitializeVector:configindex];
  if (! initialize_vector) {
    NSLog(@"[WARNING] getConfigInitializeVector initialize_vector == nil");
    return 1;
  }

  size_t i = 0;
  for (NSNumber* number in initialize_vector) {
    if (i < len) {
      value[i] = [number unsignedIntValue];
      ++i;
    }
  }

  return 0;
}

int
getConfigValue(uint32_t configindex)
{
  if (serverobjcpart) {
    return (uint32_t)([serverobjcpart getConfigValue:configindex]);
  } else {
    NSLog(@"[WARNING] getConfigValue serverobjcpart == nil");
    return 0;
  }
}
