#import "ClientForKernelspace.h"
#import "EnvironmentChecker.h"
#import "MigrationUtilities.h"
#import "NotificationKeys.h"
#import "PreferencesKeys.h"
#import "XMLCompiler.h"
#include "pqrs/xml_compiler_bindings_clang.h"

static NSInteger xmlCompilerItemId_;
static dispatch_queue_t xmlCompilerItemIdQueue_;

@interface XMLCompilerItem ()
@property pqrs_xml_compiler* pqrs_xml_compiler;
@property size_t* indexes;
@property size_t indexes_size;
@end

@implementation XMLCompilerItem

+ (void)initialize {
  xmlCompilerItemId_ = 0;
  xmlCompilerItemIdQueue_ = dispatch_queue_create("org.pqrs.Karabiner.XMLCompiler.xmlCompilerItemIdQueue_", NULL);
}

- (instancetype)init {
  self = [super init];

  if (self) {
    dispatch_sync(xmlCompilerItemIdQueue_, ^{
      ++xmlCompilerItemId_;
      _id = @(xmlCompilerItemId_);
    });
  }

  return self;
}

- (instancetype)initWithParent:(pqrs_xml_compiler*)pqrs_xml_compiler parent:(XMLCompilerItem*)parent index:(size_t)index {
  self = [self init];

  if (self) {
    self.pqrs_xml_compiler = pqrs_xml_compiler;

    if (!parent) {
      self.indexes_size = 0;
    } else {
      self.indexes_size = parent.indexes_size + 1;
      self.indexes = (size_t*)malloc(sizeof(size_t) * self.indexes_size);
      if (parent.indexes_size > 0) {
        memcpy(self.indexes, parent.indexes, sizeof(self.indexes[0]) * parent.indexes_size);
      }
      self.indexes[self.indexes_size - 1] = index;
    }
  }

  return self;
}

- (void)dealloc {
  if (self.indexes) {
    free(self.indexes);
    self.indexes = NULL;
  }
}

@end

@implementation CheckboxItem

- (NSString*)getName {
  const char* value = pqrs_xml_compiler_get_preferences_checkbox_node_tree_name(self.pqrs_xml_compiler, self.indexes, self.indexes_size);
  return value ? @(value) : @"";
}

- (NSString*)getStyle {
  const char* value = pqrs_xml_compiler_get_preferences_checkbox_node_tree_style(self.pqrs_xml_compiler, self.indexes, self.indexes_size);
  return value ? @(value) : @"";
}

- (NSString*)getIdentifier {
  const char* value = pqrs_xml_compiler_get_preferences_checkbox_node_tree_identifier(self.pqrs_xml_compiler, self.indexes, self.indexes_size);
  return value ? @(value) : @"";
}

- (NSUInteger)getChildrenCount {
  return pqrs_xml_compiler_get_preferences_checkbox_node_tree_children_count(self.pqrs_xml_compiler, self.indexes, self.indexes_size);
}

- (BOOL)needsShowCheckbox {
  NSString* identifier = [self getIdentifier];
  if ([identifier length] == 0 ||
      [identifier hasPrefix:@"notsave."]) {
    return NO;
  }
  return YES;
}

- (BOOL)isNameMatched:(NSString*)string {
  if (!string) {
    return NO;
  }
  return pqrs_xml_compiler_is_preferences_checkbox_node_tree_name_icontains(self.pqrs_xml_compiler, self.indexes, self.indexes_size, [string UTF8String]);
}

@end

@interface CheckboxItemWithStaticData : CheckboxItem
@property NSString* name;
@property NSString* style;
@end

@implementation CheckboxItemWithStaticData

- (NSString*)getName {
  return self.name ? self.name : @"";
}
- (NSString*)getStyle {
  return self.style ? self.style : @"";
}
- (NSString*)getIdentifier {
  return @"";
}
- (NSUInteger)getChildrenCount {
  return 0;
}
- (BOOL)needsShowCheckbox {
  return NO;
}
- (BOOL)isNameMatched:(NSString*)string {
  return YES;
}

@end

@implementation ParameterItem

- (NSString*)getName {
  const char* value = pqrs_xml_compiler_get_preferences_number_node_tree_name(self.pqrs_xml_compiler, self.indexes, self.indexes_size);
  return value ? @(value) : @"";
}

- (NSString*)getIdentifier {
  const char* value = pqrs_xml_compiler_get_preferences_number_node_tree_identifier(self.pqrs_xml_compiler, self.indexes, self.indexes_size);
  return value ? @(value) : @"";
}

- (NSInteger)getDefaultValue {
  return pqrs_xml_compiler_get_preferences_number_node_tree_default_value(self.pqrs_xml_compiler, self.indexes, self.indexes_size);
}

- (NSInteger)getStep {
  return pqrs_xml_compiler_get_preferences_number_node_tree_step(self.pqrs_xml_compiler, self.indexes, self.indexes_size);
}

- (NSString*)getBaseUnit {
  const char* value = pqrs_xml_compiler_get_preferences_number_node_tree_base_unit(self.pqrs_xml_compiler, self.indexes, self.indexes_size);
  return value ? @(value) : @"";
}

- (NSUInteger)getChildrenCount {
  return pqrs_xml_compiler_get_preferences_number_node_tree_children_count(self.pqrs_xml_compiler, self.indexes, self.indexes_size);
}

@end

@implementation XMLCompilerTree

- (CheckboxItem*)castNodeToCheckboxItem {
  return [self.node isKindOfClass:[CheckboxItem class]] ? (CheckboxItem*)(self.node) : nil;
}

- (ParameterItem*)castNodeToParameterItem {
  return [self.node isKindOfClass:[ParameterItem class]] ? (ParameterItem*)(self.node) : nil;
}

@end

@interface XMLCompiler () {
  pqrs_xml_compiler* pqrs_xml_compiler_;
}
@end

@implementation XMLCompiler

// ------------------------------------------------------------
// private methods

- (XMLCompilerTree*)newCautionNode:(NSString*)name {
  CheckboxItemWithStaticData* checkboxItem = [CheckboxItemWithStaticData new];
  checkboxItem.name = name;
  checkboxItem.style = @"caution";

  XMLCompilerTree* tree = [XMLCompilerTree new];
  tree.node = checkboxItem;

  return tree;
}

- (NSArray*)build_preferencepane_checkbox:(CheckboxItem*)parent {
  size_t size = [parent getChildrenCount];
  if (size == 0) return nil;

  NSMutableArray* children = [NSMutableArray new];

  if (parent.indexes_size == 0) {
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
    XMLCompilerTree* child = [XMLCompilerTree new];
    CheckboxItem* node = [[CheckboxItem alloc] initWithParent:pqrs_xml_compiler_ parent:parent index:i];
    child.node = node;
    child.children = [self build_preferencepane_checkbox:node];

    [children addObject:child];
  }

  return children;
}

- (NSArray*)build_preferencepane_parameter:(ParameterItem*)parent {
  size_t size = [parent getChildrenCount];
  if (size == 0) return nil;

  NSMutableArray* children = [NSMutableArray new];

  for (size_t i = 0; i < size; ++i) {
    XMLCompilerTree* child = [XMLCompilerTree new];
    ParameterItem* node = [[ParameterItem alloc] initWithParent:pqrs_xml_compiler_ parent:parent index:i];
    child.node = node;
    child.children = [self build_preferencepane_parameter:node];

    [children addObject:child];
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
- (id)init {
  self = [super init];

  if (self) {
    pqrs_xml_compiler_initialize(&pqrs_xml_compiler_,
                                 [[[NSBundle mainBundle] resourcePath] UTF8String],
                                 [[[XMLCompiler get_private_xml_path] stringByDeletingLastPathComponent] UTF8String]);
  }

  return self;
}

- (void)dealloc {
  pqrs_xml_compiler_terminate(&pqrs_xml_compiler_);
}

- (void)reload {
  @synchronized(self) {
    [XMLCompiler prepare_private_xml];

    const char* checkbox_xml_file_name = "checkbox.xml";
    if (![[NSUserDefaults standardUserDefaults] boolForKey:kUsePreparedSettings]) {
      checkbox_xml_file_name = "checkbox-omitted.xml";
    }

    pqrs_xml_compiler_reload(pqrs_xml_compiler_, checkbox_xml_file_name);

    {
      CheckboxItem* root = [[CheckboxItem alloc] initWithParent:pqrs_xml_compiler_ parent:nil index:0];
      XMLCompilerTree* tree = [XMLCompilerTree new];
      tree.children = [self build_preferencepane_checkbox:root];
      self.preferencepane_checkbox = tree;
    }

    // build preferencepane_parameter
    {
      ParameterItem* root = [[ParameterItem alloc] initWithParent:pqrs_xml_compiler_ parent:nil index:0];
      XMLCompilerTree* tree = [XMLCompilerTree new];
      tree.children = [self build_preferencepane_parameter:root];
      self.preferencepane_parameter = tree;
    }
  }

  // ------------------------------------------------------------
  {
    NSString* message = [self preferencepane_error_message];
    if (message) {
      NSUInteger maxlen = 500;
      if ([message length] > maxlen) {
        message = [message substringToIndex:maxlen];
      }

      dispatch_async(dispatch_get_main_queue(), ^{
        NSAlert* alert = [NSAlert new];
        [alert setMessageText:@"Karabiner Error"];
        [alert addButtonWithTitle:@"Close"];
        [alert setInformativeText:message];

        [alert runModal];
      });
    }
  }

  // ------------------------------------------------------------
  // We need to send a notification outside synchronized block to prevent lock.
  [[NSNotificationCenter defaultCenter] postNotificationName:kConfigXMLReloadedNotification object:nil];
}

- (size_t)remapclasses_initialize_vector_size {
  @synchronized(self) {
    return pqrs_xml_compiler_get_remapclasses_initialize_vector_size(pqrs_xml_compiler_);
  }
}

- (const uint32_t*)remapclasses_initialize_vector_data {
  @synchronized(self) {
    return pqrs_xml_compiler_get_remapclasses_initialize_vector_data(pqrs_xml_compiler_);
  }
}

- (uint32_t)remapclasses_initialize_vector_config_count {
  @synchronized(self) {
    return pqrs_xml_compiler_get_remapclasses_initialize_vector_config_count(pqrs_xml_compiler_);
  }
}

- (uint32_t)keycode:(NSString*)name {
  @synchronized(self) {
    return pqrs_xml_compiler_get_symbol_map_value(pqrs_xml_compiler_, [name UTF8String]);
  }
}

- (NSString*)identifier:(uint32_t)config_index {
  @synchronized(self) {
    const char* p = pqrs_xml_compiler_get_identifier(pqrs_xml_compiler_, config_index);
    if (!p) return nil;

    return @(p);
  }
}

- (NSString*)symbolMapName:(NSString*)type value:(uint32_t)value {
  @synchronized(self) {
    const char* p = pqrs_xml_compiler_get_symbol_map_name(pqrs_xml_compiler_, [type UTF8String], value);
    if (!p) return nil;

    return @(p);
  }
}

- (int)config_index:(NSString*)identifier {
  @synchronized(self) {
    return pqrs_xml_compiler_get_config_index(pqrs_xml_compiler_, [identifier UTF8String]);
  }
}

- (NSArray*)appids:(NSString*)bundleIdentifier {
  @synchronized(self) {
    NSMutableArray* ids = [NSMutableArray new];
    size_t size = pqrs_xml_compiler_get_app_vector_size(pqrs_xml_compiler_);
    const char* identifier = [bundleIdentifier UTF8String];
    for (size_t i = 0; i < size; ++i) {
      uint32_t appid = 0;
      if (pqrs_xml_compiler_is_app_matched(pqrs_xml_compiler_, &appid, i, identifier)) {
        [ids addObject:@(appid)];
      }
    }

    if ([ids count] == 0) {
      [ids addObject:@0];
    }

    return ids;
  }
}

- (NSArray*)windownameids:(NSString*)windowName {
  @synchronized(self) {
    NSMutableArray* ids = [NSMutableArray new];
    size_t size = pqrs_xml_compiler_get_window_name_vector_size(pqrs_xml_compiler_);
    const char* utf8string = [windowName UTF8String];
    for (size_t i = 0; i < size; ++i) {
      uint32_t windownameid = 0;
      if (pqrs_xml_compiler_is_window_name_matched(pqrs_xml_compiler_, &windownameid, i, utf8string)) {
        [ids addObject:@(windownameid)];
      }
    }

    if ([ids count] == 0) {
      [ids addObject:@0];
    }

    return ids;
  }
}

- (uint32_t)uielementroleid:(NSString*)uiElementRole {
  @synchronized(self) {
    NSString* key = [NSString stringWithFormat:@"UIElementRole::%@", uiElementRole];
    return pqrs_xml_compiler_get_symbol_map_value(pqrs_xml_compiler_, [key UTF8String]);
  }
}

- (NSArray*)inputsourceids:(NSString*)languagecode
             inputSourceID:(NSString*)inputSourceID
               inputModeID:(NSString*)inputModeID {
  @synchronized(self) {
    NSMutableArray* ids = [NSMutableArray new];
    size_t size = pqrs_xml_compiler_get_inputsource_vector_size(pqrs_xml_compiler_);
    const char* languagecode_u8 = [languagecode UTF8String];
    const char* inputsourceid_u8 = [inputSourceID UTF8String];
    const char* inputmodeid_u8 = [inputModeID UTF8String];
    for (size_t i = 0; i < size; ++i) {
      uint32_t inputsource = 0;
      if (pqrs_xml_compiler_is_inputsource_matched(pqrs_xml_compiler_, &inputsource, i, languagecode_u8, inputsourceid_u8, inputmodeid_u8)) {
        [ids addObject:@(inputsource)];
      }
    }

    if ([ids count] == 0) {
      [ids addObject:@0];
    }

    return ids;
  }
}

- (BOOL)is_vk_change_inputsource_matched:(uint32_t)keycode
                            languagecode:(NSString*)languagecode
                           inputSourceID:(NSString*)inputSourceID
                             inputModeID:(NSString*)inputModeID;
{
  @synchronized(self) {
    return pqrs_xml_compiler_is_vk_change_inputsource_matched(pqrs_xml_compiler_,
                                                              keycode,
                                                              [languagecode UTF8String],
                                                              [inputSourceID UTF8String],
                                                              [inputModeID UTF8String]);
  }
}

- (NSString*)url:(uint32_t)keycode {
  @synchronized(self) {
    const char* p = pqrs_xml_compiler_get_url(pqrs_xml_compiler_, keycode);
    if (!p) return nil;

    return @(p);
  }
}

- (NSString*)urlType:(uint32_t)keycode {
  @synchronized(self) {
    const char* p = pqrs_xml_compiler_get_url_type(pqrs_xml_compiler_, keycode);
    if (!p) return nil;

    return @(p);
  }
}

- (BOOL)urlIsBackground:(uint32_t)keycode {
  @synchronized(self) {
    return pqrs_xml_compiler_get_url_background(pqrs_xml_compiler_, keycode);
  }
}

- (NSString*)preferencepane_error_message;
{
  @synchronized(self) {
    if (pqrs_xml_compiler_get_error_count(pqrs_xml_compiler_) == 0) {
      return nil;
    }
    const char* error_message = pqrs_xml_compiler_get_error_message(pqrs_xml_compiler_);
    if (!error_message) {
      return nil;
    }

    return [NSString stringWithFormat:@"Error in XML.\n%s\n%s\n%s",
                                      "----------------------------------------",
                                      error_message,
                                      "----------------------------------------"];
  }
}

@end
