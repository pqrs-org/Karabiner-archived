#include "pqrs/xml_compiler_bindings_clang.h"
#include "pqrs/xml_compiler.hpp"
#include "pqrs/bridge.h"

int
main(void)
{
  pqrs::xml_compiler xml_compiler("/Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook.app/Contents/Resources",
                                  "/Users/tekezo/Library/Application Support/KeyRemap4MacBook");
  xml_compiler.reload();

  if (xml_compiler.get_error_count()) {
    std::cerr << "----------------------------------------" << std::endl;
    std::cerr << xml_compiler.get_error_message() << std::endl;
    std::cerr << "----------------------------------------" << std::endl;
    return 1;
  }

  auto remapclasses_initialize_vector = xml_compiler.get_remapclasses_initialize_vector();
  for (auto d : remapclasses_initialize_vector.get()) {
    std::cout << d << std::endl;
  }

  return 0;
}
