#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "mm.h"
#include <vector>
#include <string>

using namespace lab618;
struct test_item {
    std::string str = "Hi there!";
    int num = 4711;
};

TEST_CASE("newObject") {
CMemoryManager<test_item> mm(7, true);

test_item *p_items[10];

for (int i = 0; i < 10; ++i) {
p_items[i] = mm.newObject();
}

for (int i = 0; i < 10; ++i) {
CHECK(p_items[i]->num == 4711);
CHECK(p_items[i]->str == "Hi there!");

}
}

void createObjects(bool isDeleteElementsOnDestruct) {
  CMemoryManager<test_item> mm(31, isDeleteElementsOnDestruct);
  std::vector<test_item *> objs;
  for (int i = 0; i < 1234; ++i) {
    test_item *obj = mm.newObject();
    obj->num = i;

    obj->str = std::to_string(i - 100);
    objs.push_back(obj);
  }
  for (int i = 33; i < 1155; ++i) {
    CHECK(objs[i]->num == i);
    CHECK(objs[i]->str == std::to_string(i - 100));

  }
  for (int i = 0; i < 1233; ++i) { // последний элемент не удалится
    mm.deleteObject(objs[i]);
  }
}


TEST_CASE("Clear") {
SUBCASE("isDeleteElementsOnDestruct == true") {
CHECK_NOTHROW(createObjects(true));
}
SUBCASE("isDeleteElementsOnDestruct == false") {
CHECK_THROWS_AS(createObjects(false), lab618::CMemoryManager<test_item>::CException);
}
}



