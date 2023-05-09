#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "avltree.h"

struct TestStruct {
    int value;
    std::string str;
};

int compareTestStruct(const TestStruct *pElement, const TestStruct *pElement2) {
  return pElement->value - pElement2->value;
}

int Cmp(const unsigned int *first, const unsigned int *second) {
  return static_cast<int>(*second - *first);
}

TEST_CASE("Updating elements") {
  lab618::CAVLTree hashTable = lab618::CAVLTree<unsigned int, Cmp>(100);
  int amount = 1000;
  unsigned int *data = new unsigned int[amount];
  unsigned int *data_check = new unsigned int[amount];
  for (unsigned int i = 0; i < amount; ++i) {
    data[i] = i;
    data_check[i] = i;
            CHECK(hashTable.update(data + i) == false);
  }
  for (unsigned int i = 0; i < amount; ++i) {
            CHECK(hashTable.update(data_check + i) == true);
  }
  delete[] data;
  delete[] data_check;
}

TEST_CASE("UNIT TEST") {
lab618::CAVLTree<TestStruct, compareTestStruct> tree(1000);
std::vector<TestStruct> ts;
for (int i = 0; i < 10000; i++) {
ts.push_back(TestStruct{i, ""});
}
for (int i = 500; i < 10000; ++i) {
CHECK(tree.add(&ts[i]));
}
for (int i = 0; i < 500; ++i) {
CHECK(tree.add(&ts[i]));
}
std::vector<TestStruct> ts2 = ts;
for (int i = 0; i < 10000; i++) {
ts2[i].str = std::to_string(i);
}
for (int i = 0; i < 10000; ++i) {
CHECK(tree.update(&ts2[i]));
}
for (int i = 0; i < 10000; ++i) {
CHECK(tree.find(ts[i])->str == ts2[i].str);
}
for (int i = 500; i < 10000; ++i) {
CHECK(tree.remove(ts[i]));
}
for (int i = 0; i < 500; ++i) {
CHECK(tree.remove(ts[i]));
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
REQUIRE(found->str == "two");
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
REQUIRE(found->str == "TWO");
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



class TestClass1 {
public:
    TestClass1() = default;
    TestClass1(unsigned int x, int y) : x(x), y(y) {}
    TestClass1 &operator=(const TestClass1 &other) {
      x = other.x;
      y = other.y;
      return *this;
    }
    ~TestClass1() = default;
    unsigned int x = 0;
    int y = 0;
};

int CmpTestClass1(const TestClass1 *first, const TestClass1 *second) {
  return static_cast<int>(first->x - second->x);
}

unsigned int hashTestClass1(const TestClass1 *x) { return x->x; }

unsigned int hash(const unsigned int *x) { return *x; }



TEST_CASE("Adding elements") {
  lab618::CAVLTree hashTable = lab618::CAVLTree<unsigned int, Cmp>(100);
  int amount = 1000;
  unsigned int *data = new unsigned int[amount];
  unsigned int *data_check = new unsigned int[amount];
  for (unsigned int i = 0; i < amount; ++i) {
    data[i] = i;
    data_check[i] = i;
            CHECK(hashTable.add(data + i) == true);
  }
  for (unsigned int i = 0; i < amount; ++i) {
            CHECK(hashTable.add(data_check + i) == false);
  }
  delete[] data;
  delete[] data_check;
}



TEST_CASE("Removing elements") {
  lab618::CAVLTree hashTable = lab618::CAVLTree<unsigned int, Cmp>(100);
  int amount = 1000;
  unsigned int *data = new unsigned int[amount];
  unsigned int *data_check = new unsigned int[amount];
  for (unsigned int i = 0; i < amount; ++i) {
    data[i] = i;
    data_check[i] = i;
    hashTable.add(data + i);
  }
  for (unsigned int i = 0; i < amount / 2; ++i) {
            CHECK(hashTable.remove(data[i]) == true);
  }
  for (unsigned int i = amount / 2; i < amount; ++i) {
    //  CHECK(hashTable.add(data + i) == false);
  }
  for (unsigned int i = 0; i < amount / 2; ++i) {
    //   CHECK(hashTable.add(data + i) == true);
  }
  delete[] data;
  delete[] data_check;
}

TEST_CASE("Update and Add with Class with untracked fields") {
  lab618::CAVLTree hashTable = lab618::CAVLTree<TestClass1, CmpTestClass1>(100);
  int amount = 1000;
  int y = 1;
  TestClass1 *data = new TestClass1[amount];
  TestClass1 *data_check = new TestClass1[amount];
  for (unsigned int i = 0; i < amount; ++i) {
    data[i] = TestClass1(i, y);
    data_check[i] = TestClass1(i, y);
            CHECK(hashTable.add(data + i) == true);
  }
  for (unsigned int i = 0; i < amount; ++i) {
            CHECK(hashTable.add(data_check + i) == false);
  }
  for (unsigned int i = 0; i < amount / 2; ++i) {
    (data + i)->y = -1;
            CHECK(hashTable.update(data + i) == true);
            CHECK(hashTable.find(data[i])->y == -1);
  }
  for (unsigned int i = amount / 2; i < amount; ++i) {
            CHECK(hashTable.find(data[i])->y == 1);
  }
  delete[] data;
  delete[] data_check;
}

TEST_CASE("Adding after clear") {
  lab618::CAVLTree hashTable = lab618::CAVLTree<unsigned int, Cmp>(100);
  int amount = 1000;
  unsigned int *data = new unsigned int[amount];
  unsigned int *data_check = new unsigned int[amount];
  for (unsigned int i = 0; i < amount; ++i) {
    data[i] = i;
    data_check[i] = i;
            CHECK(hashTable.add(data + i) == true);
  }
  for (unsigned int i = 0; i < amount; ++i) {
            CHECK(hashTable.add(data_check + i) == false);
  }
  hashTable.clear();
  for (unsigned int i = 0; i < amount; ++i) {
            CHECK(hashTable.add(data + i) == true);
  }
  for (unsigned int i = 0; i < amount; ++i) {
            CHECK(hashTable.find(i) == (data + i));
  }
  delete[] data;
  delete[] data_check;
}