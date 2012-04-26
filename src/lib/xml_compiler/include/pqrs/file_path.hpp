#ifndef PQRS_FILE_PATH_HPP
#define PQRS_FILE_PATH_HPP

#include <string>

namespace pqrs {
  namespace file_path {
    std::string dirname(const std::string& path);

    // For example: /foo//bar/../baz -> /foo/baz
    void normalize(std::string& path);
  }
}

#endif
