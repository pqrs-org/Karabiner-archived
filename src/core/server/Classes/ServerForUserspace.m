#import "AppDelegate.h"
#import "ClientForKernelspace.h"
#import "KarabinerKeys.h"
#import "PreferencesKeys.h"
#import "PreferencesManager.h"
#import "Relauncher.h"
#import "ServerForUserspace.h"
#import "Updater.h"
#import "XMLCompiler.h"

@interface ServerForUserspace () {
  NSConnection* connection_;
}
@end

@implementation ServerForUserspace

- (id)init {
  self = [super init];

  if (self) {
    connection_ = [NSConnection new];
  }

  return self;
}

// ----------------------------------------------------------------------
- (BOOL) register {
  [connection_ setRootObject:self];
  if (![connection_ registerName:kKarabinerConnectionName]) {
    return NO;
  }
  return YES;
}

// ----------------------------------------------------------------------
- (int)value:(NSString*)name {
  return [preferencesManager_ value:name];
}

- (int)defaultValue:(NSString*)name {
  return [preferencesManager_ defaultValue:name];
}

- (void)setValue:(int)newval forName:(NSString*)name {
  [preferencesManager_ setValue:newval forName:name];
}

- (NSDictionary*)changed {
  return [preferencesManager_ changed];
}

// ----------------------------------------------------------------------
- (NSInteger)configlist_selectedIndex {
  return [preferencesManager_ configlist_selectedIndex];
}

- (NSArray*)configlist_getConfigList {
  return [preferencesManager_ configlist_getConfigList];
}

- (void)configlist_select:(NSInteger)newIndex {
  [preferencesManager_ configlist_select:newIndex];
}

- (void)configlist_setName:(NSInteger)rowIndex name:(NSString*)name {
  [preferencesManager_ configlist_setName:rowIndex name:name];
}

- (void)configlist_append {
  [preferencesManager_ configlist_append];
}

- (void)configlist_delete:(NSInteger)rowIndex {
  [preferencesManager_ configlist_delete:rowIndex];
}

- (void)configlist_clear_all_values:(NSInteger)rowIndex {
  [preferencesManager_ configlist_clear_all_values:rowIndex];
}

// ----------------------------------------------------------------------
- (void)configxml_reload {
  [xmlCompiler_ reload];
}

- (NSString*)symbolMapName:(NSString*)type value:(NSInteger)value {
  return [xmlCompiler_ symbolMapName:type value:(uint32_t)(value)];
}

- (void)relaunch {
  // Use dispatch_async in order to avoid "disconnected from server".
  //
  // Example error message of disconnection:
  //   "karabiner: connection went invalid while waiting for a reply because a mach port died"
  dispatch_async(dispatch_get_main_queue(), ^{
    [Relauncher relaunch];
  });
}

// ----------------------------------------------------------------------
- (NSDictionary*)preferencesForAXNotifier {
  return @{
    @"kAXNotifierDisabledInJavaApps" : @([[NSUserDefaults standardUserDefaults] boolForKey:kAXNotifierDisabledInJavaApps]),
    @"kAXNotifierDisabledInQtApps" : @([[NSUserDefaults standardUserDefaults] boolForKey:kAXNotifierDisabledInQtApps]),
    @"kAXNotifierDisabledInPreview" : @([[NSUserDefaults standardUserDefaults] boolForKey:kAXNotifierDisabledInPreview]),
    @"kAXNotifierDisabledInMicrosoftOffice" : @([[NSUserDefaults standardUserDefaults] boolForKey:kAXNotifierDisabledInMicrosoftOffice]),
  };
}

- (void)updateFocusedUIElementInformation:(NSDictionary*)information {
  return [appDelegate_ updateFocusedUIElementInformation:information];
}

// ----------------------------------------------------------------------
- (NSArray*)device_information:(NSInteger)type {
  return [clientForKernelspace_ device_information:type];
}

- (NSDictionary*)focused_uielement_information {
  return [appDelegate_ getFocusedUIElementInformation];
}

- (NSArray*)workspace_app_ids {
  return [appDelegate_ getWorkspaceAppIds];
}

- (NSArray*)workspace_window_name_ids {
  return [appDelegate_ getWorkspaceWindowNameIds];
}

- (NSArray*)workspace_inputsource_ids {
  return [appDelegate_ getWorkspaceInputSourceIds];
}

- (NSDictionary*)inputsource_information {
  return [appDelegate_ getInputSourceInformation];
}

@end
