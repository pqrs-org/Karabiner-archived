#include <ostream>
#include <gtest/gtest.h>
#include "pqrs/file_path.hpp"

TEST(pqrs_file_path, dirname)
{
  EXPECT_EQ("/usr/bin", pqrs::file_path::dirname("/usr/bin/ls"));
  EXPECT_EQ("/usr/bin", pqrs::file_path::dirname("/usr/bin/ls/"));
  EXPECT_EQ("/usr/bin", pqrs::file_path::dirname("/usr/bin/l"));
  EXPECT_EQ("/usr/bin", pqrs::file_path::dirname("/usr/bin/l/"));
  EXPECT_EQ("/", pqrs::file_path::dirname("/usr"));
  EXPECT_EQ("/", pqrs::file_path::dirname("/"));
  EXPECT_EQ("usr/bin", pqrs::file_path::dirname("usr/bin/ls"));
  EXPECT_EQ("usr", pqrs::file_path::dirname("usr/bin/"));
  EXPECT_EQ(".", pqrs::file_path::dirname("usr"));
  EXPECT_EQ(".", pqrs::file_path::dirname("usr/"));
  EXPECT_EQ(".", pqrs::file_path::dirname(""));
}

TEST(pqrs_file_path, normalize)
{
  std::string file_path;

  file_path = "";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ(".", file_path);

  file_path = ".";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ(".", file_path);

  file_path = "./";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ(".", file_path);

  file_path = "..";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("..", file_path);

  file_path = "../";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("../", file_path);

  file_path = "..//foo";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("../foo", file_path);

  file_path = "abcde";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("abcde", file_path);

  file_path = "abcde/";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("abcde/", file_path);

  file_path = "/foo//bar/../baz";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("/foo/baz", file_path);

  file_path = "/../foo//bar/../baz";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("/foo/baz", file_path);

  file_path = "foo/../bar";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("bar", file_path);

  file_path = "foo/.../bar";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("foo/.../bar", file_path);

  file_path = "a/../b/../c/d";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("c/d", file_path);

  file_path = "a/./b/./c/d";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("a/b/c/d", file_path);

  file_path = "foo/bar/..";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("foo", file_path);

  file_path = "foo/bar/../";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("foo/", file_path);

  file_path = "foo/bar/.";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("foo/bar", file_path);

  file_path = "foo/bar/./";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("foo/bar/", file_path);

  file_path = "../foo/bar";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("../foo/bar", file_path);

  file_path = "../../../foo/bar";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("../../../foo/bar", file_path);

  file_path = "./foo/bar";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("foo/bar", file_path);

  file_path = "../foo/bar/..";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("../foo", file_path);

  file_path = "../foo/bar///...";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("../foo/bar/...", file_path);

  file_path = "../a/b/../c/../d///..";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("../a", file_path);

  file_path = "a/../..////../b/c";
  pqrs::file_path::normalize(file_path);
  EXPECT_EQ("../../b/c", file_path);
}
