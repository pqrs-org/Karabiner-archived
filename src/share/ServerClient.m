#import "ServerClient.h"
#import "SharedKeys.h"
#import "weakify.h"

@interface ServerClient ()

@property NSDistantObject<ServerClientProtocol>* connection;
@property dispatch_queue_t connectionQueue;

@end

@implementation ServerClient

- (NSDistantObject<ServerClientProtocol>*)proxy {
  dispatch_sync(self.connectionQueue, ^{
    if (!self.connection) {
      self.connection = (NSDistantObject<ServerClientProtocol>*)([NSConnection rootProxyForConnectionWithRegisteredName:kKarabinerConnectionName host:nil]);
      [self.connection setProtocolForProxy:@protocol(ServerClientProtocol)];
    }
  });
  return self.connection;
}

- (void)observer_NSConnectionDidDieNotification:(NSNotification*)__unused notification {
  @weakify(self);

  dispatch_async(dispatch_get_main_queue(), ^{
    @strongify(self);
    if (!self) return;

    dispatch_sync(self.connectionQueue, ^{
      NSLog(@"observer_NSConnectionDidDieNotification is called");
      self.connection = nil;
    });
  });
}

- (instancetype)init {
  self = [super init];

  if (self) {
    self.connectionQueue = dispatch_queue_create("org.pqrs.Karabiner.ServerClient.connectionQueue", NULL);

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(observer_NSConnectionDidDieNotification:)
                                                 name:NSConnectionDidDieNotification
                                               object:nil];
  }

  return self;
}

- (void)dealloc {
  // Call removeObserver first because observer may refresh connection.
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark - ServerClientProtocol

#define NOEXCEPTION(CODE)              \
  @try {                               \
    CODE;                              \
  } @catch (NSException * exception) { \
  }

- (NSString*)bundleVersion {
  NOEXCEPTION(return [self.proxy bundleVersion]);
  return nil;
}

- (bycopy PreferencesModel*)preferencesModel {
  NOEXCEPTION(return [self.proxy preferencesModel]);
  return nil;
}

- (bycopy AXNotifierPreferencesModel*)axNotifierPreferencesModel {
  NOEXCEPTION(return [self.proxy axNotifierPreferencesModel]);
  return nil;
}

- (void)savePreferencesModel:(PreferencesModel*)preferencesModel processIdentifier:(int)processIdentifier {
  NOEXCEPTION(return [self.proxy savePreferencesModel:preferencesModel processIdentifier:processIdentifier]);
}

- (void)updateKextValue:(NSString*)name {
  NOEXCEPTION([self.proxy updateKextValue:name]);
}

- (void)updateKextValues {
  NOEXCEPTION([self.proxy updateKextValues]);
}

- (void)updateStartAtLogin {
  NOEXCEPTION([self.proxy updateStartAtLogin]);
}

- (void)updateStatusBar {
  NOEXCEPTION([self.proxy updateStatusBar]);
}

- (void)updateStatusWindow {
  NOEXCEPTION([self.proxy updateStatusWindow]);
}

- (void)restartAXNotifier {
  NOEXCEPTION([self.proxy restartAXNotifier]);
}

- (void)unsetDebugFlags {
  NOEXCEPTION([self.proxy unsetDebugFlags]);
}

- (NSString*)symbolMapName:(NSString*)type value:(NSInteger)value {
  NOEXCEPTION(return [self.proxy symbolMapName:type value:value]);
  return nil;
}

- (void)terminateServerProcess {
  NOEXCEPTION([self.proxy terminateServerProcess]);
}

- (void)relaunch {
  NOEXCEPTION([self.proxy relaunch]);
}

- (void)checkForUpdatesStableOnly {
  NOEXCEPTION([self.proxy checkForUpdatesStableOnly]);
}

- (void)checkForUpdatesWithBetaVersion {
  NOEXCEPTION([self.proxy checkForUpdatesWithBetaVersion]);
}

- (void)reloadXML {
  NOEXCEPTION([self.proxy reloadXML]);
}

- (void)openEventViewer {
  NOEXCEPTION([self.proxy openEventViewer]);
}

- (void)openMultiTouchExtension {
  NOEXCEPTION([self.proxy openMultiTouchExtension]);
}

- (void)openPrivateXMLDirectory {
  NOEXCEPTION([self.proxy openPrivateXMLDirectory]);
}

- (void)openSystemPreferencesKeyboard {
  NOEXCEPTION([self.proxy openSystemPreferencesKeyboard]);
}

- (void)openUninstaller {
  NOEXCEPTION([self.proxy openUninstaller]);
}

- (void)showExampleStatusWindow:(BOOL)visibility {
  NOEXCEPTION([self.proxy showExampleStatusWindow:visibility]);
}

- (bycopy CheckboxTree*)checkboxTree {
  NOEXCEPTION(return [self.proxy checkboxTree]);
  return nil;
}

- (bycopy ParameterTree*)parameterTree {
  NOEXCEPTION(return [self.proxy parameterTree]);
  return nil;
}

- (void)updateFocusedUIElementInformation:(NSDictionary*)information {
  NOEXCEPTION([self.proxy updateFocusedUIElementInformation:information]);
}

- (NSArray*)device_information:(NSInteger)type {
  NOEXCEPTION(return [self.proxy device_information:type]);
  return nil;
}

- (NSDictionary*)focused_uielement_information {
  NOEXCEPTION(return [self.proxy focused_uielement_information]);
  return nil;
}

- (NSArray*)workspace_app_ids {
  NOEXCEPTION(return [self.proxy workspace_app_ids]);
  return nil;
}

- (NSArray*)workspace_window_name_ids {
  NOEXCEPTION(return [self.proxy workspace_window_name_ids]);
  return nil;
}

- (NSArray*)workspace_inputsource_ids {
  NOEXCEPTION(return [self.proxy workspace_inputsource_ids]);
  return nil;
}

- (NSDictionary*)inputsource_information {
  NOEXCEPTION(return [self.proxy inputsource_information]);
  return nil;
}

@end
