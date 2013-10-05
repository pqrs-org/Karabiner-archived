#import "AppDelegate.h"
#import "ClientForKernelspace.h"
#import "KeyRemap4MacBookKeys.h"
#import "PreferencesManager.h"
#import "ServerForUserspace.h"
#import "Updater.h"
#import "UserClient_userspace.h"
#import "XMLCompiler.h"

@implementation ServerForUserspace

- (id) init
{
  self = [super init];

  if (self) {
    connection_ = [NSConnection new];
  }

  return self;
}

- (void) dealloc
{
  [connection_ release];

  [super dealloc];
}

// ----------------------------------------------------------------------
- (BOOL) register
{
  [connection_ setRootObject:self];
  if (! [connection_ registerName:kKeyRemap4MacBookConnectionName]) {
    return NO;
  }
  return YES;
}

// ----------------------------------------------------------------------
- (int) value:(NSString*)name
{
  return [preferencesManager_ value:name];
}

- (int) defaultValue:(NSString*)name
{
  return [preferencesManager_ defaultValue:name];
}

- (void) setValue:(int)newval forName:(NSString*)name
{
  [preferencesManager_ setValue:newval forName:name];
}

- (NSDictionary*) changed
{
  return [preferencesManager_ changed];
}

// ----------------------------------------------------------------------
- (NSInteger) configlist_selectedIndex
{
  return [preferencesManager_ configlist_selectedIndex];
}

- (NSArray*) configlist_getConfigList
{
  return [preferencesManager_ configlist_getConfigList];
}

- (void) configlist_select:(NSInteger)newIndex
{
  [preferencesManager_ configlist_select:newIndex];
}

// ----------------------------------------------------------------------
- (void) configxml_reload
{
  [xmlCompiler_ reload];
}

- (NSString*) symbolMapName:(NSString*)type value:(NSInteger)value
{
  return [xmlCompiler_ symbolMapName:type value:(uint32_t)(value)];
}

- (NSArray*) device_information:(NSInteger)type
{
  return [clientForKernelspace_ device_information:type];
}

- (NSDictionary*) application_information
{
  return [appDelegate_ getApplicationInformation];
}

- (NSDictionary*) inputsource_information
{
  return [appDelegate_ getInputSourceInformation];
}

@end
