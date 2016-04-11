#pragma once

#include <string>

namespace pqrs {
namespace file_path {
std::string dirname(const std::string& path);

// For example: /foo//bar/../baz -> /foo/baz
void normalize(std::string& path);
}
}
