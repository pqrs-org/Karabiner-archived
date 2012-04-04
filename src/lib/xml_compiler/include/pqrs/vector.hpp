#ifndef PQRS_VECTOR_HPP
#define PQRS_VECTOR_HPP

#include <vector>
#include <string>
#include <tr1/memory>

namespace pqrs {
  namespace vector {
    void remove_empty_strings(std::vector<std::string>& v);
    void push_back(std::vector<uint32_t>& v1, const std::vector<uint32_t>& v2);

    void make_combination(std::vector<std::tr1::shared_ptr<std::vector<std::string> > >& v,
                          const char* seeds[], size_t seeds_size);
  }
}

#endif
