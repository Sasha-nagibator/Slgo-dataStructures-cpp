#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "mm.h"
#include <vector>
#include <string>
#include <chrono>

using namespace lab618;


struct test_item {
    std::string str = "Hi there!";
    int num = 4711;
};


void createObjects(bool isDeleteElementsOnDestruct, bool deleteSomeObjects) {
  CMemoryManager<test_item> mm(7, isDeleteElementsOnDestruct);
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
  if (deleteSomeObjects) {
    for (int i = 0; i < 1234; ++i) {
      if (i % 3) {
        mm.deleteObject(objs[i]);
      }
    }
  }
}


void f() {
  CMemoryManager<test_item> mm(7);
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
}

TEST_CASE("Clear") {
  SUBCASE("isDeleteElementsOnDestruct == true and some elements are deleted" ) {
    bool deleteSomeObjects = true;
    CHECK_NOTHROW(createObjects(true, deleteSomeObjects));
  }
  SUBCASE("isDeleteElementsOnDestruct == true and all elements are not deleted" ) {
    bool deleteSomeObjects = false;
    CHECK_NOTHROW(createObjects(true, deleteSomeObjects));
  }

  SUBCASE("isDeleteElementsOnDestruct == false") {
    CHECK_THROWS_AS(createObjects(false, true), lab618::CMemoryManager<test_item>::CException);
    CHECK_THROWS_AS(createObjects(false, false), lab618::CMemoryManager<test_item>::CException);
    CHECK_THROWS_AS(f(), lab618::CMemoryManager<test_item>::CException);
  }
}


void checkTime(int kNumElements = 1000000) {
  std::cout << "Number of elemennts:" << kNumElements << std::endl;
  CMemoryManager<test_item> manager(10000, true);

  std::vector<test_item *> objs(kNumElements);

  auto start = std::chrono::high_resolution_clock::now();
  test_item *arr1 = new test_item[kNumElements];
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Standard method new: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds." << std::endl;


  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < kNumElements; ++i) {
    objs[i] = manager.newObject();
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout << "CMemoryManager new: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds." << std::endl;


  start = std::chrono::high_resolution_clock::now();
  delete[] arr1;
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Standard method clear: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds." << std::endl;

  start = std::chrono::high_resolution_clock::now();
  manager.clear();
  end = std::chrono::high_resolution_clock::now();
  std::cout << "CMemoryManager clear: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds."
            << std::endl << std::endl;

}

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



TEST_CASE("Check time") {
  checkTime(1000);
  checkTime(100000);
  checkTime(10000000);
}