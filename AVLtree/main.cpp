#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include "avltree.h"
#include <cstring>

struct TestStruct {
    int value;
    char str[20];
};

int compareTestStruct(const TestStruct *pElement, const TestStruct *pElement2) {
  return pElement->value - pElement2->value;
}

TEST_CASE("UNIT TEST"){
  lab618::CAVLTree<TestStruct, compareTestStruct> tree(10);
  std::vector<TestStruct> ts;
  for (int i = 0; i < 10; i++) {
    ts.push_back(TestStruct{i, NULL});
  }
  for (int i = 0; i < 10; ++i) {
    tree.add(&ts[i]);
  }
  for (int i = 0; i < 10; i++) {
    ts[i].value = i + 1;
  }
  for (int i = 0; i < 10; ++i) {
    tree.update(&ts[i]);
  }
  for (int i = 0; i < 10; ++i) {
    tree.find(ts[i]);
  }
  for (int i = 0; i < 10; ++i) {
    tree.remove(ts[i]);
  }
}


TEST_CASE("AVL add") {
  lab618::CAVLTree<TestStruct, compareTestStruct> tree(10);

  TestStruct ts1{1, "one"};
  TestStruct ts2{2, "two"};
  TestStruct ts3{3, "three"};

          REQUIRE(tree.add(&ts1) == true);
          REQUIRE(tree.add(&ts2) == true);
          REQUIRE(tree.add(&ts3) == true);
          REQUIRE(tree.add(&ts1) == false);
}

TEST_CASE("AVL find") {
  lab618::CAVLTree<TestStruct, compareTestStruct> tree(10);

  TestStruct ts1{1, "one"};
  TestStruct ts2{2, "two"};
  TestStruct ts3{3, "three"};

  tree.add(&ts1);
  tree.add(&ts2);
  tree.add(&ts3);

  TestStruct *found = tree.find(ts2);

          REQUIRE(found != nullptr);
          REQUIRE(found->value == 2);
          REQUIRE(strcmp(found->str, "two") == 0);
}

TEST_CASE("AVL update") {
  lab618::CAVLTree<TestStruct, compareTestStruct> tree(10);

  TestStruct ts1{1, "one"};
  TestStruct ts2{2, "two"};
  TestStruct ts3{3, "three"};

  tree.add(&ts1);
  tree.add(&ts2);

  TestStruct ts2_alt{2, "TWO"};
          REQUIRE(tree.update(&ts2_alt) == true);

  TestStruct *found = tree.find(ts2);

          REQUIRE(found != nullptr);
          REQUIRE(found->value == 2);
          REQUIRE(strcmp(found->str, "TWO") == 0);
}

TEST_CASE("AVL remove") {
  lab618::CAVLTree<TestStruct, compareTestStruct> tree(10);

  TestStruct ts1{1, "one"};
  TestStruct ts2{2, "two"};
  TestStruct ts3{3, "three"};

  tree.add(&ts1);
  tree.add(&ts2);
  tree.add(&ts3);

          REQUIRE(tree.remove(ts2) == true);

  TestStruct *found = tree.find(ts2);
          REQUIRE(found == nullptr);
}

TEST_CASE("AVL clear") {
  lab618::CAVLTree<TestStruct, compareTestStruct> tree(10);

  TestStruct ts1{1, "one"};
  TestStruct ts2{2, "two"};
  TestStruct ts3{3, "three"};

  tree.add(&ts1);
  tree.add(&ts2);
  tree.add(&ts3);

  tree.clear();
          REQUIRE(tree.find(ts1) == nullptr);
          REQUIRE(tree.find(ts2) == nullptr);
          REQUIRE(tree.find(ts3) == nullptr);
}

int compareInt(const int* a, const int* b)
{
  return *a - *b;
}

TEST_CASE("CAVLTreeTest - AddAndFind")
{
  lab618::CAVLTree<int, compareInt> avlTree(10);
  int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 90};
  int count = sizeof(values) / sizeof(values[0]);

  for (int i = 0; i < count; i++)
  {
    avlTree.add(&values[i]);
            CHECK(avlTree.find(values[i]) != nullptr);
  }
}

TEST_CASE("CAVLTreeTest - Update")
{
  lab618::CAVLTree<int, compareInt> avlTree(10);
  int value = 50;
  avlTree.add(&value);

  int updatedValue = 100;
          CHECK_FALSE(avlTree.update(&updatedValue));

  value = 100;
          CHECK(avlTree.update(&value));
          CHECK(avlTree.find(updatedValue) == &value);
}


TEST_CASE("CAVLTreeTest - Clear")
{
  lab618::CAVLTree<int, compareInt> avlTree(10);
  int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 90};
  int count = sizeof(values) / sizeof(values[0]);

  for (int i = 0; i < count; i++)
  {
    avlTree.add(&values[i]);
  }

  avlTree.clear();

  for (int i = 0; i < count; i++)
  {
            CHECK(avlTree.find(values[i]) == nullptr);
  }
}


TEST_CASE("CAVLTreeTest - Remove")
{
  lab618::CAVLTree<int, compareInt> avlTree(10);
  int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 90};
  int count = sizeof(values) / sizeof(values[0]);

  for (int i = 0; i < count; i++)
  {
    avlTree.add(&values[i]);
  }

  int removeValue = 90;
          CHECK(avlTree.remove(removeValue));
          CHECK(avlTree.find(removeValue) == nullptr);
  removeValue = 40;
          CHECK(avlTree.remove(removeValue));
          CHECK(avlTree.find(removeValue) == nullptr);

}


TEST_CASE("CAVLTreeTest - AddAndFind")
{
  lab618::CAVLTree<int, compareInt> avlTree(10);
  int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 90};
  int count = sizeof(values) / sizeof(values[0]);

  for (int i = 0; i < count; i++)
  {
    avlTree.add(&values[i]);
            CHECK(avlTree.find(values[i]) != nullptr);
  }
}

TEST_CASE("CAVLTreeTest - Update")
{
  lab618::CAVLTree<int, compareInt> avlTree(10);
  int value = 50;
  avlTree.add(&value);

  int updatedValue = 100;
          CHECK_FALSE(avlTree.update(&updatedValue));

  value = 100;
          CHECK(avlTree.update(&value));
          CHECK(avlTree.find(updatedValue) == &value);
}


TEST_CASE("CAVLTreeTest - Clear")
{
  lab618::CAVLTree<int, compareInt> avlTree(10);
  int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 90};
  int count = sizeof(values) / sizeof(values[0]);

  for (int i = 0; i < count; i++)
  {
    avlTree.add(&values[i]);
  }

  avlTree.clear();

  for (int i = 0; i < count; i++)
  {
            CHECK(avlTree.find(values[i]) == nullptr);
  }
}


TEST_CASE("CAVLTreeTest - Remove")
{
  lab618::CAVLTree<int, compareInt> avlTree(10);
  int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 90};
  int count = sizeof(values) / sizeof(values[0]);

  for (int i = 0; i < count; i++)
  {
    avlTree.add(&values[i]);
  }

  int removeValue = 20;
          CHECK(avlTree.remove(removeValue));
          CHECK(avlTree.find(removeValue) == nullptr);
}