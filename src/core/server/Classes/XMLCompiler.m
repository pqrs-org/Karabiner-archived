#import "ClientForKernelspace.h"
#import "EnvironmentChecker.h"
#import "NotificationKeys.h"
#import "XMLCompiler.h"
#include "pqrs/xml_compiler_bindings_clang.h"

@implementation XMLCompiler

// ------------------------------------------------------------
// private methods

+ (void) setStringAttributes:(NSMutableDictionary*)dict style:(NSString*)style
{
  if ([style isEqualToString:@"caution"]) {
    dict[@"stringAttributes"] = @{
      NSForegroundColorAttributeName : [NSColor blackColor],
      NSBackgroundColorAttributeName : [NSColor colorWithCalibratedRed:1.0f green:0.0f blue:0.0f alpha:0.2f],
    };

  } else if ([style isEqualToString:@"important"]) {
    dict[@"stringAttributes"] = @{
      NSForegroundColorAttributeName : [NSColor blackColor],
      NSBackgroundColorAttributeName : [NSColor colorWithCalibratedRed:0.0f green:0.0f blue:1.0f alpha:0.2f],
    };

  } else if ([style isEqualToString:@"slignt"]) {
    dict[@"stringAttributes"] = @{
      NSForegroundColorAttributeName : [NSColor grayColor],
    };
  }

  if (dict[@"stringAttributes"][NSBackgroundColorAttributeName]) {
    // append "\n" for background color.
    dict[@"name"] = [NSString stringWithFormat:@"%@\n", dict[@"name"]];
    dict[@"height"] = @([dict[@"height"] unsignedIntValue] + 1);
  }
}

+ (NSMutableArray*) build_preferencepane_checkbox:(const pqrs_xml_compiler_preferences_checkbox_node_tree*)node_tree
{
  if (! node_tree) return nil;

  size_t size = pqrs_xml_compiler_get_preferences_checkbox_node_tree_children_count(node_tree);
  if (size == 0) return nil;

  NSMutableArray* array = [[NSMutableArray new] autorelease];

  for (size_t i = 0; i < size; ++i) {
    const pqrs_xml_compiler_preferences_checkbox_node_tree* child =
      pqrs_xml_compiler_get_preferences_checkbox_node_tree_child(node_tree, i);
    if (! child) continue;

    // ----------------------------------------
    // making dictionary
    NSMutableDictionary* dict = [[NSMutableDictionary new] autorelease];

    {
      const char* name = pqrs_xml_compiler_get_preferences_checkbox_node_tree_name(child);
      if (name) {
        dict[@"name"] = @(name);
      }
    }
    {
      int name_line_count = pqrs_xml_compiler_get_preferences_checkbox_node_tree_name_line_count(child);
      dict[@"height"] = @(name_line_count);
    }
    {
      const char* identifier = pqrs_xml_compiler_get_preferences_checkbox_node_tree_identifier(child);
      if (identifier) {
        dict[@"identifier"] = @(identifier);
      }
    }
    {
      const char* name_for_filter = pqrs_xml_compiler_get_preferences_checkbox_node_tree_name_for_filter(child);
      if (name_for_filter) {
        dict[@"string_for_filter"] = @(name_for_filter);
      }
    }
    {
      const char* style = pqrs_xml_compiler_get_preferences_checkbox_node_tree_style(child);
      if (style) {
        [XMLCompiler setStringAttributes:dict style:@(style)];
      }
    }

    NSMutableArray* a = [self build_preferencepane_checkbox:child];
    if (a) {
      dict[@"children"] = a;
    }

    [array addObject:dict];
  }

  return array;
}

- (void) insert_caution_into_preferencepane_checkbox:(NSString*)message
{
  NSUInteger height = [[message componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]] count];

  NSMutableDictionary* dict = [[NSMutableDictionary new] autorelease];
  dict[@"name"] = message;
  dict[@"height"] = @(height);
  dict[@"string_for_filter"] = [message lowercaseString];

  [XMLCompiler setStringAttributes:dict style:@"caution"];

  [preferencepane_checkbox_ insertObject:dict atIndex:0];
}

+ (NSMutableArray*) build_preferencepane_number:(const pqrs_xml_compiler_preferences_number_node_tree*)node_tree
{
  if (! node_tree) return nil;

  size_t size = pqrs_xml_compiler_get_preferences_number_node_tree_children_count(node_tree);
  if (size == 0) return nil;

  NSMutableArray* array = [[NSMutableArray new] autorelease];

  for (size_t i = 0; i < size; ++i) {
    const pqrs_xml_compiler_preferences_number_node_tree* child =
      pqrs_xml_compiler_get_preferences_number_node_tree_child(node_tree, i);
    if (! child) continue;

    // ----------------------------------------
    // making dictionary
    NSMutableDictionary* dict = [[NSMutableDictionary new] autorelease];

    {
      const char* name = pqrs_xml_compiler_get_preferences_number_node_tree_name(child);
      if (name) {
        dict[@"name"] = @(name);
      }
    }
    {
      int name_line_count = pqrs_xml_compiler_get_preferences_number_node_tree_name_line_count(child);
      dict[@"height"] = @(name_line_count);
    }
    {
      const char* identifier = pqrs_xml_compiler_get_preferences_number_node_tree_identifier(child);
      if (identifier) {
        dict[@"identifier"] = @(identifier);
      }
    }
    {
      int default_value = pqrs_xml_compiler_get_preferences_number_node_tree_default_value(child);
      dict[@"default"] = @(default_value);
    }
    {
      int step = pqrs_xml_compiler_get_preferences_number_node_tree_step(child);
      dict[@"step"] = @(step);
    }
    {
      const char* base_unit = pqrs_xml_compiler_get_preferences_number_node_tree_base_unit(child);
      if (base_unit) {
        dict[@"baseunit"] = @(base_unit);
      }
    }

    NSMutableArray* a = [self build_preferencepane_number:child];
    if (a) {
      dict[@"children"] = a;
    }

    [array addObject:dict];
  }

  return array;
}

// ------------------------------------------------------------
+ (void) prepare_private_xml
{
  [XMLCompiler get_private_xml_path];
}

+ (NSString*) get_private_xml_path
{
  NSFileManager* filemanager = [NSFileManager defaultManager];
  NSArray* paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
  NSString* path = paths[0];
  path = [path stringByAppendingPathComponent:@"KeyRemap4MacBook"];
  if (! [filemanager fileExistsAtPath:path]) {
    [filemanager createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:NULL];
  }

  path = [path stringByAppendingPathComponent:@"private.xml"];
  // Copy default private.xml to user directory if private.xml is not found or filesize is 0.
  if (! [filemanager fileExistsAtPath:path] ||
      [[filemanager attributesOfItemAtPath:path error:nil] fileSize] == 0) {
    [filemanager removeItemAtPath:path error:nil];
    [filemanager copyItemAtPath:[[NSBundle mainBundle] pathForResource:@"private" ofType:@"xml"] toPath:path error:nil];

    // copyItemAtPath does not change ctime and mtime of file.
    // (For example, mtime of destination file == mtime of source file.)
    // Therefore, we need to set mtime to current time.
    NSDictionary* attr = @{ NSFileCreationDate: [NSDate date],
                            NSFileModificationDate: [NSDate date] };
    [filemanager setAttributes:attr ofItemAtPath:path error:NULL];
  }

  BOOL isDirectory;
  if ([filemanager fileExistsAtPath:path isDirectory:&isDirectory] && ! isDirectory) {
    return path;
  }

  return @"";
}

// ------------------------------------------------------------
- (id) init
{
  self = [super init];

  if (self) {
    pqrs_xml_compiler_initialize(&pqrs_xml_compiler_,
                                 [[[NSBundle mainBundle] resourcePath] UTF8String],
                                 [[[XMLCompiler get_private_xml_path] stringByDeletingLastPathComponent] UTF8String]);
  }

  return self;
}

- (void) dealloc
{
  pqrs_xml_compiler_terminate(&pqrs_xml_compiler_);
  [preferencepane_checkbox_ release];
  [preferencepane_number_ release];

  [super dealloc];
}

- (void) reload {
  @synchronized(self) {
    [XMLCompiler prepare_private_xml];

    pqrs_xml_compiler_reload(pqrs_xml_compiler_);

    // build preferencepane_checkbox_
    {
      const pqrs_xml_compiler_preferences_checkbox_node_tree* node_tree =
        pqrs_xml_compiler_get_preferences_checkbox_node_tree_root(pqrs_xml_compiler_);

      [preferencepane_checkbox_ release];
      preferencepane_checkbox_ = [XMLCompiler build_preferencepane_checkbox:node_tree];
      [preferencepane_checkbox_ retain];
    }

    // build preferencepane_number_
    {
      const pqrs_xml_compiler_preferences_number_node_tree* node_tree =
        pqrs_xml_compiler_get_preferences_number_node_tree_root(pqrs_xml_compiler_);

      [preferencepane_number_ release];
      preferencepane_number_ = [XMLCompiler build_preferencepane_number:node_tree];
      [preferencepane_number_ retain];
    }
  }

  // ------------------------------------------------------------
  if (pqrs_xml_compiler_get_error_count(pqrs_xml_compiler_) > 0) {
    NSString* message = [self preferencepane_error_message];
    [self insert_caution_into_preferencepane_checkbox:message];

    NSAlert* alert = [[NSAlert new] autorelease];
    [alert setMessageText:@"KeyRemap4MacBook Error"];
    [alert addButtonWithTitle:@"Close"];
    [alert setInformativeText:message];

    [alert runModal];
  }

  // ------------------------------------------------------------
  if ([EnvironmentChecker checkDoubleCommand]) {
    [self insert_caution_into_preferencepane_checkbox:@"A conflicting application is installed: DoubleCommand\n\nKeyRemap4MacBook ignores keyboard devices.\n(You can use KeyRemap4MacBook as a pointing device remapper.)"];
  }
  if ([EnvironmentChecker checkSmoothMouse]) {
    [self insert_caution_into_preferencepane_checkbox:@"A conflicting application is installed: SmoothMouse\n\nKeyRemap4MacBook ignores pointing devices.\n(You can use KeyRemap4MacBook as a keyboard device remapper.)"];
  }

  // ------------------------------------------------------------
  // We need to send a notification outside synchronized block to prevent lock.
  [[NSNotificationCenter defaultCenter] postNotificationName:kConfigXMLReloadedNotification object:nil];
}

- (size_t) remapclasses_initialize_vector_size
{
  @synchronized(self) {
    return pqrs_xml_compiler_get_remapclasses_initialize_vector_size(pqrs_xml_compiler_);
  }
}

- (const uint32_t*) remapclasses_initialize_vector_data
{
  @synchronized(self) {
    return pqrs_xml_compiler_get_remapclasses_initialize_vector_data(pqrs_xml_compiler_);
  }
}

- (uint32_t) remapclasses_initialize_vector_config_count
{
  @synchronized(self) {
    return pqrs_xml_compiler_get_remapclasses_initialize_vector_config_count(pqrs_xml_compiler_);
  }
}

- (uint32_t) keycode:(NSString*)name
{
  @synchronized(self) {
    return pqrs_xml_compiler_get_symbol_map_value(pqrs_xml_compiler_, [name UTF8String]);
  }
}

- (NSString*) identifier:(uint32_t)config_index
{
  @synchronized(self) {
    const char* p = pqrs_xml_compiler_get_identifier(pqrs_xml_compiler_, config_index);
    if (! p) return nil;

    return @(p);
  }
}

- (NSString*) symbolMapName:(NSString*)type value:(uint32_t)value
{
  @synchronized(self) {
    const char* p = pqrs_xml_compiler_get_symbol_map_name(pqrs_xml_compiler_, [type UTF8String], value);
    if (! p) return nil;

    return @(p);
  }
}

- (int) config_index:(NSString*)identifier
{
  @synchronized(self) {
    return pqrs_xml_compiler_get_config_index(pqrs_xml_compiler_, [identifier UTF8String]);
  }
}

- (uint32_t) appid:(NSString*)bundleIdentifier
{
  @synchronized(self) {
    return pqrs_xml_compiler_get_appid(pqrs_xml_compiler_, [bundleIdentifier UTF8String]);
  }
}

- (BOOL) is_vk_change_inputsource_matched:(uint32_t)keycode
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

- (void) inputsourceid:(uint32_t*)inputSource
  inputSourceDetail:(uint32_t*)inputSourceDetail
  languagecode:(NSString*)languagecode
  inputSourceID:(NSString*)inputSourceID
  inputModeID:(NSString*)inputModeID
{
  @synchronized(self) {
    pqrs_xml_compiler_get_inputsourceid(pqrs_xml_compiler_,
                                        inputSource,
                                        inputSourceDetail,
                                        [languagecode UTF8String],
                                        [inputSourceID UTF8String],
                                        [inputModeID UTF8String]);
  }
}

- (NSString*) url:(uint32_t)keycode
{
  @synchronized(self) {
    const char* p = pqrs_xml_compiler_get_url(pqrs_xml_compiler_, keycode);
    if (! p) return nil;

    return @(p);
  }
}

- (NSString*) urlType:(uint32_t)keycode
{
  @synchronized(self) {
    const char* p = pqrs_xml_compiler_get_url_type(pqrs_xml_compiler_, keycode);
    if (! p) return nil;

    return @(p);
  }
}

- (NSArray*) preferencepane_checkbox
{
  @synchronized(self) {
    return preferencepane_checkbox_;
  }
}

- (NSArray*) preferencepane_number;
{
  @synchronized(self) {
    return preferencepane_number_;
  }
}

- (NSString*) preferencepane_error_message;
{
  @synchronized(self) {
    if (pqrs_xml_compiler_get_error_count(pqrs_xml_compiler_) == 0) {
      return nil;
    }
    const char* error_message = pqrs_xml_compiler_get_error_message(pqrs_xml_compiler_);
    if (! error_message) {
      return nil;
    }

    return [NSString stringWithFormat:@"Error in XML.\n%s\n%s\n%s",
            "----------------------------------------",
            error_message,
            "----------------------------------------"];
  }
}

@end
