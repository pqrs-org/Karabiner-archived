#import "KeyRemap4MacBookKeys.h"
#import "ServerForUserspace.h"

@implementation ServerForUserspace

- (id) init
{
  self = [super init];

  if (self) {
    connection_ = [NSConnection new];
    [connection_ setRootObject:self];
    if (! [connection_ registerName:kKeyRemap4MacBookConnectionName]) {
      // Quit when registerName is failed.
      // We wait 2 second before quit to avoid consecutive restarting from launchd.
      NSLog(@"[NSConnection registerName] is failed. Restarting process.");
      [NSThread sleepForTimeInterval:2];
      [NSApp terminate:nil];
    }
  }

  return self;
}

- (void) dealloc
{
  [connection_ release];

  [super dealloc];
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
- (NSInteger) checkForUpdatesMode
{
  return [preferencesManager_ checkForUpdatesMode];
}

- (void) setCheckForUpdatesMode:(NSInteger)newval
{
  return [preferencesManager_ setCheckForUpdatesMode:newval];
}

// ----------------------------------------------------------------------
- (void) configxml_reload
{
  [xmlCompiler_ reload];
}

- (NSArray*) preferencepane_checkbox
{
  return [xmlCompiler_ preferencepane_checkbox];
}

- (NSArray*) preferencepane_number
{
  return [xmlCompiler_ preferencepane_number];
}

- (int) enabled_count:(NSArray*)checkbox changed:(NSDictionary*)changed
{
  int count = 0;

  if (checkbox) {
    for (NSDictionary* dict in checkbox) {
      NSString* identifier = [dict objectForKey:@"identifier"];
      if (identifier) {
        if ([[changed objectForKey:identifier] intValue] != 0) {
          ++count;
        }
      }

      count += [self enabled_count:[dict objectForKey:@"children"] changed:changed];
    }
  }

  return count;
}

- (int) preferencepane_enabled_count
{
  NSArray* checkbox = [xmlCompiler_ preferencepane_checkbox];
  NSDictionary* changed = [self changed];
  return [self enabled_count:checkbox changed:changed];
}

- (NSString*) preferencepane_error_message
{
  return [xmlCompiler_ preferencepane_error_message];
}

- (NSString*) preferencepane_get_private_xml_path
{
  return [XMLCompiler get_private_xml_path];
}

- (NSString*) preferencepane_version
{
  return [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"];
}

- (NSArray*) device_information:(NSInteger)type
{
  NSMutableArray* information = [[NSMutableArray new] autorelease];

  for (size_t i = 0;; ++i) {
    struct BridgeDeviceInformation deviceInformation;

    struct BridgeUserClientStruct bridgestruct;
    bridgestruct.type   = (uint32_t)(type);
    bridgestruct.option = i;
    bridgestruct.data   = (uintptr_t)(&deviceInformation);
    bridgestruct.size   = sizeof(deviceInformation);

    if (! [userClient_userspace_ synchronized_communication:&bridgestruct]) break;

    if (! deviceInformation.isFound) break;

    NSDictionary* newdict = [NSDictionary dictionaryWithObjectsAndKeys:
                             [NSString stringWithUTF8String:deviceInformation.manufacturer], @"manufacturer",
                             [NSString stringWithUTF8String:deviceInformation.product], @"product",
                             [NSString stringWithFormat:@"0x%x", deviceInformation.vendorID], @"vendorID",
                             [NSString stringWithFormat:@"0x%x", deviceInformation.productID], @"productID",
                             [NSString stringWithFormat:@"0x%x", deviceInformation.locationID], @"locationID",
                             nil];

    // skip if newdict is already exists.
    BOOL found = NO;
    for (NSDictionary* d in information) {
      if ([newdict isEqualToDictionary:d]) {
        found = YES;
      }
    }
    if (! found) {
      [information addObject:newdict];
    }
  }

  return information;
}

@end
