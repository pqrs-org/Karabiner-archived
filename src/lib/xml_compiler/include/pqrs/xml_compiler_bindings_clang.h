#ifndef PQRS_XML_COMPILER_BINDINGS_CLANG_H
#define PQRS_XML_COMPILER_BINDINGS_CLANG_H

#ifdef __cplusplus
extern "C" {
#endif

struct pqrs_xml_compiler;

int pqrs_xml_compiler_initialize(struct pqrs_xml_compiler** out,
                                 const char* system_xml_directory,
                                 const char* private_xml_directory);
void pqrs_xml_compiler_terminate(struct pqrs_xml_compiler** out);

void pqrs_xml_compiler_reload(struct pqrs_xml_compiler* p);
const char* pqrs_xml_compiler_get_error_message(const struct pqrs_xml_compiler* p);
int pqrs_xml_compiler_get_error_count(const struct pqrs_xml_compiler* p);

#ifdef __cplusplus
}
#endif

#endif
