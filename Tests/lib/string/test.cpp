#define CATCH_CONFIG_MAIN
#include "../../include/catch.hpp"

#include <boost/lexical_cast.hpp>
#include <ostream>

#include "pqrs/string.hpp"

TEST_CASE("string_from_file", "[pqrs_string]") {
  std::string actual;
  int error = 0;
  error = pqrs::string::string_from_file(actual, "data/sample");
  REQUIRE(actual == "{{AAA}} {{BBB}} {{ CCC }}\n");
  REQUIRE(error == 0);

  error = pqrs::string::string_from_file(actual, "data/noexists");
  REQUIRE(actual == "");
  REQUIRE(error == -1);
}

TEST_CASE("string_by_replacing_double_curly_braces_from_file", "[pqrs_string]") {
  pqrs::string::replacement replacement;
  replacement["AAA"] = "1";
  replacement["BBB"] = "2222";
  replacement["CCC"] = "";
  std::string replacement_warnings;

  std::string actual;
  int error = 0;
  error = pqrs::string::string_by_replacing_double_curly_braces_from_file(actual, replacement_warnings, "data/sample", replacement);
  REQUIRE(actual == "1 2222 \n");
  REQUIRE(replacement_warnings.empty() == true);
  REQUIRE(error == 0);

  error = pqrs::string::string_by_replacing_double_curly_braces_from_file(actual, replacement_warnings, "data/noexists", replacement);
  REQUIRE(actual == "");
  REQUIRE(replacement_warnings.empty() == true);
  REQUIRE(error == -1);

  // performance test
  {
    for (int i = 0; i < 1000; ++i) {
      std::string key = std::string("TARGET") + boost::lexical_cast<std::string>(i);
      replacement[key] = "REPLACEMENT";
    }
    const char* filepath = "data/large";
    pqrs::string::string_by_replacing_double_curly_braces_from_file(actual, replacement_warnings, filepath, replacement);
  }
}

TEST_CASE("string_by_replacing_double_curly_braces_from_string", "[pqrs_string]") {
  pqrs::string::replacement replacement;
  replacement["AAA"] = "1";
  replacement["BBB"] = "2222";
  replacement["CCC"] = "";
  replacement["DDD"] = "44444444444444444444";
  replacement["LOOP1"] = "{{ LOOP1 }}";
  replacement["LOOP2"] = "   {{ LOOP2 }}    ";
  std::string replacement_warnings;

  std::string actual;

  // no replacing
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, replacement_warnings, "abc", replacement);
  REQUIRE(actual == "abc");
  REQUIRE(replacement_warnings.empty() == true);

  // normal replacing
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, replacement_warnings, "{{AAA}} {{BBB}} !{{ CCC }}!{{ DDD }}", replacement);
  REQUIRE(actual == "1 2222 !!44444444444444444444");
  REQUIRE(replacement_warnings.empty() == true);

  // unknown replacing
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, replacement_warnings, "{{UNKNOWN}}", replacement);
  REQUIRE(actual == "");
  REQUIRE(replacement_warnings == "Warning - \"UNKNOWN\" is not found in replacement.\n");
  replacement_warnings.clear();

  // "} }" is not end.
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, replacement_warnings, "{{AAA} } BBB}} XXX", replacement);
  REQUIRE(actual == " XXX");
  REQUIRE(replacement_warnings == "Warning - \"AAA} } BBB\" is not found in replacement.\n");
  replacement_warnings.clear();

  // no }}
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, replacement_warnings, "AAA {{AAA}", replacement);
  REQUIRE(actual == "AAA ");
  REQUIRE(replacement_warnings.empty() == true);

  // no }}
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, replacement_warnings, "{{ AAA }", replacement);
  REQUIRE(actual == "");
  REQUIRE(replacement_warnings.empty() == true);

  // looped replacing
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, replacement_warnings, "{{ LOOP1 }}{{ LOOP2 }}", replacement);
  REQUIRE(actual == "{{ LOOP1 }}   {{ LOOP2 }}    ");
  REQUIRE(replacement_warnings.empty() == true);
}

TEST_CASE("to_uint32_t", "[pqrs_string]") {
  boost::optional<uint32_t> actual;

  actual = pqrs::string::to_uint32_t("123456");
  REQUIRE(static_cast<bool>(actual) == true);
  REQUIRE(*actual == static_cast<uint32_t>(123456));

  actual = pqrs::string::to_uint32_t("0");
  REQUIRE(static_cast<bool>(actual) == true);
  REQUIRE(*actual == static_cast<uint32_t>(0));

  // oct
  actual = pqrs::string::to_uint32_t("0100");
  REQUIRE(static_cast<bool>(actual) == true);
  REQUIRE(*actual == static_cast<uint32_t>(64));

  // hex
  actual = pqrs::string::to_uint32_t("0x123456");
  REQUIRE(static_cast<bool>(actual) == true);
  REQUIRE(*actual == static_cast<uint32_t>(1193046));

  actual = pqrs::string::to_uint32_t("");
  REQUIRE(static_cast<bool>(actual) == false);

  actual = pqrs::string::to_uint32_t("0xG");
  REQUIRE(static_cast<bool>(actual) == false);

  actual = pqrs::string::to_uint32_t("abc");
  REQUIRE(static_cast<bool>(actual) == false);

  // boost::optional<std::string>
  actual = pqrs::string::to_uint32_t(boost::none);
  REQUIRE(static_cast<bool>(actual) == false);

  actual = pqrs::string::to_uint32_t(boost::optional<std::string>("123"));
  REQUIRE(*actual == static_cast<uint32_t>(123));
}

TEST_CASE("tokenizer", "[pqrs_string]") {
  {
    std::string string = "A,B,,C";
    pqrs::string::tokenizer tokenizer(string, ',');
    std::string actual;

    REQUIRE(tokenizer.split_removing_empty(actual) == true);
    REQUIRE(actual == "A");

    REQUIRE(tokenizer.split_removing_empty(actual) == true);
    REQUIRE(actual == "B");

    REQUIRE(tokenizer.split_removing_empty(actual) == true);
    REQUIRE(actual == "C");

    REQUIRE(tokenizer.split_removing_empty(actual) == false);
  }
  {
    std::string string = ",";
    pqrs::string::tokenizer tokenizer(string, ',');
    std::string actual;

    REQUIRE(tokenizer.split_removing_empty(actual) == false);
  }
  {
    std::string string = ",,,A,B,,C,D, ,  E E  ,,";
    pqrs::string::tokenizer tokenizer(string, ',');
    std::string actual;

    REQUIRE(tokenizer.split_removing_empty(actual) == true);
    REQUIRE(actual == "A");

    REQUIRE(tokenizer.split_removing_empty(actual) == true);
    REQUIRE(actual == "B");

    REQUIRE(tokenizer.split_removing_empty(actual) == true);
    REQUIRE(actual == "C");

    REQUIRE(tokenizer.split_removing_empty(actual) == true);
    REQUIRE(actual == "D");

    REQUIRE(tokenizer.split_removing_empty(actual) == true);
    REQUIRE(actual == " ");

    REQUIRE(tokenizer.split_removing_empty(actual) == true);
    REQUIRE(actual == "  E E  ");

    REQUIRE(tokenizer.split_removing_empty(actual) == false);
  }
}

TEST_CASE("remove_whitespaces", "[pqrs_string]") {
  std::string actual = " A B C \r\n \t D ";
  pqrs::string::remove_whitespaces(actual);
  REQUIRE(actual == "ABCD");
}
