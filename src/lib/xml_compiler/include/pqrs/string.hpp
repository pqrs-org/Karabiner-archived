#ifndef PQRS_STRING_HPP
#define PQRS_STRING_HPP

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <tr1/cstdint>
#include <tr1/unordered_map>

namespace pqrs {
  class string {
  public:
    static int string_from_file(std::string& out, const char* filename);

    typedef std::tr1::unordered_map<std::string, std::string> replacement;

    static int string_by_replacing_double_curly_braces_from_file(std::string& out,
                                                                 const char* filename,
                                                                 replacement replacement);
    static int string_by_replacing_double_curly_braces_from_string(std::string& out,
                                                                   const std::string& source,
                                                                   replacement replacement);

    // octal,decimal,hex is supported.
    static boost::optional<uint32_t> to_uint32_t(const char* string) {
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

    static boost::optional<uint32_t> to_uint32_t(const std::string& string) {
      return to_uint32_t(string.c_str());
    }

    static boost::optional<uint32_t> to_uint32_t(const boost::optional<std::string>& string) {
      if (! string) return boost::none;
      return to_uint32_t(*string);
    }

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
    static void remove_whitespaces(std::string& string) {
      auto it = std::remove_if(string.begin(), string.end(), boost::is_any_of(" \n\r\t"));
      string.erase(it, string.end());
    }

    static std::string remove_whitespaces_copy(const std::string& string) {
      std::string s;
      std::back_insert_iterator<std::string> it(s);
      std::remove_copy_if(string.begin(), string.end(), it, boost::is_any_of(" \n\r\t"));
      return s;
    }

  private:
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

    static int string_by_replacing_double_curly_braces_(std::string& out,
                                                        std::istream& istream,
                                                        replacement replacement);
  };
}

#endif
