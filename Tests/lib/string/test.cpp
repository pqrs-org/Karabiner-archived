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

  std::string actual;
  int error = 0;
  error = pqrs::string::string_by_replacing_double_curly_braces_from_file(actual, "data/sample", replacement);
  EXPECT_EQ("1 2222 \n", actual);
  EXPECT_EQ(0, error);

  error = pqrs::string::string_by_replacing_double_curly_braces_from_file(actual, "data/noexists", replacement);
  EXPECT_EQ("", actual);
  EXPECT_EQ(-1, error);

  // performance test
  {
    for (int i = 0; i < 1000; ++i) {
      std::string key = std::string("TARGET") + boost::lexical_cast<std::string>(i);
      replacement[key] = "REPLACEMENT";
    }
    const char* filepath = "data/checkbox.xml";
    pqrs::string::string_by_replacing_double_curly_braces_from_file(actual, filepath, replacement);
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

  std::string actual;

  // no replacing
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, "abc", replacement);
  EXPECT_EQ("abc", actual);

  // normal replacing
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, "{{AAA}} {{BBB}} !{{ CCC }}!{{ DDD }}", replacement);
  EXPECT_EQ("1 2222 !!44444444444444444444", actual);

  // unknown replacing
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, "{{UNKNOWN}}", replacement);
  EXPECT_EQ("", actual);

  // "} }" is not end.
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, "{{AAA} } BBB}} XXX", replacement);
  EXPECT_EQ(" XXX", actual);

  // no }}
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, "AAA {{AAA}", replacement);
  EXPECT_EQ("AAA ", actual);

  // no }}
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, "{{ AAA }", replacement);
  EXPECT_EQ("", actual);

  // looped replacing
  pqrs::string::string_by_replacing_double_curly_braces_from_string(actual, "{{ LOOP1 }}{{ LOOP2 }}", replacement);
  EXPECT_EQ("{{ LOOP1 }}   {{ LOOP2 }}    ", actual);
}

TEST(pqrs_string, to_uint32_t)
{
  boost::optional<uint32_t> actual;

  actual = pqrs::string::to_uint32_t("123456");
  EXPECT_TRUE(actual);
  EXPECT_EQ(static_cast<uint32_t>(123456), *actual);

  // oct
  actual = pqrs::string::to_uint32_t("0100");
  EXPECT_TRUE(actual);
  EXPECT_EQ(static_cast<uint32_t>(64), *actual);

  // hex
  actual = pqrs::string::to_uint32_t("0x123456");
  EXPECT_TRUE(actual);
  EXPECT_EQ(static_cast<uint32_t>(1193046), *actual);

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

TEST(pqrs_string, split_by_comma)
{
  std::vector<std::string> actual;
  pqrs::string::split_by_comma(actual, ", A, , B,C,D, E,  F,,,,G,");

  std::vector<std::string> expected;
  expected.push_back("A");
  expected.push_back("B");
  expected.push_back("C");
  expected.push_back("D");
  expected.push_back("E");
  expected.push_back("F");
  expected.push_back("G");

  EXPECT_EQ(expected, actual);
}

TEST(pqrs_string, split_by_pipe)
{
  std::vector<std::string> actual;
  pqrs::string::split_by_pipe(actual, "A||B|C|");

  std::vector<std::string> expected;
  expected.push_back("A");
  expected.push_back("B");
  expected.push_back("C");

  EXPECT_EQ(expected, actual);
}

TEST(pqrs_string, remove_whitespaces)
{
  std::string actual = " A B C \r\n \t D ";
  pqrs::string::remove_whitespaces(actual);
  EXPECT_EQ("ABCD", actual);
}
