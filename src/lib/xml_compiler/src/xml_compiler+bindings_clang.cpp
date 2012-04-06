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
    *out = NULL;
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

const char*
pqrs_xml_compiler_get_error_message(const pqrs_xml_compiler* p)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return NULL;

  return (xml_compiler->get_error_message()).c_str();
}

size_t
pqrs_xml_compiler_get_error_count(const pqrs_xml_compiler* p)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return 0;

  return xml_compiler->get_error_count();
}

// ------------------------------------------------------------
const uint32_t*
pqrs_xml_compiler_get_remapclasses_initialize_vector_data(const pqrs_xml_compiler* p)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return NULL;

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

// ------------------------------------------------------------
const pqrs_xml_compiler_preferences_checkbox_node_tree*
pqrs_xml_compiler_get_preferences_checkbox_node_tree_root(const pqrs_xml_compiler* p)
{
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (! xml_compiler) return NULL;

  // ----------------------------------------
  return reinterpret_cast<const pqrs_xml_compiler_preferences_checkbox_node_tree*>((xml_compiler->get_preferences_checkbox_node_tree()).get_children().get());
}

size_t
pqrs_xml_compiler_get_preferences_checkbox_node_tree_children_count(const pqrs_xml_compiler_preferences_checkbox_node_tree* p)
{
  auto node_tree = reinterpret_cast<const pqrs::xml_compiler::preferences_node_tree<pqrs::xml_compiler::preferences_checkbox_node>*>(p);
  if (! node_tree) return 0;

  // ----------------------------------------
  auto children = node_tree->get_children();
  if (! children) return 0;
  return children->size();
}

const pqrs_xml_compiler_preferences_checkbox_node_tree*
pqrs_xml_compiler_get_preferences_checkbox_node_tree_child(const pqrs_xml_compiler_preferences_checkbox_node_tree* p,
                                                           size_t index)
{
  auto node_tree = reinterpret_cast<const pqrs::xml_compiler::preferences_node_tree<pqrs::xml_compiler::preferences_checkbox_node>*>(p);
  if (! node_tree) return 0;

  // ----------------------------------------
  auto children = node_tree->get_children();
  if (! children) return NULL;
  if (index >= children->size()) return NULL;

  auto child = (*children)[index];
  return reinterpret_cast<pqrs_xml_compiler_preferences_checkbox_node_tree*>(child.get());
}
