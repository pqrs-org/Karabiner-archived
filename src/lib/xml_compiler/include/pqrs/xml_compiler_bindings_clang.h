#ifndef PQRS_XML_COMPILER_BINDINGS_CLANG_H
#define PQRS_XML_COMPILER_BINDINGS_CLANG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void pqrs_xml_compiler;
typedef void pqrs_xml_compiler_preferences_checkbox_node_tree;
typedef void pqrs_xml_compiler_preferences_number_node_tree;

// ------------------------------------------------------------
int pqrs_xml_compiler_initialize(pqrs_xml_compiler** out,
                                 const char* system_xml_directory,
                                 const char* private_xml_directory);
void pqrs_xml_compiler_terminate(pqrs_xml_compiler** out);

// ------------------------------------------------------------
void pqrs_xml_compiler_reload(pqrs_xml_compiler* p, const char* checkbox_xml_file_name);

// ------------------------------------------------------------
const char* pqrs_xml_compiler_get_error_message(const pqrs_xml_compiler* p);
size_t pqrs_xml_compiler_get_error_count(const pqrs_xml_compiler* p);

// ------------------------------------------------------------
uint32_t pqrs_xml_compiler_get_symbol_map_value(const pqrs_xml_compiler* p, const char* name);
const char* pqrs_xml_compiler_get_symbol_map_name(const pqrs_xml_compiler* p, const char* type, uint32_t value);
const char* pqrs_xml_compiler_get_identifier(const pqrs_xml_compiler* p, int config_index);
int pqrs_xml_compiler_get_config_index(const pqrs_xml_compiler* p, const char* identifier);
size_t pqrs_xml_compiler_get_app_vector_size(const pqrs_xml_compiler* p);
size_t pqrs_xml_compiler_get_inputsource_vector_size(const pqrs_xml_compiler* p);
size_t pqrs_xml_compiler_get_window_name_vector_size(const pqrs_xml_compiler* p);
bool pqrs_xml_compiler_is_app_matched(const pqrs_xml_compiler* p, uint32_t* appid, size_t index, const char* application_identifier);
bool pqrs_xml_compiler_is_inputsource_matched(const pqrs_xml_compiler* p,
                                              uint32_t* inputsource,
                                              size_t index,
                                              const char* languagecode,
                                              const char* inputsourceid,
                                              const char* inputmodeid);
bool pqrs_xml_compiler_is_window_name_matched(const pqrs_xml_compiler* p, uint32_t* windownameid, size_t index, const char* window_name);
uint32_t pqrs_xml_compiler_is_vk_change_inputsource_matched(const pqrs_xml_compiler* p,
                                                            uint32_t keycode,
                                                            const char* languagecode,
                                                            const char* inputsourceid,
                                                            const char* inputmodeid);
const char* pqrs_xml_compiler_get_url(const pqrs_xml_compiler* p, uint32_t keycode);
const char* pqrs_xml_compiler_get_url_type(const pqrs_xml_compiler* p, uint32_t keycode);
bool pqrs_xml_compiler_get_url_background(const pqrs_xml_compiler* p, uint32_t keycode);

// ------------------------------------------------------------
const uint32_t* pqrs_xml_compiler_get_remapclasses_initialize_vector_data(const pqrs_xml_compiler* p);
size_t pqrs_xml_compiler_get_remapclasses_initialize_vector_size(const pqrs_xml_compiler* p);
uint32_t pqrs_xml_compiler_get_remapclasses_initialize_vector_config_count(const pqrs_xml_compiler* p);

// ------------------------------------------------------------
size_t pqrs_xml_compiler_get_preferences_checkbox_node_tree_children_count(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size);
const char* pqrs_xml_compiler_get_preferences_checkbox_node_tree_name(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size);
const char* pqrs_xml_compiler_get_preferences_checkbox_node_tree_style(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size);
bool pqrs_xml_compiler_is_preferences_checkbox_node_tree_name_icontains(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size, const char* string);
const char* pqrs_xml_compiler_get_preferences_checkbox_node_tree_identifier(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size);

// ------------------------------------------------------------
size_t pqrs_xml_compiler_get_preferences_number_node_tree_children_count(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size);
const char* pqrs_xml_compiler_get_preferences_number_node_tree_name(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size);
const char* pqrs_xml_compiler_get_preferences_number_node_tree_identifier(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size);
int pqrs_xml_compiler_get_preferences_number_node_tree_default_value(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size);
int pqrs_xml_compiler_get_preferences_number_node_tree_step(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size);
const char* pqrs_xml_compiler_get_preferences_number_node_tree_base_unit(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size);

#ifdef __cplusplus
}
#endif

#endif
