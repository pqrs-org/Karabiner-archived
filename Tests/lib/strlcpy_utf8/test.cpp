#include <gtest/gtest.h>
#include "strlcpy_utf8.hpp"

TEST(strlcpy_utf8, ascii)
{
  char string[] = "abc";
  char buf[512];
  pqrs::strlcpy_utf8::strlcpy(buf, string, sizeof(buf));

  EXPECT_EQ(buf[0], 'a');
  EXPECT_EQ(buf[1], 'b');
  EXPECT_EQ(buf[2], 'c');
  EXPECT_EQ(buf[3], '\0');
}

TEST(strlcpy_utf8, ascii_shortbuf)
{
  char string[] = "abc";
  char buf[3];
  pqrs::strlcpy_utf8::strlcpy(buf, string, sizeof(buf));

  EXPECT_EQ(buf[0], 'a');
  EXPECT_EQ(buf[1], 'b');
  EXPECT_EQ(buf[2], '\0');
}

TEST(strlcpy_utf8, multibyte)
{
  uint8_t string[] = {
    0xd5, 0xa3, // 2byte
    0xe3, 0x81, 0x93, // 3byte
    0xe3, 0x82, 0x93, // 3byte
    0xe3, 0x81, 0xab, // 3byte
    0xe3, 0x81, 0xa1, // 3byte
    0xe3, 0x81, 0xaf, // 3byte
    0xf0, 0xa0, 0x80, 0x8b // 4byte
  };

  char buf[512];
  pqrs::strlcpy_utf8::strlcpy(buf, reinterpret_cast<char*>(string), sizeof(buf));

  EXPECT_EQ(strcmp(reinterpret_cast<char*>(string), buf), 0);
}

TEST(strlcpy_utf8, multibyte_short)
{
  uint8_t string[] = {
    0xd5, 0xa3, // 2byte
    0xe3, 0x81, 0x93, // 3byte
    0xe3, 0x82, 0x93, // 3byte
    0xe3, 0x81, 0xab, // 3byte
    0xe3, 0x81, 0xa1, // 3byte
    0xe3, 0x81, 0xaf, // 3byte
    0xf0, 0xa0, 0x80, 0x8b // 4byte
  };

  char buf[8];
  pqrs::strlcpy_utf8::strlcpy(buf, reinterpret_cast<char*>(string), sizeof(buf));

  EXPECT_EQ(buf[0], static_cast<char>(0xd5));
  EXPECT_EQ(buf[1], static_cast<char>(0xa3));
  EXPECT_EQ(buf[2], static_cast<char>(0xe3));
  EXPECT_EQ(buf[3], static_cast<char>(0x81));
  EXPECT_EQ(buf[4], static_cast<char>(0x93));
  EXPECT_EQ(buf[5], '\0');
}

TEST(strlcpy_utf8, invalid_char)
{
  uint8_t string[] = {
    0x80, // 0x10000000
  };

  char buf[512];
  pqrs::strlcpy_utf8::strlcpy(buf, reinterpret_cast<char*>(string), sizeof(buf));

  EXPECT_EQ(strcmp("", buf), 0);
}
