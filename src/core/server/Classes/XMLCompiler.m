#import "XMLCompiler.h"
#import "CheckboxTree.h"
#import "EnvironmentChecker.h"
#import "MigrationUtilities.h"
#import "NotificationKeys.h"
#import "ParameterTree.h"
#import "PreferencesManager.h"
#import "PreferencesModel.h"
#import "SharedKeys.h"
#import "weakify.h"

#include "pqrs/xml_compiler_bindings_clang.h"

@interface XMLCompiler ()

@property(weak) IBOutlet PreferencesManager* preferencesManager;
@property(weak) IBOutlet PreferencesModel* preferencesModel;

@property dispatch_queue_t xmlCompilerReloadQueue;
@property dispatch_source_t needsReloadTimer;

@property(readwrite) CheckboxTree* checkboxTree;
@property(readwrite) ParameterTree* parameterTree;

@property pqrs_xml_compiler* pqrs_xml_compiler;
@property NSInteger xmlNotificationsSequentialNumber;

@end

@implementation XMLCompiler

// ------------------------------------------------------------
// private methods

- (CheckboxTree*)newCautionNode:(NSString*)name {
  CheckboxItem* node = [[CheckboxItem alloc] initWithName:[name UTF8String] style:"caution" identifier:""];
  CheckboxTree* tree = [[CheckboxTree alloc] initWithItem:node children:nil];
  return tree;
}

- (NSString*)preferencepane_error_message {
  if (pqrs_xml_compiler_get_error_count(self.pqrs_xml_compiler) == 0) {
    return nil;
  }
  const char* error_message = pqrs_xml_compiler_get_error_message(self.pqrs_xml_compiler);
  if (!error_message) {
    return nil;
  }

  return [NSString stringWithFormat:@"Error in XML.\n%s\n%s\n%s",
                                    "----------------------------------------",
                                    error_message,
                                    "----------------------------------------"];
}

- (NSArray*)checkboxTreeChildren:(const pqrs_xml_compiler_preferences_checkbox_node_tree*)parent root:(BOOL)root {
  size_t size = pqrs_xml_compiler_get_preferences_checkbox_node_tree_children_count(parent);
  if (size == 0) return nil;

  NSMutableArray* children = [NSMutableArray new];

  if (root) {
    // Add error messages into root children.
    {
      NSString* message = [self preferencepane_error_message];
      if (message) {
        [children addObject:[self newCautionNode:message]];
      }
    }
    if ([EnvironmentChecker checkDoubleCommand]) {
      [children addObject:[self newCautionNode:@"A conflicting application is installed: DoubleCommand\n\nKarabiner ignores keyboard devices.\n(You can use Karabiner as a pointing device remapper.)"]];
    }
    if ([EnvironmentChecker checkKeyRemap4MacBook]) {
      [children addObject:[self newCautionNode:@"An old kernel extension has still been loaded. Please restart your system in order to unload it."]];
    }
    if ([EnvironmentChecker checkKirgudu]) {
      [children addObject:[self newCautionNode:@"A conflicting application is installed: Kirgudu\n\nKarabiner ignores keyboard devices.\n(You can use Karabiner as a pointing device remapper.)"]];
    }
    if ([EnvironmentChecker checkSmoothMouse]) {
      [children addObject:[self newCautionNode:@"A conflicting application is installed: SmoothMouse\n\nKarabiner ignores pointing devices.\n(You can use Karabiner as a keyboard device remapper.)"]];
    }
  }

  for (size_t i = 0; i < size; ++i) {
    const pqrs_xml_compiler_preferences_checkbox_node_tree* child = pqrs_xml_compiler_get_preferences_checkbox_node_tree_child(parent, i);
    if (!child) continue;

    const char* name = pqrs_xml_compiler_get_preferences_checkbox_node_tree_name(child);
    const char* style = pqrs_xml_compiler_get_preferences_checkbox_node_tree_style(child);
    const char* identifier = pqrs_xml_compiler_get_preferences_checkbox_node_tree_identifier(child);

    CheckboxItem* node = [[CheckboxItem alloc] initWithName:name style:style identifier:identifier];
    CheckboxTree* tree = [[CheckboxTree alloc] initWithItem:node children:[self checkboxTreeChildren:child root:NO]];
    [children addObject:tree];
  }

  return children;
}

- (NSArray*)parameterTreeChildren:(const pqrs_xml_compiler_preferences_number_node_tree*)parent {
  size_t size = pqrs_xml_compiler_get_preferences_number_node_tree_children_count(parent);
  if (size == 0) return nil;

  NSMutableArray* children = [NSMutableArray new];

  for (size_t i = 0; i < size; ++i) {
    const pqrs_xml_compiler_preferences_number_node_tree* child = pqrs_xml_compiler_get_preferences_number_node_tree_child(parent, i);
    if (!child) continue;

    const char* name = pqrs_xml_compiler_get_preferences_number_node_tree_name(child);
    const char* identifier = pqrs_xml_compiler_get_preferences_number_node_tree_identifier(child);
    int defaultValue = pqrs_xml_compiler_get_preferences_number_node_tree_default_value(child);
    int step = pqrs_xml_compiler_get_preferences_number_node_tree_step(child);
    const char* baseUnit = pqrs_xml_compiler_get_preferences_number_node_tree_base_unit(child);

    ParameterItem* node = [[ParameterItem alloc] initWithName:name identifier:identifier defaultValue:defaultValue step:step baseUnit:baseUnit];
    ParameterTree* tree = [[ParameterTree alloc] initWithItem:node children:[self parameterTreeChildren:child]];
    [children addObject:tree];
  }

  return children;
}

// ------------------------------------------------------------
+ (void)prepare_private_xml {
  [XMLCompiler get_private_xml_path];
}

+ (NSString*)get_private_xml_path {
  NSFileManager* filemanager = [NSFileManager defaultManager];
  NSArray* paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
  NSString* path = paths[0];
  path = [path stringByAppendingPathComponent:[MigrationUtilities applicationSupportName]];
  if (![filemanager fileExistsAtPath:path]) {
    [filemanager createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:NULL];
  }

  path = [path stringByAppendingPathComponent:@"private.xml"];
  // Copy default private.xml to user directory if private.xml is not found or filesize is 0.
  if (![filemanager fileExistsAtPath:path] ||
      [[filemanager attributesOfItemAtPath:path error:nil] fileSize] == 0) {
    [filemanager removeItemAtPath:path error:nil];
    [filemanager copyItemAtPath:[[NSBundle mainBundle] pathForResource:@"private" ofType:@"xml"] toPath:path error:nil];

    // copyItemAtPath does not change ctime and mtime of file.
    // (For example, mtime of destination file == mtime of source file.)
    // Therefore, we need to set mtime to current time.
    NSDictionary* attr = @{NSFileCreationDate : [NSDate date],
                           NSFileModificationDate : [NSDate date]};
    [filemanager setAttributes:attr ofItemAtPath:path error:NULL];
  }

  BOOL isDirectory;
  if ([filemanager fileExistsAtPath:path isDirectory:&isDirectory] && !isDirectory) {
    return path;
  }

  return @"";
}

// ------------------------------------------------------------
- (instancetype)init {
  self = [super init];

  if (self) {
    _xmlCompilerReloadQueue = dispatch_queue_create("org.pqrs.Karabiner.XMLCompiler.xmlCompilerReloadQueue", NULL);

    pqrs_xml_compiler* p = NULL;
    pqrs_xml_compiler_initialize(&p,
                                 [[[NSBundle mainBundle] resourcePath] UTF8String],
                                 [[[XMLCompiler get_private_xml_path] stringByDeletingLastPathComponent] UTF8String]);
    _pqrs_xml_compiler = p;

    _needsReloadTimer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
    if (_needsReloadTimer) {
      dispatch_source_set_timer(_needsReloadTimer, dispatch_time(DISPATCH_TIME_NOW, 5.0 * NSEC_PER_SEC), 5.0 * NSEC_PER_SEC, 0);
      @weakify(self);
      dispatch_source_set_event_handler(_needsReloadTimer, ^{
        @strongify(self);
        if (!self) return;

        __block bool needsReload = NO;
        dispatch_sync(self.xmlCompilerReloadQueue, ^{
          needsReload = pqrs_xml_compiler_needs_reload(self.pqrs_xml_compiler);
        });
        if (needsReload) {
          NSLog(@"Call reload in needsReloadTimer");
          [self reload];
        }
      });
      dispatch_resume(_needsReloadTimer);
    }
  }

  return self;
}

- (void)dealloc {
  pqrs_xml_compiler* p = _pqrs_xml_compiler;
  pqrs_xml_compiler_terminate(&p);
}

- (void)reload {
  ++(self.xmlNotificationsSequentialNumber);
  [[NSDistributedNotificationCenter defaultCenter] postNotificationName:kKarabinerXMLLoadingNotification
                                                                 object:nil
                                                               userInfo:@{ @"xmlNotificationsSequentialNumber" : @(self.xmlNotificationsSequentialNumber) }
                                                     deliverImmediately:YES];

  __block NSString* errorMessage = nil;

  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    [XMLCompiler prepare_private_xml];

    const char* checkbox_xml_file_name = "checkbox.xml";
    if (!self.preferencesModel.usePreparedSettings) {
      checkbox_xml_file_name = "checkbox-omitted.xml";
    }

    pqrs_xml_compiler_reload(self.pqrs_xml_compiler, checkbox_xml_file_name);

    {
      const pqrs_xml_compiler_preferences_checkbox_node_tree* root = pqrs_xml_compiler_get_preferences_checkbox_node_tree_root(self.pqrs_xml_compiler);
      self.checkboxTree = [[CheckboxTree alloc] initWithItem:nil children:[self checkboxTreeChildren:root root:YES]];
    }
    // build preferencepane_parameter
    {
      const pqrs_xml_compiler_preferences_number_node_tree* root = pqrs_xml_compiler_get_preferences_number_node_tree_root(self.pqrs_xml_compiler);
      self.parameterTree = [[ParameterTree alloc] initWithItem:nil children:[self parameterTreeChildren:root]];
    }

    errorMessage = [self preferencepane_error_message];
  });

  // ------------------------------------------------------------
  if (errorMessage) {
    NSUInteger maxlen = 500;
    if ([errorMessage length] > maxlen) {
      errorMessage = [errorMessage substringToIndex:maxlen];
    }

    dispatch_async(dispatch_get_main_queue(), ^{
      NSAlert* alert = [NSAlert new];
      [alert setMessageText:@"Karabiner Error"];
      [alert addButtonWithTitle:@"Close"];
      [alert setInformativeText:errorMessage];

      [alert runModal];
    });
  }

  // ------------------------------------------------------------
  // We need to send a notification outside synchronized block to prevent lock.
  [[NSNotificationCenter defaultCenter] postNotificationName:kConfigXMLReloadedNotification object:nil];

  ++(self.xmlNotificationsSequentialNumber);
  [[NSDistributedNotificationCenter defaultCenter] postNotificationName:kKarabinerXMLReloadedNotification
                                                                 object:nil
                                                               userInfo:@{ @"xmlNotificationsSequentialNumber" : @(self.xmlNotificationsSequentialNumber) }
                                                     deliverImmediately:YES];
}

- (size_t)remapclasses_initialize_vector_size {
  __block size_t result = 0;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    result = pqrs_xml_compiler_get_remapclasses_initialize_vector_size(self.pqrs_xml_compiler);
  });
  return result;
}

- (const uint32_t*)remapclasses_initialize_vector_data {
  __block const uint32_t* result = NULL;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    result = pqrs_xml_compiler_get_remapclasses_initialize_vector_data(self.pqrs_xml_compiler);
  });
  return result;
}

- (uint32_t)remapclasses_initialize_vector_config_count {
  __block uint32_t result = 0;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    result = pqrs_xml_compiler_get_remapclasses_initialize_vector_config_count(self.pqrs_xml_compiler);
  });
  return result;
}

- (uint32_t)keycode:(NSString*)name {
  __block uint32_t result = 0;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    result = pqrs_xml_compiler_get_symbol_map_value(self.pqrs_xml_compiler, [name UTF8String]);
  });
  return result;
}

- (NSString*)identifier:(uint32_t)config_index {
  __block NSString* result = nil;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    const char* p = pqrs_xml_compiler_get_identifier(self.pqrs_xml_compiler, config_index);
    if (p) {
      result = @(p);
    }
  });
  return result;
}

- (NSString*)symbolMapName:(NSString*)type value:(uint32_t)value {
  __block NSString* result = nil;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    const char* p = pqrs_xml_compiler_get_symbol_map_name(self.pqrs_xml_compiler, [type UTF8String], value);
    if (p) {
      result = @(p);
    }
  });
  return result;
}

- (int)config_index:(NSString*)identifier {
  __block int result = 0;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    result = pqrs_xml_compiler_get_config_index(self.pqrs_xml_compiler, [identifier UTF8String]);
  });
  return result;
}

- (NSString*)overrideBundleIdentifier:(NSString*)bundleIdentifier windowName:(NSString*)windowName uiElementRole:(NSString*)uiElementRole {
  __block NSString* result = nil;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    const char* override_bundle_identifier = pqrs_xml_compiler_override_bundle_identifier(self.pqrs_xml_compiler,
                                                                                          [bundleIdentifier UTF8String],
                                                                                          [windowName UTF8String],
                                                                                          [uiElementRole UTF8String]);
    if (override_bundle_identifier) {
      result = [NSString stringWithUTF8String:override_bundle_identifier];
    }
  });
  return result;
}

- (NSArray*)appids:(NSString*)bundleIdentifier {
  __block NSArray* result = nil;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    NSMutableArray* ids = [NSMutableArray new];
    size_t size = pqrs_xml_compiler_get_app_vector_size(self.pqrs_xml_compiler);
    const char* identifier = [bundleIdentifier UTF8String];
    for (size_t i = 0; i < size; ++i) {
      uint32_t appid = 0;
      if (pqrs_xml_compiler_is_app_matched(self.pqrs_xml_compiler, &appid, i, identifier)) {
        [ids addObject:@(appid)];
      }
    }

    if ([ids count] == 0) {
      [ids addObject:@0];
    }

    result = ids;
  });
  return result;
}

- (NSArray*)windownameids:(NSString*)windowName {
  __block NSArray* result = nil;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    NSMutableArray* ids = [NSMutableArray new];
    size_t size = pqrs_xml_compiler_get_window_name_vector_size(self.pqrs_xml_compiler);
    const char* utf8string = [windowName UTF8String];
    for (size_t i = 0; i < size; ++i) {
      uint32_t windownameid = 0;
      if (pqrs_xml_compiler_is_window_name_matched(self.pqrs_xml_compiler, &windownameid, i, utf8string)) {
        [ids addObject:@(windownameid)];
      }
    }

    if ([ids count] == 0) {
      [ids addObject:@0];
    }

    result = ids;
  });
  return result;
}

- (uint32_t)uielementroleid:(NSString*)uiElementRole {
  __block uint32_t result = 0;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    NSString* key = [NSString stringWithFormat:@"UIElementRole::%@", uiElementRole];
    result = pqrs_xml_compiler_get_symbol_map_value(self.pqrs_xml_compiler, [key UTF8String]);
  });
  return result;
}

- (NSArray*)inputsourceids:(NSString*)languagecode
             inputSourceID:(NSString*)inputSourceID
               inputModeID:(NSString*)inputModeID {
  __block NSArray* result = nil;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    NSMutableArray* ids = [NSMutableArray new];
    size_t size = pqrs_xml_compiler_get_inputsource_vector_size(self.pqrs_xml_compiler);
    const char* languagecode_u8 = [languagecode UTF8String];
    const char* inputsourceid_u8 = [inputSourceID UTF8String];
    const char* inputmodeid_u8 = [inputModeID UTF8String];
    for (size_t i = 0; i < size; ++i) {
      uint32_t inputsource = 0;
      if (pqrs_xml_compiler_is_inputsource_matched(self.pqrs_xml_compiler, &inputsource, i, languagecode_u8, inputsourceid_u8, inputmodeid_u8)) {
        [ids addObject:@(inputsource)];
      }
    }

    if ([ids count] == 0) {
      [ids addObject:@0];
    }

    result = ids;
  });
  return result;
}

- (BOOL)is_vk_change_inputsource_matched:(uint32_t)keycode
                            languagecode:(NSString*)languagecode
                           inputSourceID:(NSString*)inputSourceID
                             inputModeID:(NSString*)inputModeID;
{
  __block BOOL result = NO;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    result = pqrs_xml_compiler_is_vk_change_inputsource_matched(self.pqrs_xml_compiler,
                                                                keycode,
                                                                [languagecode UTF8String],
                                                                [inputSourceID UTF8String],
                                                                [inputModeID UTF8String]);
  });
  return result;
}

- (NSString*)url:(uint32_t)keycode {
  __block NSString* result = nil;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    const char* p = pqrs_xml_compiler_get_url(self.pqrs_xml_compiler, keycode);
    if (p) {
      result = @(p);
    }
  });
  return result;
}

- (NSString*)urlType:(uint32_t)keycode {
  __block NSString* result = nil;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    const char* p = pqrs_xml_compiler_get_url_type(self.pqrs_xml_compiler, keycode);
    if (p) {
      result = @(p);
    }
  });
  return result;
}

- (BOOL)urlIsBackground:(uint32_t)keycode {
  __block BOOL result = NO;
  dispatch_sync(self.xmlCompilerReloadQueue, ^{
    result = pqrs_xml_compiler_get_url_background(self.pqrs_xml_compiler, keycode);
  });
  return result;
}

@end
