#include <fstream>
#include <iostream>
#include <sstream>
#include "pqrs/string.hpp"
#include "pqrs/vector.hpp"

namespace pqrs {
  int
  string::string_by_replacing_double_curly_braces_(std::string& out,
                                                   std::string& replacement_warnings,
                                                   std::istream& istream,
                                                   replacement replacement)
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
            } else {
              replacement_warnings += "Warning - \"" + key + "\" is not found in replacement.\n";
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
    out.reserve(static_cast<size_t>(istream.tellg()));
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
                                                            std::string& replacement_warnings,
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
    out.reserve(static_cast<size_t>(istream.tellg()));
    istream.seekg(0, std::ios::beg);

    // ----------------------------------------
    return string_by_replacing_double_curly_braces_(out, replacement_warnings, istream, replacement);
  }

  int
  string::string_by_replacing_double_curly_braces_from_string(std::string& out,
                                                              std::string& replacement_warnings,
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
    return string_by_replacing_double_curly_braces_(out, replacement_warnings, istream, replacement);
  }
}
