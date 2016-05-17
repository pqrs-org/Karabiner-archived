#define CATCH_CONFIG_MAIN
#include "../../include/catch.hpp"

#include <ostream>

#include "pqrs/vector.hpp"

TEST_CASE("remove_empty_strings", "[pqrs_vector]") {
  std::vector<std::string> v;
  v.push_back("");
  v.push_back("A");
  v.push_back("");
  v.push_back("");
  v.push_back("B");
  v.push_back("");
  v.push_back("C");
  v.push_back("");
  v.push_back("");
  pqrs::vector::remove_empty_strings(v);

  std::vector<std::string> expected;
  expected.push_back("A");
  expected.push_back("B");
  expected.push_back("C");

  REQUIRE(v == expected);
}

TEST_CASE("push_back", "[pqrs_vector]") {
  std::vector<uint32_t> v1;
  v1.push_back(1);
  v1.push_back(2);
  v1.push_back(3);
  v1.push_back(4);
  v1.push_back(5);

  std::vector<uint32_t> v2;
  v2.push_back(6);
  v2.push_back(7);

  std::vector<uint32_t> v3;

  pqrs::vector::push_back(v1, v2);
  std::vector<uint32_t> expected;
  expected.push_back(1);
  expected.push_back(2);
  expected.push_back(3);
  expected.push_back(4);
  expected.push_back(5);
  expected.push_back(6);
  expected.push_back(7);
  REQUIRE(v1 == expected);

  pqrs::vector::push_back(v1, v3);
  REQUIRE(v1 == expected);
}

TEST_CASE("make_combination", "[pqrs_vector]") {
  const char* seeds[] = {"A", "B", "C", "D"};
  std::vector<std::shared_ptr<std::vector<std::string>>> actual;
  pqrs::vector::make_combination(actual, seeds, sizeof(seeds) / sizeof(seeds[0]));

  REQUIRE(actual.size() == static_cast<uint32_t>(16));

  { // 0
    std::vector<std::string> expected;
    REQUIRE(*(actual[0]) == expected);
  }
  { // 1
    std::vector<std::string> expected;
    expected.push_back("A");
    REQUIRE(*(actual[1]) == expected);
  }
  { // 2
    std::vector<std::string> expected;
    expected.push_back("B");
    REQUIRE(*(actual[2]) == expected);
  }
  { // 3
    std::vector<std::string> expected;
    expected.push_back("A");
    expected.push_back("B");
    REQUIRE(*(actual[3]) == expected);
  }
  { // 4
    std::vector<std::string> expected;
    expected.push_back("C");
    REQUIRE(*(actual[4]) == expected);
  }
  { // 5
    std::vector<std::string> expected;
    expected.push_back("A");
    expected.push_back("C");
    REQUIRE(*(actual[5]) == expected);
  }
  { // 6
    std::vector<std::string> expected;
    expected.push_back("B");
    expected.push_back("C");
    REQUIRE(*(actual[6]) == expected);
  }
  { // 7
    std::vector<std::string> expected;
    expected.push_back("A");
    expected.push_back("B");
    expected.push_back("C");
    REQUIRE(*(actual[7]) == expected);
  }
  { // 8
    std::vector<std::string> expected;
    expected.push_back("D");
    REQUIRE(*(actual[8]) == expected);
  }
  { // 9
    std::vector<std::string> expected;
    expected.push_back("A");
    expected.push_back("D");
    REQUIRE(*(actual[9]) == expected);
  }
  { // 10
    std::vector<std::string> expected;
    expected.push_back("B");
    expected.push_back("D");
    REQUIRE(*(actual[10]) == expected);
  }
  { // 11
    std::vector<std::string> expected;
    expected.push_back("A");
    expected.push_back("B");
    expected.push_back("D");
    REQUIRE(*(actual[11]) == expected);
  }
  { // 12
    std::vector<std::string> expected;
    expected.push_back("C");
    expected.push_back("D");
    REQUIRE(*(actual[12]) == expected);
  }
  { // 13
    std::vector<std::string> expected;
    expected.push_back("A");
    expected.push_back("C");
    expected.push_back("D");
    REQUIRE(*(actual[13]) == expected);
  }
  { // 14
    std::vector<std::string> expected;
    expected.push_back("B");
    expected.push_back("C");
    expected.push_back("D");
    REQUIRE(*(actual[14]) == expected);
  }
  { // 15
    std::vector<std::string> expected;
    expected.push_back("A");
    expected.push_back("B");
    expected.push_back("C");
    expected.push_back("D");
    REQUIRE(*(actual[15]) == expected);
  }
}
