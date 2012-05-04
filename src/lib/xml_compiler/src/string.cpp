#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "pqrs/string.hpp"
#include "pqrs/vector.hpp"

namespace pqrs {
  int
  string::string_by_replacing_double_curly_braces_(std::string& out, std::istream& istream, replacement replacement)
  {
    int previous = '\0';

    if (! istream.good()) {
      return -1;
    }

    for (;;) {
      int c = istream.get();
      if (! istream.good()) goto finish;

      // ----------------------------------------
      // processing {{ XXX }}
      if (c == '{' && previous == '{') {
        // --------------------
        // remove {
        out.erase(out.length() - 1, 1);

        // --------------------
        // finding }}
        std::string key;

        for (;;) {
          c = istream.get();
          if (! istream.good()) {
            // }} is not found
            goto finish;
          }

          if (c == '}' && previous == '}') {
            // remove }
            key.erase(key.length() - 1, 1);
            boost::trim(key);

            auto it = replacement.find(key);
            if (it != replacement.end()) {
              out += it->second;
            }
            break;
          }

          key.push_back(c);
          previous = c;
        }

      } else {
        out.push_back(c);
        previous = c;
      }
    }

  finish:
    return 0;
  }

  int
  string::string_from_file(std::string& out, const char* filename)
  {
    out.clear();

    // ----------------------------------------
    // Validate parameters.
    if (! filename) {
      return -1;
    }

    // ----------------------------------------
    std::ifstream istream(filename);
    if (! istream) {
      return -1;
    }

    // ----------------------------------------
    // Get length of file and call string.reserve with file length.
    istream.seekg(0, std::ios::end);
    out.reserve(istream.tellg());
    istream.seekg(0, std::ios::beg);

    for (;;) {
      int c = istream.get();
      if (! istream.good()) break;

      out.push_back(c);
    }

    return 0;
  }

  int
  string::string_by_replacing_double_curly_braces_from_file(std::string& out,
                                                            const char* filename,
                                                            replacement replacement)
  {
    out.clear();

    // ----------------------------------------
    // Validate parameters.
    if (! filename) {
      return -1;
    }

    // ----------------------------------------
    std::ifstream istream(filename);
    if (! istream) {
      return -1;
    }

    // ----------------------------------------
    // Get length of file and call string.reserve with file length.
    istream.seekg(0, std::ios::end);
    out.reserve(istream.tellg());
    istream.seekg(0, std::ios::beg);

    // ----------------------------------------
    return string_by_replacing_double_curly_braces_(out, istream, replacement);
  }

  int
  string::string_by_replacing_double_curly_braces_from_string(std::string& out,
                                                              const std::string& source,
                                                              replacement replacement)
  {
    out.clear();

    // ----------------------------------------
    // Validate parameters.
    if (source.empty()) {
      return 0;
    }

    // ----------------------------------------
    std::stringstream istream(source, std::stringstream::in);
    if (! istream) {
      return -1;
    }

    // ----------------------------------------
    out.reserve(source.length());

    // ----------------------------------------
    return string_by_replacing_double_curly_braces_(out, istream, replacement);
  }

// ============================================================
// for boost::lexical_cast
  template<typename T>
  class hex_to
  {
  public:
    operator T(void) const { return value; }
    friend std::istream& operator>>(std::istream& in, hex_to& out)
    {
      in >> std::hex >> out.value;
      return in;
    }

  private:
    T value;
  };

// for boost::lexical_cast
  template<typename T>
  class oct_to
  {
  public:
    operator T(void) const { return value; }
    friend std::istream& operator>>(std::istream& in, oct_to& out)
    {
      in >> std::oct >> out.value;
      return in;
    }

  private:
    T value;
  };

  boost::optional<uint32_t>
  string::to_uint32_t(const char* string)
  {
    if (! string) return boost::none;
    if (! *string) return boost::none;

    try {
      if (string[0] == '0') {
        // hex
        if (string[1] == 'x' || string[1] == 'X') {
          uint32_t v = boost::lexical_cast<hex_to<uint32_t> >(string);
          return v;
        }

        // oct
        uint32_t v = boost::lexical_cast<oct_to<uint32_t> >(string);
        return v;

      } else {
        // decimal
        return boost::lexical_cast<uint32_t>(string);
      }
    } catch (boost::bad_lexical_cast&) {
      return boost::none;
    }
  }

  boost::optional<uint32_t>
  string::to_uint32_t(const std::string& string)
  {
    return to_uint32_t(string.c_str());
  }

  boost::optional<uint32_t>
  string::to_uint32_t(const boost::optional<std::string>& string)
  {
    if (! string) return boost::none;
    return to_uint32_t(*string);
  }
}
