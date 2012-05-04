#ifndef PQRS_STRING_HPP
#define PQRS_STRING_HPP

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <tr1/cstdint>
#include <tr1/unordered_map>

namespace pqrs {
  namespace string {
    int string_from_file(std::string& string, const char* filename);

    typedef std::tr1::unordered_map<std::string, std::string> replacement;

    int string_by_replacing_double_curly_braces_from_file(std::string& string,
                                                          const char* filename,
                                                          replacement replacement);
    int string_by_replacing_double_curly_braces_from_string(std::string& string,
                                                            const std::string& source,
                                                            replacement replacement);

    // octal,decimal,hex is supported.
    boost::optional<uint32_t> to_uint32_t(const char* string);
    boost::optional<uint32_t> to_uint32_t(const std::string& string);
    boost::optional<uint32_t> to_uint32_t(const boost::optional<std::string>& string);

    // tokenizer
    class tokenizer {
    public:
      tokenizer(const std::string& string, char c) :
        string_(string),
        c_(c),
        pos_(0)
      {}

      bool split_removing_empty(std::string& out) {
        auto size = string_.size();
        for (size_t i = pos_; i < size; ++i) {
          if (string_[i] == c_) {
            if (i == pos_) {
              ++pos_;
              continue;
            }

            out = string_.substr(pos_, i - pos_);
            pos_ = i + 1;
            return true;
          }
        }
        if (pos_ < size) {
          out = string_.substr(pos_);
          pos_ = size;
          return true;
        }
        return false;
      }

    private:
      const std::string& string_;
      const char c_;
      size_t pos_;
    };

    // others
    void remove_whitespaces(std::string& string);
  }
}

#endif
