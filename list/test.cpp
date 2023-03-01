#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "list.h"

using namespace lab618;

TEST_CASE("Push variables") {
  CSingleLinkedList<int> list;

  int a = 5;
  list.pushBack(a);
          CHECK(list.getSize() == 1);
          CHECK(list.begin().getData() == 5);

  int b = 10;
  list.pushBack(b);
          CHECK(list.getSize() == 2);
          CHECK(list.begin().getData() == 5);

  int c = 15;
  list.pushFront(c);
          CHECK(list.getSize() == 3);
          CHECK(list.begin().getData() == 15);

  int d = 20;
  list.pushFront(d);
          CHECK(list.getSize() == 4);
          CHECK(list.begin().getData() == 20);

  int e = 25;
  list.pushBack(e);
          CHECK(list.getSize() == 5);
          CHECK(list.begin().getData() == 20);
}

TEST_CASE("Pop front") {
  CSingleLinkedList<int> list;

  int a = 5;
  list.pushBack(a);
  int b = 10;
  list.pushBack(b);
  int c = 15;
  list.pushBack(c);
  int d = 20;
  list.pushBack(d);

  int front = list.popFront();
          CHECK(front == 5);
          CHECK(list.getSize() == 3);
          CHECK(list.begin().getData() == 10);

  front = list.popFront();
          CHECK(front == 10);
          CHECK(list.getSize() == 2);
          CHECK(list.begin().getData() == 15);

  front = list.popFront();
          CHECK(front == 15);
          CHECK(list.getSize() == 1);
          CHECK(list.begin().getData() == 20);

  front = list.popFront();
          CHECK(front == 20);
          CHECK(list.getSize() == 0);
}

TEST_CASE("Erase") {
  CSingleLinkedList<int> list;

  int a = 5;
  list.pushBack(a);
  int b = 10;
  list.pushBack(b);
  int c = 15;
  list.pushBack(c);
  int d = 20;
  list.pushBack(d);

  CSingleLinkedList<int>::CIterator it = list.begin();
  ++it;
  ++it;

  list.erase(it);
          CHECK(list.getSize() == 3);
          CHECK(list.begin().getData() == 5);
  ++it;
          CHECK(it.getData() == 20);

  it = list.begin();
  ++it;

  list.erase(it);
          CHECK(list.getSize() == 2);
          CHECK(list.begin().getData() == 5);
  ++it;
          CHECK(it.getData() == 20);

  it = list.begin();

  list.erase(it);
          CHECK(list.getSize() == 1);
          CHECK(list.begin().getData() == 20);

}

TEST_CASE("CDualLinkedList tests") {
  CDualLinkedList<int> list;

          SUBCASE("pushBack and getSize") {
    for (int i = 1; i < 4; ++i) {
      list.pushBack(i);
    }
            CHECK(list.getSize() == 3);
  }

          SUBCASE("popBack and getSize") {
    for (int i = 1; i < 4; ++i) {
      list.pushBack(i);
    }

            CHECK(list.popBack() == 3);
            CHECK(list.getSize() == 2);

            CHECK(list.popBack() == 2);
            CHECK(list.getSize() == 1);

            CHECK(list.popBack() == 1);
            CHECK(list.getSize() == 0);
  }

          SUBCASE("pushFront and getSize") {
    for (int i = 1; i < 4; ++i) {
      list.pushFront(i);
    }

            CHECK(list.getSize() == 3);
  }

          SUBCASE("popFront and getSize") {
    for (int i = 1; i < 4; ++i) {
      list.pushFront(i);
    }

            CHECK(list.popFront() == 3);
            CHECK(list.getSize() == 2);

            CHECK(list.popFront() == 2);
            CHECK(list.getSize() == 1);

            CHECK(list.popFront() == 1);
            CHECK(list.getSize() == 0);
  }

          SUBCASE("begin and end") {
    for (int i = 1; i < 4; ++i) {
      list.pushBack(i);
    }

    auto it = list.begin();
            CHECK(*it == 1);

    ++it;
            CHECK(*it == 2);
    ++it;
            CHECK(*it == 3);

            CHECK(!(it != list.end()));
  }

          SUBCASE("erase") {
    for (int i = 1; i < 4; ++i) {
      list.pushBack(i);
    }

    auto it = list.begin();
    ++it;

    list.erase(it);
            CHECK(list.getSize() == 2);

    it = list.begin();
            CHECK(*it == 1);

    ++it;
            CHECK(*it == 3);

            CHECK(!(it != list.end()));
  }

          SUBCASE("eraseAndNext") {
    for (int i = 1; i < 4; ++i) {
      list.pushBack(i);
    }

    auto it = list.begin();
    ++it;

    list.eraseAndNext(it);
            CHECK(list.getSize() == 2);

    it = list.begin();
            CHECK(*it == 1);

    ++it;
            CHECK(*it == 3);
            CHECK(!(it != list.end()));
    ++it;
            REQUIRE(it.getLeaf() == nullptr);
  }

          SUBCASE("clear and getSize") {
    for (int i = 1; i < 4; ++i) {
      list.pushBack(i);
    }

    list.clear();
            CHECK(list.getSize() == 0);
  }
}

TEST_CASE("Iterating through the list using the iterator") {
  CDualLinkedList<int> list;
  for (int i = 1; i < 4; ++i) {
    list.pushBack(i);
  }

          SUBCASE("Iterating forward through the list") {
    auto it = list.begin();
            REQUIRE(*it == 1);

    ++it;

            REQUIRE(*it == 2);
    ++it;

            REQUIRE(*it == 3);

            REQUIRE(!(it != list.end()));
  }

          SUBCASE("Iterating backward through the list") {
    auto it = list.end();
            REQUIRE(*it == 3);
    --it;
            REQUIRE(*it == 2);
    --it;
            REQUIRE(*it == 1);
    --it;
            REQUIRE(it.getLeaf() == nullptr);
  }
}