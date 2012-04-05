#include "pqrs/xml_compiler.hpp"
#include "pqrs/xml_compiler_bindings_clang.h"

typedef std::tr1::shared_ptr<pqrs::xml_compiler> xml_compiler_ptr;

struct pqrs_xml_compiler {
  xml_compiler_ptr xml_compiler_ptr;
};

int
pqrs_xml_compiler_initialize(struct pqrs_xml_compiler** out,
                             const char* system_xml_directory,
                             const char* private_xml_directory)
{
  if (! out) return -1;
  // return if already initialized.
  if (*out) return -1;

  *out = new pqrs_xml_compiler();
  (*out)->xml_compiler_ptr = xml_compiler_ptr(new pqrs::xml_compiler(system_xml_directory, private_xml_directory));

  return 0;
}

void
pqrs_xml_compiler_terminate(struct pqrs_xml_compiler** out)
{
  if (out && *out) {
    delete *out;
    *out = NULL;
  }
}

void
pqrs_xml_compiler_reload(struct pqrs_xml_compiler* p)
{
  if (! p) return;
  if (! p->xml_compiler_ptr) return;

  p->xml_compiler_ptr->reload();
}

const char*
pqrs_xml_compiler_get_error_message(const struct pqrs_xml_compiler* p)
{
  if (! p) return NULL;
  if (! p->xml_compiler_ptr) return NULL;

  return (p->xml_compiler_ptr->get_error_message()).c_str();
}

int
pqrs_xml_compiler_get_error_count(const struct pqrs_xml_compiler* p)
{
  if (! p) return NULL;
  if (! p->xml_compiler_ptr) return NULL;

  return p->xml_compiler_ptr->get_error_count();
}

const uint32_t*
pqrs_xml_compiler_get_remapclasses_initialize_vector_data(const struct pqrs_xml_compiler* p)
{
  if (! p) return NULL;
  if (! p->xml_compiler_ptr) return NULL;

  auto v = (p->xml_compiler_ptr->get_remapclasses_initialize_vector()).get();
  return &(v[0]);
}

size_t
pqrs_xml_compiler_get_remapclasses_initialize_vector_size(const struct pqrs_xml_compiler* p)
{
  if (! p) return NULL;
  if (! p->xml_compiler_ptr) return NULL;

  auto v = (p->xml_compiler_ptr->get_remapclasses_initialize_vector()).get();
  return v.size();
}
