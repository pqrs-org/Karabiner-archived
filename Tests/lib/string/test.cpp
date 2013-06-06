#include <ostream>
#include <gtest/gtest.h>
#include <boost/lexical_cast.hpp>
#include "pqrs/string.hpp"

TEST(pqrs_string, string_from_file)
{
  std::string actual;
  int error = 0;
  error = pqrs::string::string_from_file(actual, "data/sample");
  EXPECT_EQ("{{AAA}} {{BBB}} {{ CCC }}\n", actual);
  EXPECT_EQ(0, error);

  error = pqrs::string::string_from_file(actual, "data/noexists");
  EXPECT_EQ("", actual);
  EXPECT_EQ(-1, error);
}

TEST(pqrs_string, string_by_replacing_double_curly_braces_from_file)
{
  pqrs::string::replacement replacement;
  replacement["AAA"] = "1";
  replacement["BBB"] = "2222";
  replacement["CCC"] = "";
  std::string replacement_warnings;

  std::string actual;
  int error = 0;
  error = pqrs::string::string_by_replacing_double_curly_braces_from_file(actual, replacement_warnings, "data/sample", replacement);
  EXPECT_EQ("1 2222 \n", actual);
  EXPECT_TRUE(replacement_warnings.empty());
  EXPECT_EQ(0, error);

  error = pqrs::string::string_by_replacing_double_curly_braces_from_file(actual, replacement_warnings, "data/noexists", replacement);
  EXPECT_EQ("", actual);
  EXPECT_TRUE(replacement_warnings.empty());
  EXPECT_EQ(-1, error);

  // performance test
  {
    for (int i = 0; i < 1000; ++i) {
      std::string key = std::string("TARGET") + boost::lexical_cast<std::string>(i);
      replacement[key] = "REPLACEMENT";
    }
    const char* filepath = "data/checkbox.xml";
    pqrs::string::string_by_replacing_double_curly_braces_from_file(actual, replacement_warnings, filepath, replacement);
  }
}

TEST(pqrs_string, string_by_replacing_double_curly_braces_from_string)
{
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
  EXPECT_EQ("abc", actual);
  EXPECT_TRUE(replacement_warnings.empty());

  // normal replacing
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, replacement_warnings, "{{AAA}} {{BBB}} !{{ CCC }}!{{ DDD }}", replacement);
  EXPECT_EQ("1 2222 !!44444444444444444444", actual);
  EXPECT_TRUE(replacement_warnings.empty());

  // unknown replacing
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, replacement_warnings, "{{UNKNOWN}}", replacement);
  EXPECT_EQ("", actual);
  EXPECT_EQ("Warning - \"UNKNOWN\" is not found in replacement.\n", replacement_warnings);
  replacement_warnings.clear();

  // "} }" is not end.
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, replacement_warnings, "{{AAA} } BBB}} XXX", replacement);
  EXPECT_EQ(" XXX", actual);
  EXPECT_EQ("Warning - \"AAA} } BBB\" is not found in replacement.\n", replacement_warnings);
  replacement_warnings.clear();

  // no }}
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, replacement_warnings, "AAA {{AAA}", replacement);
  EXPECT_EQ("AAA ", actual);
  EXPECT_TRUE(replacement_warnings.empty());

  // no }}
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, replacement_warnings, "{{ AAA }", replacement);
  EXPECT_EQ("", actual);
  EXPECT_TRUE(replacement_warnings.empty());

  // looped replacing
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, replacement_warnings, "{{ LOOP1 }}{{ LOOP2 }}", replacement);
  EXPECT_EQ("{{ LOOP1 }}   {{ LOOP2 }}    ", actual);
  EXPECT_TRUE(replacement_warnings.empty());
}

TEST(pqrs_string, to_uint32_t)
{
  boost::optional<uint32_t> actual;

  actual = pqrs::string::to_uint32_t("123456");
  EXPECT_TRUE(actual);
  EXPECT_EQ(static_cast<uint32_t>(123456), *actual);

  actual = pqrs::string::to_uint32_t("0");
  EXPECT_TRUE(actual);
  EXPECT_EQ(static_cast<uint32_t>(0), *actual);

  // oct
  actual = pqrs::string::to_uint32_t("0100");
  EXPECT_TRUE(actual);
  EXPECT_EQ(static_cast<uint32_t>(64), *actual);

  // hex
  actual = pqrs::string::to_uint32_t("0x123456");
  EXPECT_TRUE(actual);
  EXPECT_EQ(static_cast<uint32_t>(1193046), *actual);

  actual = pqrs::string::to_uint32_t("");
  EXPECT_FALSE(actual);

  actual = pqrs::string::to_uint32_t("0xG");
  EXPECT_FALSE(actual);

  actual = pqrs::string::to_uint32_t("abc");
  EXPECT_FALSE(actual);

  // boost::optional<std::string>
  actual = pqrs::string::to_uint32_t(boost::none);
  EXPECT_FALSE(actual);

  actual = pqrs::string::to_uint32_t(boost::optional<std::string>("123"));
  EXPECT_EQ(static_cast<uint32_t>(123), *actual);
}

TEST(pqrs_string, tokenizer)
{
  {
    std::string string = "A,B,,C";
    pqrs::string::tokenizer tokenizer(string, ',');
    std::string actual;

    EXPECT_TRUE(tokenizer.split_removing_empty(actual));
    EXPECT_EQ("A", actual);

    EXPECT_TRUE(tokenizer.split_removing_empty(actual));
    EXPECT_EQ("B", actual);

    EXPECT_TRUE(tokenizer.split_removing_empty(actual));
    EXPECT_EQ("C", actual);

    EXPECT_FALSE(tokenizer.split_removing_empty(actual));
  }
  {
    std::string string = ",";
    pqrs::string::tokenizer tokenizer(string, ',');
    std::string actual;

    EXPECT_FALSE(tokenizer.split_removing_empty(actual));
  }
  {
    std::string string = ",,,A,B,,C,D, ,  E E  ,,";
    pqrs::string::tokenizer tokenizer(string, ',');
    std::string actual;

    EXPECT_TRUE(tokenizer.split_removing_empty(actual));
    EXPECT_EQ("A", actual);

    EXPECT_TRUE(tokenizer.split_removing_empty(actual));
    EXPECT_EQ("B", actual);

    EXPECT_TRUE(tokenizer.split_removing_empty(actual));
    EXPECT_EQ("C", actual);

    EXPECT_TRUE(tokenizer.split_removing_empty(actual));
    EXPECT_EQ("D", actual);

    EXPECT_TRUE(tokenizer.split_removing_empty(actual));
    EXPECT_EQ(" ", actual);

    EXPECT_TRUE(tokenizer.split_removing_empty(actual));
    EXPECT_EQ("  E E  ", actual);

    EXPECT_FALSE(tokenizer.split_removing_empty(actual));
  }
}

TEST(pqrs_string, remove_whitespaces)
{
  std::string actual = " A B C \r\n \t D ";
  pqrs::string::remove_whitespaces(actual);
  EXPECT_EQ("ABCD", actual);
}
