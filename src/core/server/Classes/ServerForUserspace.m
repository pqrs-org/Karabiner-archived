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

#define ASYNC_RUN_IN_MAIN_QUEUE(CODE)                                              \
  {                                                                                \
    @weakify(self);                                                                \
                                                                                   \
    /* We have to use main queue for [PreferencesManager savePreferencesModel]. */ \
    dispatch_async(dispatch_get_main_queue(), ^{                                   \
      @strongify(self);                                                            \
      if (!self) return;                                                           \
                                                                                   \
      CODE;                                                                        \
    });                                                                            \
  }

- (void)savePreferencesModel:(bycopy PreferencesModel*)preferencesModel processIdentifier:(int)processIdentifier {
  // We have to use main queue for [PreferencesManager savePreferencesModel].
  ASYNC_RUN_IN_MAIN_QUEUE(
      [self.preferencesManager savePreferencesModel:preferencesModel
                                  processIdentifier:processIdentifier]);
}

- (void)updateKextValue:(NSString*)name {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [self.preferencesManager updateKextValue:name]);
}

- (void)updateKextValues {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [self.clientForKernelspace send_config_to_kext]);
}

- (void)updateStartAtLogin {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [self.serverController updateStartAtLogin:YES]);
}

- (void)updateStatusBar {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [self.statusBar refresh]);
}

- (void)updateStatusWindow {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [self.statusMessageManager refresh]);
}

- (void)restartAXNotifier {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [self.axNotifierManager restartAXNotifier]);
}

- (void)unsetDebugFlags {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [self.clientForKernelspace unset_debug_flags]);
}

- (void)terminateServerProcess {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [self.serverController terminateServerProcess]);
}

- (void)relaunch {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [Relauncher relaunch]);
}

- (void)checkForUpdatesStableOnly {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [UpdaterController checkForUpdatesStableOnly]);
}

- (void)checkForUpdatesWithBetaVersion {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [UpdaterController checkForUpdatesWithBetaVersion]);
}

- (void)reloadXML {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [self.xmlCompiler reload]);
}

- (void)openEventViewer {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [AppLauncher openEventViewer]);
}

- (void)openMultiTouchExtension {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [AppLauncher openMultiTouchExtension]);
}

- (void)openPrivateXMLDirectory {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [AppLauncher openPrivateXMLDirectory]);
}

- (void)openSystemPreferencesKeyboard {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [AppLauncher openSystemPreferencesKeyboard]);
}

- (void)openUninstaller {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [AppLauncher openUninstaller]);
}

// ----------------------------------------------------------------------
- (void)showExampleStatusWindow:(BOOL)visibility {
  ASYNC_RUN_IN_MAIN_QUEUE(
      [self.statusMessageManager showExampleStatusWindow:visibility]);
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
  ASYNC_RUN_IN_MAIN_QUEUE(
      return [self.appDelegate updateFocusedUIElementInformation:information]);
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
