#define CATCH_CONFIG_MAIN
#include "../../include/catch.hpp"

#include <ostream>

#include "pqrs/file_path.hpp"

TEST_CASE("dirname", "[pqrs_file_path]") {
  REQUIRE(pqrs::file_path::dirname("/usr/bin/ls") == "/usr/bin");
  REQUIRE(pqrs::file_path::dirname("/usr/bin/ls/") == "/usr/bin");
  REQUIRE(pqrs::file_path::dirname("/usr/bin/l") == "/usr/bin");
  REQUIRE(pqrs::file_path::dirname("/usr/bin/l/") == "/usr/bin");
  REQUIRE(pqrs::file_path::dirname("/usr") == "/");
  REQUIRE(pqrs::file_path::dirname("/") == "/");
  REQUIRE(pqrs::file_path::dirname("usr/bin/ls") == "usr/bin");
  REQUIRE(pqrs::file_path::dirname("usr/bin/") == "usr");
  REQUIRE(pqrs::file_path::dirname("usr") == ".");
  REQUIRE(pqrs::file_path::dirname("usr/") == ".");
  REQUIRE(pqrs::file_path::dirname("") == ".");
}

TEST_CASE("normalize", "[pqrs_file_path]") {
  std::string file_path;

  file_path = "";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == ".");

  file_path = ".";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == ".");

  file_path = "./";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == ".");

  file_path = "..";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "..");

  file_path = "../";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "../");

  file_path = "..//foo";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "../foo");

  file_path = "abcde";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "abcde");

  file_path = "abcde/";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "abcde/");

  file_path = "/foo//bar/../baz";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "/foo/baz");

  file_path = "/../foo//bar/../baz";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "/foo/baz");

  file_path = "foo/../bar";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "bar");

  file_path = "foo/.../bar";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "foo/.../bar");

  file_path = "a/../b/../c/d";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "c/d");

  file_path = "a/./b/./c/d";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "a/b/c/d");

  file_path = "foo/bar/..";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "foo");

  file_path = "foo/bar/../";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "foo/");

  file_path = "foo/bar/.";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "foo/bar");

  file_path = "foo/bar/./";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "foo/bar/");

  file_path = "../foo/bar";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "../foo/bar");

  file_path = "../../../foo/bar";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "../../../foo/bar");

  file_path = "./foo/bar";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "foo/bar");

  file_path = "../foo/bar/..";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "../foo");

  file_path = "../foo/bar///...";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "../foo/bar/...");

  file_path = "../a/b/../c/../d///..";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "../a");

  file_path = "a/../..////../b/c";
  pqrs::file_path::normalize(file_path);
  REQUIRE(file_path == "../../b/c");
}
