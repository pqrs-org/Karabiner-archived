#include "pqrs/xml_compiler.hpp"
#include "pqrs/xml_compiler_bindings_clang.h"

// ------------------------------------------------------------
int
pqrs_xml_compiler_initialize(pqrs_xml_compiler** out,
                             const char* system_xml_directory,
                             const char* private_xml_directory)
{
  if (! out) return -1;
  // return if already initialized.
  if (*out) return -1;

  *out = reinterpret_cast<pqrs_xml_compiler*>(new pqrs::xml_compiler(system_xml_directory, private_xml_directory));

  return 0;
}

void
pqrs_xml_compiler_terminate(pqrs_xml_compiler** out)
{
  if (out && *out) {
    delete reinterpret_cast<pqrs::xml_compiler*>(*out);
    *out = nullptr;
  }
}

// ------------------------------------------------------------
void
pqrs_xml_compiler_reload(pqrs_xml_compiler* p)
{
  pqrs::xml_compiler* xml_compiler = reinterpret_cast<pqrs::xml_compiler*>(p);
  if (! xml_compiler) return;

  xml_compiler->reload();
}

// ------------------------------------------------------------
const char*
pqrs_xml_compiler_get_error_message(const pqrs_xml_compiler* p)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return nullptr;

  return (xml_compiler->get_error_information()).get_message().c_str();
}

size_t
pqrs_xml_compiler_get_error_count(const pqrs_xml_compiler* p)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return 0;

  return (xml_compiler->get_error_information()).get_count();
}

// ------------------------------------------------------------
uint32_t
pqrs_xml_compiler_get_symbol_map_value(const pqrs_xml_compiler* p, const char* name)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return 0;

  auto v = (xml_compiler->get_symbol_map()).get_optional(name);
  if (! v) return 0;

  return *v;
}

const char*
pqrs_xml_compiler_get_symbol_map_name(const pqrs_xml_compiler* p, const char* type, uint32_t value)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return 0;

  auto v = (xml_compiler->get_symbol_map()).get_name(type, value);
  if (! v) return nullptr;

  return v->c_str();
}

const char*
pqrs_xml_compiler_get_identifier(const pqrs_xml_compiler* p, int config_index)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return nullptr;

  auto v = xml_compiler->get_identifier(config_index);
  if (! v) return nullptr;

  return v->c_str();
}

int
pqrs_xml_compiler_get_config_index(const pqrs_xml_compiler* p, const char* identifier)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return -1;

  auto v = xml_compiler->get_config_index(identifier);
  if (! v) return -1;

  return *v;
}

uint32_t
pqrs_xml_compiler_get_appid(const pqrs_xml_compiler* p, const char* application_identifier)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return 0;

  return xml_compiler->get_appid(application_identifier);
}

uint32_t
pqrs_xml_compiler_is_vk_change_inputsource_matched(const pqrs_xml_compiler* p,
                                                   uint32_t keycode,
                                                   const char* languagecode,
                                                   const char* inputsourceid,
                                                   const char* inputmodeid)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return 0;

  return xml_compiler->is_vk_change_inputsource_matched(keycode,
                                                        languagecode  != nullptr ? languagecode  : "",
                                                        inputsourceid != nullptr ? inputsourceid : "",
                                                        inputmodeid   != nullptr ? inputmodeid   : "");
}

void
pqrs_xml_compiler_get_inputsourceid(const pqrs_xml_compiler* p,
                                    uint32_t* inputsource,
                                    uint32_t* inputsource_detail,
                                    const char* languagecode,
                                    const char* inputsourceid,
                                    const char* inputmodeid)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return;

  if (! inputsource) return;
  if (! inputsource_detail) return;

  return xml_compiler->get_inputsourceid(*inputsource,
                                         *inputsource_detail,
                                         languagecode  != nullptr ? languagecode  : "",
                                         inputsourceid != nullptr ? inputsourceid : "",
                                         inputmodeid   != nullptr ? inputmodeid   : "");
}

const char*
pqrs_xml_compiler_get_url(const pqrs_xml_compiler* p, uint32_t keycode)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return nullptr;

  auto v = (xml_compiler->get_url(keycode));
  if (! v) return nullptr;

  return v->c_str();
}

// ------------------------------------------------------------
const uint32_t*
pqrs_xml_compiler_get_remapclasses_initialize_vector_data(const pqrs_xml_compiler* p)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return nullptr;

  auto v = (xml_compiler->get_remapclasses_initialize_vector()).get();
  return &(v[0]);
}

size_t
pqrs_xml_compiler_get_remapclasses_initialize_vector_size(const pqrs_xml_compiler* p)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return 0;

  auto v = (xml_compiler->get_remapclasses_initialize_vector()).get();
  return v.size();
}

uint32_t
pqrs_xml_compiler_get_remapclasses_initialize_vector_config_count(const pqrs_xml_compiler* p)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return 0;

  return (xml_compiler->get_remapclasses_initialize_vector()).get_config_count();
}

// ------------------------------------------------------------
const pqrs_xml_compiler_preferences_checkbox_node_tree*
pqrs_xml_compiler_get_preferences_checkbox_node_tree_root(const pqrs_xml_compiler* p)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return nullptr;

  return reinterpret_cast<const pqrs_xml_compiler_preferences_checkbox_node_tree*>(&(xml_compiler->get_preferences_checkbox_node_tree()));
}

static const pqrs::xml_compiler::preferences_node_tree<pqrs::xml_compiler::preferences_checkbox_node>*
cast_to_preferences_checkbox_node_tree(const pqrs_xml_compiler_preferences_checkbox_node_tree* p)
{
  return reinterpret_cast<const pqrs::xml_compiler::preferences_node_tree<pqrs::xml_compiler::preferences_checkbox_node>*>(p);
}

size_t
pqrs_xml_compiler_get_preferences_checkbox_node_tree_children_count(const pqrs_xml_compiler_preferences_checkbox_node_tree* p)
{
  auto node_tree = cast_to_preferences_checkbox_node_tree(p);
  if (! node_tree) return 0;

  auto children = node_tree->get_children();
  if (! children) return 0;

  return children->size();
}

const pqrs_xml_compiler_preferences_checkbox_node_tree*
pqrs_xml_compiler_get_preferences_checkbox_node_tree_child(const pqrs_xml_compiler_preferences_checkbox_node_tree* p,
                                                           size_t index)
{
  auto node_tree = cast_to_preferences_checkbox_node_tree(p);
  if (! node_tree) return nullptr;

  auto children = node_tree->get_children();
  if (! children) return nullptr;
  if (index >= children->size()) return nullptr;

  return reinterpret_cast<const pqrs_xml_compiler_preferences_checkbox_node_tree*>(((*children)[index]).get());
}

const char*
pqrs_xml_compiler_get_preferences_checkbox_node_tree_name(const pqrs_xml_compiler_preferences_checkbox_node_tree* p)
{
  auto node_tree = cast_to_preferences_checkbox_node_tree(p);
  if (! node_tree) return nullptr;

  return (node_tree->get_node()).get_name().c_str();
}

int
pqrs_xml_compiler_get_preferences_checkbox_node_tree_name_line_count(const pqrs_xml_compiler_preferences_checkbox_node_tree* p)
{
  auto node_tree = cast_to_preferences_checkbox_node_tree(p);
  if (! node_tree) return 0;

  return (node_tree->get_node()).get_name_line_count();
}

const char*
pqrs_xml_compiler_get_preferences_checkbox_node_tree_name_for_filter(const pqrs_xml_compiler_preferences_checkbox_node_tree* p)
{
  auto node_tree = cast_to_preferences_checkbox_node_tree(p);
  if (! node_tree) return nullptr;

  return (node_tree->get_node()).get_name_for_filter().c_str();
}

const char*
pqrs_xml_compiler_get_preferences_checkbox_node_tree_identifier(const pqrs_xml_compiler_preferences_checkbox_node_tree* p)
{
  auto node_tree = cast_to_preferences_checkbox_node_tree(p);
  if (! node_tree) return nullptr;

  const auto& identifier = (node_tree->get_node()).get_identifier();
  if (identifier.empty()) return nullptr;

  return identifier.c_str();
}

const char*
pqrs_xml_compiler_get_preferences_checkbox_node_tree_style(const pqrs_xml_compiler_preferences_checkbox_node_tree* p)
{
  auto node_tree = cast_to_preferences_checkbox_node_tree(p);
  if (! node_tree) return nullptr;

  return (node_tree->get_node()).get_style().c_str();
}

// ------------------------------------------------------------
const pqrs_xml_compiler_preferences_number_node_tree*
pqrs_xml_compiler_get_preferences_number_node_tree_root(const pqrs_xml_compiler* p)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return nullptr;

  return reinterpret_cast<const pqrs_xml_compiler_preferences_number_node_tree*>(&(xml_compiler->get_preferences_number_node_tree()));
}

static const pqrs::xml_compiler::preferences_node_tree<pqrs::xml_compiler::preferences_number_node>*
cast_to_preferences_number_node_tree(const pqrs_xml_compiler_preferences_number_node_tree* p)
{
  return reinterpret_cast<const pqrs::xml_compiler::preferences_node_tree<pqrs::xml_compiler::preferences_number_node>*>(p);
}

size_t
pqrs_xml_compiler_get_preferences_number_node_tree_children_count(const pqrs_xml_compiler_preferences_number_node_tree* p)
{
  auto node_tree = cast_to_preferences_number_node_tree(p);
  if (! node_tree) return 0;

  auto children = node_tree->get_children();
  if (! children) return 0;

  return children->size();
}

const pqrs_xml_compiler_preferences_number_node_tree*
pqrs_xml_compiler_get_preferences_number_node_tree_child(const pqrs_xml_compiler_preferences_number_node_tree* p,
                                                         size_t index)
{
  auto node_tree = cast_to_preferences_number_node_tree(p);
  if (! node_tree) return nullptr;

  auto children = node_tree->get_children();
  if (! children) return nullptr;
  if (index >= children->size()) return nullptr;

  return reinterpret_cast<const pqrs_xml_compiler_preferences_number_node_tree*>(((*children)[index]).get());
}

const char*
pqrs_xml_compiler_get_preferences_number_node_tree_name(const pqrs_xml_compiler_preferences_number_node_tree* p)
{
  auto node_tree = cast_to_preferences_number_node_tree(p);
  if (! node_tree) return nullptr;

  return (node_tree->get_node()).get_name().c_str();
}

int
pqrs_xml_compiler_get_preferences_number_node_tree_name_line_count(const pqrs_xml_compiler_preferences_number_node_tree* p)
{
  auto node_tree = cast_to_preferences_number_node_tree(p);
  if (! node_tree) return 0;

  return (node_tree->get_node()).get_name_line_count();
}

const char*
pqrs_xml_compiler_get_preferences_number_node_tree_identifier(const pqrs_xml_compiler_preferences_number_node_tree* p)
{
  auto node_tree = cast_to_preferences_number_node_tree(p);
  if (! node_tree) return nullptr;

  const auto& identifier = (node_tree->get_node()).get_identifier();
  if (identifier.empty()) return nullptr;

  return identifier.c_str();
}

int
pqrs_xml_compiler_get_preferences_number_node_tree_default_value(const pqrs_xml_compiler_preferences_number_node_tree* p)
{
  auto node_tree = cast_to_preferences_number_node_tree(p);
  if (! node_tree) return 0;

  return (node_tree->get_node()).get_default_value();
}

int
pqrs_xml_compiler_get_preferences_number_node_tree_step(const pqrs_xml_compiler_preferences_number_node_tree* p)
{
  auto node_tree = cast_to_preferences_number_node_tree(p);
  if (! node_tree) return 0;

  return (node_tree->get_node()).get_step();
}

const char*
pqrs_xml_compiler_get_preferences_number_node_tree_base_unit(const pqrs_xml_compiler_preferences_number_node_tree* p)
{
  auto node_tree = cast_to_preferences_number_node_tree(p);
  if (! node_tree) return nullptr;

  const auto& base_unit = (node_tree->get_node()).get_base_unit();
  if (base_unit.empty()) return nullptr;

  return base_unit.c_str();
}

int
pqrs_xml_compiler_get_preferences_number_node_tree_indent(const pqrs_xml_compiler_preferences_number_node_tree* p)
{
  auto node_tree = cast_to_preferences_number_node_tree(p);
  if (! node_tree) return 0;

  return (node_tree->get_node()).get_indent();
}
