#include <functional>
#include <algorithm>
#include "pqrs/vector.hpp"

namespace pqrs {
  namespace vector {
    void
    remove_empty_strings(std::vector<std::string>& v)
    {
      auto it = std::remove_if(v.begin(), v.end(), mem_fun_ref(&std::string::empty));
      v.erase(it, v.end());
    }

    void
    push_back(std::vector<uint32_t>& v1, const std::vector<uint32_t>& v2)
    {
      std::copy(v2.begin(), v2.end(), std::back_inserter(v1));
    }

    // ============================================================
    void
    make_combination(std::vector<std::tr1::shared_ptr<std::vector<std::string> > >& v,
                     const char* seeds[], size_t seeds_size)
    {
      typedef std::tr1::shared_ptr<std::vector<std::string> > ptr;

      if (seeds_size == 0) {
        ptr ptr(new std::vector<std::string>());
        v.clear();
        v.push_back(ptr);
        return;
      }

      auto last = seeds[seeds_size - 1];
      make_combination(v, seeds, seeds_size - 1);
      size_t size = v.size();
      for (size_t i = 0; i < size; ++i) {
        ptr copy(new std::vector<std::string>(*(v[i])));
        copy->push_back(last);
        v.push_back(copy);
      }
    }
  }
}
