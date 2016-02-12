// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import <Cocoa/Cocoa.h>
#include "pqrs/xml_compiler_bindings_clang.h"

@class ClientForKernelspace;

@interface XMLCompilerItem : NSObject
@property(readonly) NSNumber* id;
@end

@interface CheckboxItem : XMLCompilerItem
- (NSString*)getName;
- (NSString*)getStyle;
- (NSString*)getIdentifier;
- (NSUInteger)getChildrenCount;
- (BOOL)needsShowCheckbox;
- (BOOL)isNameMatched:(NSString*)string;
@end

@interface ParameterItem : XMLCompilerItem
- (NSString*)getName;
- (NSString*)getIdentifier;
- (NSInteger)getDefaultValue;
- (NSInteger)getStep;
- (NSString*)getBaseUnit;
- (NSUInteger)getChildrenCount;
@end

@interface XMLCompilerTree : NSObject
@property XMLCompilerItem* node;
@property NSArray* children;

- (CheckboxItem*)castNodeToCheckboxItem;
- (ParameterItem*)castNodeToParameterItem;
@end

@interface XMLCompiler : NSObject {
  IBOutlet ClientForKernelspace* clientForKernelspace_;
}

@property XMLCompilerTree* preferencepane_checkbox;
@property XMLCompilerTree* preferencepane_parameter;

+ (NSString*)get_private_xml_path;

- (void)reload;

- (size_t)remapclasses_initialize_vector_size;
- (const uint32_t*)remapclasses_initialize_vector_data;
- (uint32_t)remapclasses_initialize_vector_config_count;

- (uint32_t)keycode:(NSString*)name;
- (NSString*)identifier:(uint32_t)config_index;
- (NSString*)symbolMapName:(NSString*)type value:(uint32_t)value;
- (int)config_index:(NSString*)identifier;
- (NSArray*)appids:(NSString*)bundleIdentifier;
- (NSArray*)windownameids:(NSString*)windowName;
- (uint32_t)uielementroleid:(NSString*)uiElementRole;
- (NSArray*)inputsourceids:(NSString*)languagecode
             inputSourceID:(NSString*)inputSourceID
               inputModeID:(NSString*)inputModeID;
- (BOOL)is_vk_change_inputsource_matched:(uint32_t)keycode
                            languagecode:(NSString*)languagecode
                           inputSourceID:(NSString*)inputSourceID
                             inputModeID:(NSString*)inputModeID;
- (NSString*)url:(uint32_t)keycode;
- (NSString*)urlType:(uint32_t)keycode;
- (BOOL)urlIsBackground:(uint32_t)keycode;

@end
