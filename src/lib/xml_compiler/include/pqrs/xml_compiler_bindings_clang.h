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
void pqrs_xml_compiler_reload(pqrs_xml_compiler* p);

// ------------------------------------------------------------
const char* pqrs_xml_compiler_get_error_message(const pqrs_xml_compiler* p);
size_t pqrs_xml_compiler_get_error_count(const pqrs_xml_compiler* p);

// ------------------------------------------------------------
uint32_t pqrs_xml_compiler_get_symbol_map_value(const pqrs_xml_compiler* p, const char* name);
const char* pqrs_xml_compiler_get_identifier(const pqrs_xml_compiler* p, int config_index);

// ------------------------------------------------------------
const uint32_t* pqrs_xml_compiler_get_remapclasses_initialize_vector_data(const pqrs_xml_compiler* p);
size_t pqrs_xml_compiler_get_remapclasses_initialize_vector_size(const pqrs_xml_compiler* p);

// ------------------------------------------------------------
const pqrs_xml_compiler_preferences_checkbox_node_tree*
pqrs_xml_compiler_get_preferences_checkbox_node_tree_root(const pqrs_xml_compiler* p);

size_t
pqrs_xml_compiler_get_preferences_checkbox_node_tree_children_count(const pqrs_xml_compiler_preferences_checkbox_node_tree* p);

const pqrs_xml_compiler_preferences_checkbox_node_tree*
pqrs_xml_compiler_get_preferences_checkbox_node_tree_child(const pqrs_xml_compiler_preferences_checkbox_node_tree* p,
                                                           size_t index);

const char*
pqrs_xml_compiler_get_preferences_checkbox_node_tree_name(const pqrs_xml_compiler_preferences_checkbox_node_tree* p);

int
pqrs_xml_compiler_get_preferences_checkbox_node_tree_name_line_count(const pqrs_xml_compiler_preferences_checkbox_node_tree* p);

const char*
pqrs_xml_compiler_get_preferences_checkbox_node_tree_identifier(const pqrs_xml_compiler_preferences_checkbox_node_tree* p);

const char*
pqrs_xml_compiler_get_preferences_checkbox_node_tree_name_for_filter(const pqrs_xml_compiler_preferences_checkbox_node_tree* p);

// ------------------------------------------------------------
const pqrs_xml_compiler_preferences_number_node_tree*
pqrs_xml_compiler_get_preferences_number_node_tree_root(const pqrs_xml_compiler* p);

size_t
pqrs_xml_compiler_get_preferences_number_node_tree_children_count(const pqrs_xml_compiler_preferences_number_node_tree* p);

const pqrs_xml_compiler_preferences_number_node_tree*
pqrs_xml_compiler_get_preferences_number_node_tree_child(const pqrs_xml_compiler_preferences_number_node_tree* p,
                                                         size_t index);

const char*
pqrs_xml_compiler_get_preferences_number_node_tree_name(const pqrs_xml_compiler_preferences_number_node_tree* p);

int
pqrs_xml_compiler_get_preferences_number_node_tree_name_line_count(const pqrs_xml_compiler_preferences_number_node_tree* p);

const char*
pqrs_xml_compiler_get_preferences_number_node_tree_identifier(const pqrs_xml_compiler_preferences_number_node_tree* p);

int
pqrs_xml_compiler_get_preferences_number_node_tree_default_value(const pqrs_xml_compiler_preferences_number_node_tree* p);

int
pqrs_xml_compiler_get_preferences_number_node_tree_step(const pqrs_xml_compiler_preferences_number_node_tree* p);

const char*
pqrs_xml_compiler_get_preferences_number_node_tree_base_unit(const pqrs_xml_compiler_preferences_number_node_tree* p);

#ifdef __cplusplus
}
#endif

#endif
