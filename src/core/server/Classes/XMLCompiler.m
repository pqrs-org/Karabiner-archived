#import "XMLCompiler.h"
#import "EnvironmentChecker.h"
#import "MigrationUtilities.h"
#import "NotificationKeys.h"
#import "PreferencesManager.h"
#import "PreferencesModel.h"
#import "SharedXMLCompilerTree.h"
#include "pqrs/xml_compiler_bindings_clang.h"

static NSInteger xmlCompilerItemId_;
static dispatch_queue_t xmlCompilerItemIdQueue_;

@interface XMLCompilerItem ()

@property(readwrite) NSNumber* id;
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
      self.id = @(xmlCompilerItemId_);
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

- (CheckboxItem*)castToCheckboxItem {
  return [self isKindOfClass:[CheckboxItem class]] ? (CheckboxItem*)(self) : nil;
}

- (ParameterItem*)castToParameterItem {
  return [self isKindOfClass:[ParameterItem class]] ? (ParameterItem*)(self) : nil;
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

@property(copy) NSString* name;
@property(copy) NSString* style;

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
@end

@interface XMLCompiler ()

@property(weak) IBOutlet PreferencesManager* preferencesManager;
@property(weak) IBOutlet PreferencesModel* preferencesModel;

@property(readwrite) XMLCompilerTree* preferencepane_checkbox;
@property(readwrite) XMLCompilerTree* preferencepane_parameter;
@property(readwrite) SharedXMLCompilerTree* sharedCheckboxTree;
@property(readwrite) SharedXMLCompilerTree* sharedParameterTree;

@property pqrs_xml_compiler* pqrs_xml_compiler;
@property NSMutableDictionary* xmlCompilerTreeDictionary;

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
    CheckboxItem* node = [[CheckboxItem alloc] initWithParent:self.pqrs_xml_compiler parent:parent index:i];
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
    ParameterItem* node = [[ParameterItem alloc] initWithParent:self.pqrs_xml_compiler parent:parent index:i];
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
- (instancetype)init {
  self = [super init];

  if (self) {
    pqrs_xml_compiler* p = NULL;
    pqrs_xml_compiler_initialize(&p,
                                 [[[NSBundle mainBundle] resourcePath] UTF8String],
                                 [[[XMLCompiler get_private_xml_path] stringByDeletingLastPathComponent] UTF8String]);
    self.pqrs_xml_compiler = p;

    self.xmlCompilerTreeDictionary = [NSMutableDictionary new];
  }

  return self;
}

- (void)dealloc {
  pqrs_xml_compiler* p = self.pqrs_xml_compiler;
  pqrs_xml_compiler_terminate(&p);
}

- (void)reload {
  @synchronized(self) {
    [XMLCompiler prepare_private_xml];

    const char* checkbox_xml_file_name = "checkbox.xml";
    if (!self.preferencesModel.usePreparedSettings) {
      checkbox_xml_file_name = "checkbox-omitted.xml";
    }

    pqrs_xml_compiler_reload(self.pqrs_xml_compiler, checkbox_xml_file_name);
    [self.xmlCompilerTreeDictionary removeAllObjects];

    {
      CheckboxItem* root = [[CheckboxItem alloc] initWithParent:self.pqrs_xml_compiler parent:nil index:0];
      XMLCompilerTree* tree = [XMLCompilerTree new];
      tree.children = [self build_preferencepane_checkbox:root];
      self.preferencepane_checkbox = tree;
      self.sharedCheckboxTree = [self buildSharedXMLCompilerTree:tree];
    }

    // build preferencepane_parameter
    {
      ParameterItem* root = [[ParameterItem alloc] initWithParent:self.pqrs_xml_compiler parent:nil index:0];
      XMLCompilerTree* tree = [XMLCompilerTree new];
      tree.children = [self build_preferencepane_parameter:root];
      self.preferencepane_parameter = tree;
      self.sharedParameterTree = [self buildSharedXMLCompilerTree:tree];
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

- (SharedXMLCompilerTree*)buildSharedXMLCompilerTree:(XMLCompilerTree*)tree {
  NSMutableArray* children = [NSMutableArray new];
  for (XMLCompilerTree* child in tree.children) {
    [children addObject:[self buildSharedXMLCompilerTree:child]];
  }
  SharedXMLCompilerTree* sharedTree = [[SharedXMLCompilerTree alloc] initWithId:tree.node.id children:children];
  if ([sharedTree.id integerValue] > 0) {
    self.xmlCompilerTreeDictionary[sharedTree.id] = tree;
  }
  return sharedTree;
}

- (size_t)remapclasses_initialize_vector_size {
  @synchronized(self) {
    return pqrs_xml_compiler_get_remapclasses_initialize_vector_size(self.pqrs_xml_compiler);
  }
}

- (const uint32_t*)remapclasses_initialize_vector_data {
  @synchronized(self) {
    return pqrs_xml_compiler_get_remapclasses_initialize_vector_data(self.pqrs_xml_compiler);
  }
}

- (uint32_t)remapclasses_initialize_vector_config_count {
  @synchronized(self) {
    return pqrs_xml_compiler_get_remapclasses_initialize_vector_config_count(self.pqrs_xml_compiler);
  }
}

- (uint32_t)keycode:(NSString*)name {
  @synchronized(self) {
    return pqrs_xml_compiler_get_symbol_map_value(self.pqrs_xml_compiler, [name UTF8String]);
  }
}

- (NSString*)identifier:(uint32_t)config_index {
  @synchronized(self) {
    const char* p = pqrs_xml_compiler_get_identifier(self.pqrs_xml_compiler, config_index);
    if (!p) return nil;

    return @(p);
  }
}

- (NSString*)symbolMapName:(NSString*)type value:(uint32_t)value {
  @synchronized(self) {
    const char* p = pqrs_xml_compiler_get_symbol_map_name(self.pqrs_xml_compiler, [type UTF8String], value);
    if (!p) return nil;

    return @(p);
  }
}

- (int)config_index:(NSString*)identifier {
  @synchronized(self) {
    return pqrs_xml_compiler_get_config_index(self.pqrs_xml_compiler, [identifier UTF8String]);
  }
}

- (NSString*)overrideBundleIdentifier:(NSString*)bundleIdentifier windowName:(NSString*)windowName uiElementRole:(NSString*)uiElementRole {
  @synchronized(self) {
    const char* override_bundle_identifier = pqrs_xml_compiler_override_bundle_identifier(self.pqrs_xml_compiler,
                                                                                          [bundleIdentifier UTF8String],
                                                                                          [windowName UTF8String],
                                                                                          [uiElementRole UTF8String]);
    if (override_bundle_identifier) {
      return [NSString stringWithUTF8String:override_bundle_identifier];
    }
    return nil;
  }
}

- (NSArray*)appids:(NSString*)bundleIdentifier {
  @synchronized(self) {
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

    return ids;
  }
}

- (NSArray*)windownameids:(NSString*)windowName {
  @synchronized(self) {
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

    return ids;
  }
}

- (uint32_t)uielementroleid:(NSString*)uiElementRole {
  @synchronized(self) {
    NSString* key = [NSString stringWithFormat:@"UIElementRole::%@", uiElementRole];
    return pqrs_xml_compiler_get_symbol_map_value(self.pqrs_xml_compiler, [key UTF8String]);
  }
}

- (NSArray*)inputsourceids:(NSString*)languagecode
             inputSourceID:(NSString*)inputSourceID
               inputModeID:(NSString*)inputModeID {
  @synchronized(self) {
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

    return ids;
  }
}

- (BOOL)is_vk_change_inputsource_matched:(uint32_t)keycode
                            languagecode:(NSString*)languagecode
                           inputSourceID:(NSString*)inputSourceID
                             inputModeID:(NSString*)inputModeID;
{
  @synchronized(self) {
    return pqrs_xml_compiler_is_vk_change_inputsource_matched(self.pqrs_xml_compiler,
                                                              keycode,
                                                              [languagecode UTF8String],
                                                              [inputSourceID UTF8String],
                                                              [inputModeID UTF8String]);
  }
}

- (NSString*)url:(uint32_t)keycode {
  @synchronized(self) {
    const char* p = pqrs_xml_compiler_get_url(self.pqrs_xml_compiler, keycode);
    if (!p) return nil;

    return @(p);
  }
}

- (NSString*)urlType:(uint32_t)keycode {
  @synchronized(self) {
    const char* p = pqrs_xml_compiler_get_url_type(self.pqrs_xml_compiler, keycode);
    if (!p) return nil;

    return @(p);
  }
}

- (BOOL)urlIsBackground:(uint32_t)keycode {
  @synchronized(self) {
    return pqrs_xml_compiler_get_url_background(self.pqrs_xml_compiler, keycode);
  }
}

- (NSString*)preferencepane_error_message;
{
  @synchronized(self) {
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
}

- (CheckboxItem*)getCheckboxItem:(NSNumber*)id {
  if (!id) return nil;
  XMLCompilerTree* tree = self.xmlCompilerTreeDictionary[id];
  return tree ? [tree.node castToCheckboxItem] : nil;
}

- (ParameterItem*)getParameterItem:(NSNumber*)id {
  if (!id) return nil;
  XMLCompilerTree* tree = self.xmlCompilerTreeDictionary[id];
  return tree ? [tree.node castToParameterItem] : nil;
}

- (SharedXMLCompilerTree*)narrowedSharedCheckboxTree:(BOOL)isEnabledOnly strings:(NSArray*)strings {
  return [self narrowedSharedCheckboxTree:self.preferencepane_checkbox isEnabledOnly:isEnabledOnly strings:strings];
}

- (SharedXMLCompilerTree*)narrowedSharedCheckboxTree:(XMLCompilerTree*)tree isEnabledOnly:(BOOL)isEnabledOnly strings:(NSArray*)strings {
  // check strings
  BOOL stringsMatched = YES;
  if (strings) {
    // Remove matched strings from strings for children.
    //
    // For example:
    //   strings == @[@"Emacs", @"Mode", @"Tab"]
    //
    //   * Emacs Mode
    //     * Control+I to Tab
    //
    //   notMatchedStrings == @[@"Tab"] at "Emacs Mode".
    //   Then "Control+I to Tab" will be matched by strings == @[@"Tab"].

    NSMutableArray* notMatchedStrings = nil;
    for (NSString* s in strings) {
      CheckboxItem* checkboxItem = [tree.node castToCheckboxItem];
      if (![checkboxItem isNameMatched:s]) {
        stringsMatched = NO;
      } else {
        if (!notMatchedStrings) {
          notMatchedStrings = [NSMutableArray arrayWithArray:strings];
        }
        [notMatchedStrings removeObject:s];
      }
    }

    if (notMatchedStrings) {
      strings = notMatchedStrings;
    }
  }

  // ------------------------------------------------------------
  // check children
  NSMutableArray* newchildren = [NSMutableArray new];
  for (XMLCompilerTree* child in tree.children) {
    SharedXMLCompilerTree* t = [self narrowedSharedCheckboxTree:child isEnabledOnly:isEnabledOnly strings:strings];
    if (t) {
      [newchildren addObject:t];
    }
  }

  if ([newchildren count] > 0) {
    return [[SharedXMLCompilerTree alloc] initWithId:tree.node.id children:newchildren];
  }

  // ------------------------------------------------------------
  // filter by isEnabledOnly
  if (isEnabledOnly) {
    CheckboxItem* checkboxItem = [tree.node castToCheckboxItem];
    NSString* identifier = [checkboxItem getIdentifier];
    if ([identifier length] == 0) {
      return nil;
    }
    if (![self.preferencesManager value:identifier]) {
      return nil;
    }
  }

  // check strings
  if (!stringsMatched) {
    return nil;
  }

  return [[SharedXMLCompilerTree alloc] initWithId:tree.node.id children:nil];
}

@end
