#define CATCH_CONFIG_MAIN
#include "../../include/catch.hpp"

#include <ostream>
#include <stdexcept>
#include <unordered_map>

#include "Vector.hpp"

class TestItem final {
public:
  static int counter;

  TestItem(void) : value_(0) {
    ++counter;
    vector_.push_back(0);
    vector_.push_back(1);
    vector_.push_back(2);
    vector_.push_back(3);
  }
  TestItem(size_t v) : value_(v) {
    ++counter;
    vector_.push_back(0);
    vector_.push_back(1);
    vector_.push_back(2);
    vector_.push_back(3);
  }
  ~TestItem(void) {
    --counter;
  }

  size_t v(void) { return value_; }
  int vector(size_t index) { return vector_[index]; }

private:
  size_t value_;
  Vector_int vector_;
};
int TestItem::counter = 0;
DECLARE_VECTOR_WITH_CHUNKSIZE(TestItem, 1);

TEST_CASE("push_back", "[Vector]") {
  REQUIRE(TestItem::counter == 0);

  Vector_TestItem v;
  const size_t MAXITEM = 10;

  REQUIRE(v.empty() == true);
  REQUIRE(v.capacity() == static_cast<size_t>(0));

  for (size_t i = 0; i < MAXITEM; ++i) {
    // push twice
    REQUIRE(v.size() == i * 2);
    REQUIRE(v.capacity() == i * 2);
    v.push_back(TestItem(i)).push_back(TestItem(i));
    REQUIRE(v.size() == i * 2 + 2);
    REQUIRE(v.capacity() == i * 2 + 2);
  }

  for (size_t i = 0; i < MAXITEM; ++i) {
    REQUIRE(v[i * 2 + 0].v() == i);
    REQUIRE(v[i * 2 + 1].v() == i);

    REQUIRE(v[i * 2 + 0].vector(0) == 0);
    REQUIRE(v[i * 2 + 0].vector(1) == 1);
    REQUIRE(v[i * 2 + 0].vector(2) == 2);
    REQUIRE(v[i * 2 + 0].vector(3) == 3);

    REQUIRE(v[i * 2 + 1].vector(0) == 0);
    REQUIRE(v[i * 2 + 1].vector(1) == 1);
    REQUIRE(v[i * 2 + 1].vector(2) == 2);
    REQUIRE(v[i * 2 + 1].vector(3) == 3);
  }

  REQUIRE(!v.empty() == true);

  REQUIRE(TestItem::counter > 0);
}

TEST_CASE("frontback", "[Vector]") {
  REQUIRE(TestItem::counter == 0);

  Vector_TestItem v;

  const size_t MAXITEM = 10;
  for (size_t i = 0; i < MAXITEM; ++i) {
    v.push_back(TestItem(i));
  }

  REQUIRE(v.front().v() == static_cast<size_t>(0));
  REQUIRE(v.back().v() == static_cast<size_t>(MAXITEM - 1));
}

TEST_CASE("reserve", "[Vector]") {
  REQUIRE(TestItem::counter == 0);

  Vector_TestItem v;
  const size_t MAXITEM = 100;

  v.reserve(1000);

  REQUIRE(v.empty() == true);
  REQUIRE(v.capacity() == static_cast<size_t>(1000));

  for (size_t i = 0; i < MAXITEM; ++i) {
    // push twice
    REQUIRE(v.size() == i * 2);
    REQUIRE(v.capacity() == static_cast<size_t>(1000));
    v.push_back(TestItem(i)).push_back(TestItem(i));
    REQUIRE(v.size() == i * 2 + 2);
    REQUIRE(v.capacity() == static_cast<size_t>(1000));
  }

  for (size_t i = 0; i < MAXITEM; ++i) {
    REQUIRE(v[i * 2 + 0].v() == i);
    REQUIRE(v[i * 2 + 1].v() == i);
  }

  REQUIRE(!v.empty() == true);
}

TEST_CASE("clear", "[Vector]") {
  REQUIRE(TestItem::counter == 0);

  Vector_TestItem v;
  v.reserve(1000);
  REQUIRE(v.capacity() == static_cast<size_t>(1000));

  v.clear();
  REQUIRE(v.capacity() == static_cast<size_t>(0));

  // --------------------
  v.push_back(TestItem(1));
  v.push_back(TestItem(2));
  REQUIRE(v.capacity() == static_cast<size_t>(2));

  v.clear();
  REQUIRE(v.capacity() == static_cast<size_t>(0));
}

TEST_CASE("assignment", "[Vector]") {
  Vector_int v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  REQUIRE(v[0] == 1);
  REQUIRE(v[1] == 2);
  REQUIRE(v[2] == 3);

  v[0] = 10;
  v[1] = 20;
  v[2] = 30;

  REQUIRE(v[0] == 10);
  REQUIRE(v[1] == 20);
  REQUIRE(v[2] == 30);
}

TEST_CASE("is_include", "[Vector]") {
  Vector_int v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);

  REQUIRE(v.is_include(4) == true);
  REQUIRE(!v.is_include(6) == true);
}
