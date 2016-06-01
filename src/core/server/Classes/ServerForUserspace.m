#import "ServerForUserspace.h"
#import "AXNotifierManager.h"
#import "AppDelegate.h"
#import "AppLauncher.h"
#import "ClientForKernelspace.h"
#import "NotificationKeys.h"
#import "PreferencesKeys.h"
#import "PreferencesManager.h"
#import "PreferencesModel.h"
#import "Relauncher.h"
#import "ServerController.h"
#import "SharedKeys.h"
#import "StatusBar.h"
#import "StatusMessageManager.h"
#import "UpdaterController.h"
#import "XMLCompiler.h"
#import "weakify.h"

@interface ServerForUserspace ()

@property(weak) IBOutlet AppDelegate* appDelegate;
@property(weak) IBOutlet AXNotifierManager* axNotifierManager;
@property(weak) IBOutlet ClientForKernelspace* clientForKernelspace;
@property(weak) IBOutlet PreferencesManager* preferencesManager;
@property(weak) IBOutlet PreferencesModel* preferencesModel;
@property(weak) IBOutlet ServerController* serverController;
@property(weak) IBOutlet StatusBar* statusBar;
@property(weak) IBOutlet StatusMessageManager* statusMessageManager;
@property(weak) IBOutlet XMLCompiler* xmlCompiler;
@property NSConnection* connection;

@end

@implementation ServerForUserspace

- (instancetype)init {
  self = [super init];

  if (self) {
    _connection = [NSConnection new];
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

- (bycopy AXNotifierPreferencesModel*)axNotifierPreferencesModel {
  return self.preferencesModel.axNotifier;
}

- (void)savePreferencesModel:(bycopy PreferencesModel*)preferencesModel processIdentifier:(int)processIdentifier {
  @weakify(self);

  dispatch_async(dispatch_get_main_queue(), ^{
    @strongify(self);
    if (!self) return;

    [self.preferencesManager savePreferencesModel:preferencesModel processIdentifier:processIdentifier];
  });
}

- (void)updateKextValue:(NSString*)name {
  @weakify(self);

  // We should use main queue in order to keep call order of ServerClientProtocol methods.
  dispatch_async(dispatch_get_main_queue(), ^{
    @strongify(self);
    if (!self) return;

    [self.preferencesManager updateKextValue:name];
  });
}

- (void)updateKextValues {
  @weakify(self);

  // We should use main queue in order to keep call order of ServerClientProtocol methods.
  dispatch_async(dispatch_get_main_queue(), ^{
    @strongify(self);
    if (!self) return;

    [self.clientForKernelspace send_config_to_kext];
  });
}

- (void)updateStartAtLogin {
  [self.serverController updateStartAtLogin:YES];
}

- (void)updateStatusBar {
  @weakify(self);

  dispatch_async(dispatch_get_main_queue(), ^{
    @strongify(self);
    if (!self) return;

    [self.statusBar refresh];
  });
}

- (void)updateStatusWindow {
  @weakify(self);

  dispatch_async(dispatch_get_main_queue(), ^{
    @strongify(self);
    if (!self) return;

    [self.statusMessageManager refresh];
  });
}

- (void)restartAXNotifier {
  [self.axNotifierManager restartAXNotifier];
}

- (void)unsetDebugFlags {
  [self.clientForKernelspace unset_debug_flags];
}

- (void)terminateServerProcess {
  @weakify(self);

  dispatch_async(dispatch_get_main_queue(), ^{
    @strongify(self);
    if (!self) return;

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

- (void)checkForUpdatesStableOnly {
  [UpdaterController checkForUpdatesStableOnly];
}

- (void)checkForUpdatesWithBetaVersion {
  [UpdaterController checkForUpdatesWithBetaVersion];
}

- (void)reloadXML {
  [self.xmlCompiler reload];
}

- (void)openEventViewer {
  [AppLauncher openEventViewer];
}

- (void)openMultiTouchExtension {
  [AppLauncher openMultiTouchExtension];
}

- (void)openPrivateXMLDirectory {
  [AppLauncher openPrivateXMLDirectory];
}

- (void)openSystemPreferencesKeyboard {
  [AppLauncher openSystemPreferencesKeyboard];
}

- (void)openUninstaller {
  [AppLauncher openUninstaller];
}

// ----------------------------------------------------------------------
- (void)showExampleStatusWindow:(BOOL)visibility {
  @weakify(self);

  dispatch_async(dispatch_get_main_queue(), ^{
    @strongify(self);
    if (!self) return;

    [self.statusMessageManager showExampleStatusWindow:visibility];
  });
}

// ----------------------------------------------------------------------
- (NSString*)symbolMapName:(NSString*)type value:(NSInteger)value {
  return [self.xmlCompiler symbolMapName:type value:(uint32_t)(value)];
}

// ----------------------------------------------------------------------
- (bycopy CheckboxTree*)checkboxTree {
  return self.xmlCompiler.checkboxTree;
}

- (bycopy ParameterTree*)parameterTree {
  return self.xmlCompiler.parameterTree;
}

// ----------------------------------------------------------------------
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
