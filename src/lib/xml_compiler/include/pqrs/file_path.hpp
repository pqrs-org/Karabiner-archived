#ifndef PQRS_FILE_PATH_HPP
#define PQRS_FILE_PATH_HPP

#include <string>

namespace pqrs {
  namespace file_path {
    std::string combine(const std::string& path1,
                        const std::string& path2);

    std::string dirname(const std::string& path);

    // For example: /foo//bar/../baz -> /foo/baz
    void normalize(std::string& path);
  }
}

#endif
