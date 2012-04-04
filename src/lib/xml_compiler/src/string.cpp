#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "pqrs/string.hpp"
#include "pqrs/vector.hpp"

namespace pqrs {
  namespace string {
    static int
    string_by_replacing_double_curly_braces_(std::string& string, std::istream& istream, replacement replacement)
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
          string.erase(string.length() - 1, 1);

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
                string += it->second;
              }
              break;
            }

            key.push_back(c);
            previous = c;
          }

        } else {
          string.push_back(c);
          previous = c;
        }
      }

    finish:
      return 0;
    }

    int
    string_from_file(std::string& string, const char* filename)
    {
      string.clear();

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
      string.reserve(istream.tellg());
      istream.seekg(0, std::ios::beg);

      for (;;) {
        int c = istream.get();
        if (! istream.good()) break;

        string.push_back(c);
      }

      return 0;
    }

    int
    string_by_replacing_double_curly_braces_from_file(std::string& string,
                                                      const char* filename,
                                                      replacement replacement)
    {
      string.clear();

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
      string.reserve(istream.tellg());
      istream.seekg(0, std::ios::beg);

      // ----------------------------------------
      return string_by_replacing_double_curly_braces_(string, istream, replacement);
    }

    int
    string_by_replacing_double_curly_braces_from_string(std::string& string,
                                                        const std::string& source,
                                                        replacement replacement)
    {
      string.clear();

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
      string.reserve(source.length());

      // ----------------------------------------
      return string_by_replacing_double_curly_braces_(string, istream, replacement);
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
    to_uint32_t(const char* string)
    {
      if (! string) return boost::none;

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
    to_uint32_t(const std::string& string)
    {
      return to_uint32_t(string.c_str());
    }

    boost::optional<uint32_t>
    to_uint32_t(const boost::optional<std::string>& string)
    {
      if (! string) return boost::none;
      return to_uint32_t(*string);
    }

    // ============================================================
    static void
    split(std::vector<std::string>& v, std::string string, int flags, const char* delimiter)
    {
      v.clear();
      boost::split(v, string, boost::is_any_of(delimiter));

      if (flags & split_option::trim) {
        for (auto& it : v) {
          boost::trim(it);
        }
      }
      if (flags & split_option::remove_empty_strings) {
        pqrs::vector::remove_empty_strings(v);
      }
    }

    void
    split_by_comma(std::vector<std::string>& v, std::string string, int flags)
    {
      split(v, string, flags, ",");
    }

    void
    split_by_pipe(std::vector<std::string>& v, std::string string, int flags)
    {
      split(v, string, flags, "|");
    }

    // ============================================================
    void
    remove_whitespaces(std::string& string)
    {
      auto it = std::remove_if(string.begin(), string.end(), boost::is_any_of(" \n\r\t"));
      string.erase(it, string.end());
    }
  }
}
