#include <algorithm>
#include <functional>
#include "pqrs/vector.hpp"

namespace pqrs {
  namespace vector {
    static bool
    is_string_empty(const std::string& s)
    {
      // If we use std::mem_fun_ref(&std::string::empty),
      // we get this error:
      //
      //   Undefined symbols for architecture x86_64:
      //     "std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::empty() const"
      //
      // libc++ does not include std::string::empty
      // because libc++ assumes that std::string::empty is always inline function.
      //
      // To avoid this error, we need to make this function.
      return s.empty();
    }

    void
    remove_empty_strings(std::vector<std::string>& v)
    {
      auto it = std::remove_if(v.begin(), v.end(), is_string_empty);
      v.erase(it, v.end());
    }

    void
    push_back(std::vector<uint32_t>& v1, const std::vector<uint32_t>& v2)
    {
      std::copy(v2.begin(), v2.end(), std::back_inserter(v1));
    }

    // ============================================================
    void
    make_combination(std::vector<std::shared_ptr<std::vector<std::string> > >& v,
                     const char* seeds[], size_t seeds_size)
    {
      typedef std::shared_ptr<std::vector<std::string> > ptr;

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
