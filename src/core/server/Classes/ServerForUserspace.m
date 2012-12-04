#import "ClientForKernelspace.h"
#import "KeyRemap4MacBookKeys.h"
#import "KeyRemap4MacBookNSDistributedNotificationCenter.h"
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

  // Other apps which are connected to KeyRemap4MacBook (Prefs, EventViewer, ...) should reconnect.
  // So, sending notification as soon as possible.
  [org_pqrs_KeyRemap4MacBook_NSDistributedNotificationCenter postNotificationName:kKeyRemap4MacBookServerLaunchedNotification userInfo:nil];
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

- (void) setValueForName:(int)newval forName:(NSString*)name
{
  [preferencesManager_ setValueForName:newval forName:name];
}

- (NSArray*) essential_config
{
  return [preferencesManager_ essential_config];
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

- (NSString*) configlist_selectedName
{
  return [preferencesManager_ configlist_selectedName];
}

- (NSString*) configlist_selectedIdentifier
{
  return [preferencesManager_ configlist_selectedIdentifier];
}

- (NSArray*) configlist_getConfigList
{
  return [preferencesManager_ configlist_getConfigList];
}

- (NSUInteger) configlist_count
{
  return [preferencesManager_ configlist_count];
}

- (NSDictionary*) configlist_dictionary:(NSInteger)rowIndex
{
  return [preferencesManager_ configlist_dictionary:rowIndex];
}

- (NSString*) configlist_name:(NSInteger)rowIndex
{
  return [preferencesManager_ configlist_name:rowIndex];
}

- (NSString*) configlist_identifier:(NSInteger)rowIndex
{
  return [preferencesManager_ configlist_identifier:rowIndex];
}

- (void) configlist_select:(NSInteger)newIndex
{
  [preferencesManager_ configlist_select:newIndex];
}

- (void) configlist_setName:(NSInteger)rowIndex name:(NSString*)name
{
  [preferencesManager_ configlist_setName:rowIndex name:name];
}

- (void) configlist_append
{
  [preferencesManager_ configlist_append];
}

- (void) configlist_delete:(NSInteger)rowIndex
{
  [preferencesManager_ configlist_delete:rowIndex];
}

- (BOOL) isStatusbarEnable
{
  return [preferencesManager_ isStatusbarEnable];
}

- (BOOL) isShowSettingNameInStatusBar
{
  return [preferencesManager_ isShowSettingNameInStatusBar];
}

- (void) toggleStatusbarEnable
{
  return [preferencesManager_ toggleStatusbarEnable];
}

- (void) toggleShowSettingNameInStatusBar
{
  return [preferencesManager_ toggleShowSettingNameInStatusBar];
}

// ----------------------------------------------------------------------
- (void) configxml_reload
{
  [xmlCompiler_ reload];
}

- (NSArray*) device_information:(NSInteger)type
{
  return [clientForKernelspace_ device_information:type];
}

@end
