#define CATCH_CONFIG_MAIN
#include "../../include/catch.hpp"

#include "strlcpy_utf8.hpp"

TEST_CASE("ascii", "[strlcpy_utf8]") {
  char string[] = "abc";
  char buf[512];
  pqrs::strlcpy_utf8::strlcpy(buf, string, sizeof(buf));

  REQUIRE(buf[0] == 'a');
  REQUIRE(buf[1] == 'b');
  REQUIRE(buf[2] == 'c');
  REQUIRE(buf[3] == '\0');
}

TEST_CASE("ascii_shortbuf", "[strlcpy_utf8]") {
  char string[] = "abc";
  char buf[3];
  pqrs::strlcpy_utf8::strlcpy(buf, string, sizeof(buf));

  REQUIRE(buf[0] == 'a');
  REQUIRE(buf[1] == 'b');
  REQUIRE(buf[2] == '\0');
}

TEST_CASE("multibyte", "[strlcpy_utf8]") {
  uint8_t string[] = {
      0xd5, 0xa3,            // 2byte
      0xe3, 0x81, 0x93,      // 3byte
      0xe3, 0x82, 0x93,      // 3byte
      0xe3, 0x81, 0xab,      // 3byte
      0xe3, 0x81, 0xa1,      // 3byte
      0xe3, 0x81, 0xaf,      // 3byte
      0xf0, 0xa0, 0x80, 0x8b // 4byte
  };

  char buf[512];
  pqrs::strlcpy_utf8::strlcpy(buf, reinterpret_cast<char*>(string), sizeof(buf));

  REQUIRE(strcmp(reinterpret_cast<char*>(string), buf) == 0);
}

TEST_CASE("multibyte_short", "[strlcpy_utf8]") {
  uint8_t string[] = {
      0xd5, 0xa3,            // 2byte
      0xe3, 0x81, 0x93,      // 3byte
      0xe3, 0x82, 0x93,      // 3byte
      0xe3, 0x81, 0xab,      // 3byte
      0xe3, 0x81, 0xa1,      // 3byte
      0xe3, 0x81, 0xaf,      // 3byte
      0xf0, 0xa0, 0x80, 0x8b // 4byte
  };

  char buf[8];
  pqrs::strlcpy_utf8::strlcpy(buf, reinterpret_cast<char*>(string), sizeof(buf));

  REQUIRE(buf[0] == static_cast<char>(0xd5));
  REQUIRE(buf[1] == static_cast<char>(0xa3));
  REQUIRE(buf[2] == static_cast<char>(0xe3));
  REQUIRE(buf[3] == static_cast<char>(0x81));
  REQUIRE(buf[4] == static_cast<char>(0x93));
  REQUIRE(buf[5] == '\0');
}

TEST_CASE("invalid_char", "[strlcpy_utf8]") {
  uint8_t string[] = {
      0x80, // 0x10000000
  };

  char buf[512];
  pqrs::strlcpy_utf8::strlcpy(buf, reinterpret_cast<char*>(string), sizeof(buf));

  REQUIRE(strcmp("", buf) == 0);
}
