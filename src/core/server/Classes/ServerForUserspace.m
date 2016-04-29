#import "ServerForUserspace.h"
#import "AXNotifierManager.h"
#import "AppDelegate.h"
#import "ClientForKernelspace.h"
#import "NotificationKeys.h"
#import "PreferencesKeys.h"
#import "PreferencesManager.h"
#import "PreferencesModel.h"
#import "Relauncher.h"
#import "ServerController.h"
#import "SharedKeys.h"
#import "XMLCompiler.h"

@interface ServerForUserspace ()

@property(weak) IBOutlet AppDelegate* appDelegate;
@property(weak) IBOutlet AXNotifierManager* axNotifierManager;
@property(weak) IBOutlet ClientForKernelspace* clientForKernelspace;
@property(weak) IBOutlet PreferencesManager* preferencesManager;
@property(weak) IBOutlet PreferencesModel* preferencesModel;
@property(weak) IBOutlet ServerController* serverController;
@property(weak) IBOutlet XMLCompiler* xmlCompiler;
@property NSConnection* connection;

@end

@implementation ServerForUserspace

- (instancetype)init {
  self = [super init];

  if (self) {
    self.connection = [NSConnection new];
  }

  return self;
}

- (BOOL)registerService {
  [self.connection setRootObject:self];
  if (![self.connection registerName:kKarabinerConnectionName]) {
    return NO;
  }
  return YES;
}

// ----------------------------------------------------------------------
- (NSString*)bundleVersion {
  return [[NSBundle mainBundle] infoDictionary][@"CFBundleVersion"];
}

- (void)loadPreferencesModel:(PreferencesModel*)preferencesModel {
  [self.preferencesManager loadPreferencesModel:preferencesModel];
}

- (void)savePreferencesModel:(PreferencesModel*)preferencesModel processIdentifier:(int)processIdentifier {
  [self.preferencesManager savePreferencesModel:preferencesModel processIdentifier:processIdentifier];
}

- (void)updateStartAtLogin {
  [self.serverController updateStartAtLogin:YES];
}

- (void)updateStatusBar {
  [[NSNotificationCenter defaultCenter] postNotificationName:kStatusBarConfigurationChangedNotification object:nil];
}

- (void)restartAXNotifier {
  [self.axNotifierManager restartAXNotifier];
}

- (void)terminateServerProcess {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self.serverController terminateServerProcess];
  });
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
- (int)value:(NSString*)name {
  return [self.preferencesManager value:name];
}

- (int)defaultValue:(NSString*)name {
  return [self.preferencesManager defaultValue:name];
}

- (void)setValue:(int)newval forName:(NSString*)name {
  [self.preferencesManager setValue:newval forName:name];
}

- (NSDictionary*)changed {
  return [self.preferencesManager changed];
}

// ----------------------------------------------------------------------
- (NSInteger)configlist_selectedIndex {
  return [self.preferencesManager configlist_selectedIndex];
}

- (NSArray*)configlist_getConfigList {
  return [self.preferencesManager configlist_getConfigList];
}

- (void)configlist_select:(NSInteger)newIndex {
  [self.preferencesManager configlist_select:newIndex];
}

- (void)configlist_setName:(NSInteger)rowIndex name:(NSString*)name {
  [self.preferencesManager configlist_setName:rowIndex name:name];
}

- (void)configlist_append {
  [self.preferencesManager configlist_append];
}

- (void)configlist_delete:(NSInteger)rowIndex {
  [self.preferencesManager configlist_delete:rowIndex];
}

- (void)configlist_clear_all_values:(NSInteger)rowIndex {
  [self.preferencesManager configlist_clear_all_values:rowIndex];
}

// ----------------------------------------------------------------------
- (void)configxml_reload {
  [self.xmlCompiler reload];
}

- (NSString*)symbolMapName:(NSString*)type value:(NSInteger)value {
  return [self.xmlCompiler symbolMapName:type value:(uint32_t)(value)];
}

// ----------------------------------------------------------------------
- (NSDictionary*)preferencesForAXNotifier {
  return @{
    @"kAXNotifierDisabledInJavaApps" : @(self.preferencesModel.disableAXNotifierInJavaApps),
    @"kAXNotifierDisabledInQtApps" : @([[NSUserDefaults standardUserDefaults] boolForKey:kAXNotifierDisabledInQtApps]),
    @"kAXNotifierDisabledInPreview" : @([[NSUserDefaults standardUserDefaults] boolForKey:kAXNotifierDisabledInPreview]),
    @"kAXNotifierDisabledInMicrosoftOffice" : @([[NSUserDefaults standardUserDefaults] boolForKey:kAXNotifierDisabledInMicrosoftOffice]),
  };
}

- (void)updateFocusedUIElementInformation:(NSDictionary*)information {
  return [self.appDelegate updateFocusedUIElementInformation:information];
}

// ----------------------------------------------------------------------
- (NSArray*)device_information:(NSInteger)type {
  return [self.clientForKernelspace device_information:type];
}

- (NSDictionary*)focused_uielement_information {
  return self.appDelegate.focusedUIElementInformation;
}

- (NSArray*)workspace_app_ids {
  return self.appDelegate.workspaceAppIds;
}

- (NSArray*)workspace_window_name_ids {
  return self.appDelegate.workspaceWindowNameIds;
}

- (NSArray*)workspace_inputsource_ids {
  return self.appDelegate.workspaceInputSourceIds;
}

- (NSDictionary*)inputsource_information {
  return self.appDelegate.inputSourceInformation;
}

@end
