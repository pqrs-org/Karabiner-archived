#import "server_objc_part.h"

@implementation ServerObjcPart

- (NSArray*) getEssentialConfig
{
  return [preferencesmanager_ essential_config];
}

- (NSUInteger) getConfigCount
{
  return [configxmlparser_ count];
}

- (NSUInteger) getConfigInitializeVectorSize:(unsigned int)configindex
{
  return [configxmlparser_ initialize_vector_size:configindex];
}

- (NSArray*) getConfigInitializeVector:(unsigned int)configindex
{
  return [configxmlparser_ initialize_vector:configindex];
}

- (int) getConfigValue:(unsigned int)configindex
{
  NSString* name = [configxmlparser_ configname:configindex];
  if (! name) return 0;

  return [preferencesmanager_ value:name];
}

- (void) selectInputSource:(unsigned int)vk_keycode
{
  [workspacedata_ selectInputSource:vk_keycode];
}

- (void) setStatusMessage:(StatusMessageType)type message:(const char*)message
{
  NSString* s = [NSString stringWithCString:message encoding:NSUTF8StringEncoding];
  [statuswindow_ setStatusMessage:type message:s];
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
void
selectInputSource(uint32_t vk_keycode)
{
  if (! serverobjcpart) {
    NSLog(@"[WARNING] selectInputSource_language serverobjcpart == nil");
    return;
  }

  [serverobjcpart selectInputSource:vk_keycode];
}

// ------------------------------------------------------------
int
getEssentialConfig(int32_t* value, size_t len)
{
  if (! serverobjcpart) {
    NSLog(@"[WARNING] getEssentialConfig serverobjcpart == nil");
    return 1;
  }

  NSArray* essential_config = [serverobjcpart getEssentialConfig];
  if (! essential_config) {
    NSLog(@"[WARNING] getEssentialConfig essential_config == nil");
    return 1;
  }

  size_t i = 0;
  for (NSNumber* number in essential_config) {
    if (i < len) {
      value[i] = [number intValue];
      ++i;
    }
  }

  return 0;
}

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

void
set_statusmessage(StatusMessageType type, const char* message)
{
  if (serverobjcpart) {
    [serverobjcpart setStatusMessage:type message:message];
  } else {
    NSLog(@"[WARNING] set_statusmessage serverobjcpart == nil");
  }
}
