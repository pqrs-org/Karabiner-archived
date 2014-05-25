#import "AppDelegate.h"
#import "ClientForKernelspace.h"
#import "KeyRemap4MacBookKeys.h"
#import "PreferencesManager.h"
#import "Relauncher.h"
#import "ServerForUserspace.h"
#import "Updater.h"
#import "UserClient_userspace.h"
#import "XMLCompiler.h"

@interface ServerForUserspace ()
{
  NSConnection* connection_;
}
@end

@implementation ServerForUserspace

- (id) init
{
  self = [super init];

  if (self) {
    connection_ = [NSConnection new];
  }

  return self;
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

- (void) relaunch
{
  // Use dispatch_async in order to avoid "disconnected from server".
  //
  // Example error message of disconnection:
  //   "KeyRemap4MacBook_cli: connection went invalid while waiting for a reply because a mach port died"
  dispatch_async(dispatch_get_main_queue(), ^{
    [Relauncher relaunch];
  });
}

// ----------------------------------------------------------------------
- (void) updateFocusedUIElementInformation:(NSDictionary*)information
{
  return [appDelegate_ updateFocusedUIElementInformation:information];
}

// ----------------------------------------------------------------------
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
