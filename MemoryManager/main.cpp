#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "mm.h"
#include <string>

using namespace lab618;


struct TestStruct {
    int number = 1;
    std::string str;
    double dt;

};

TEST_CASE("CMemoryManager - newObject() and deleteObject()") {
  // Создаем менеджер с размером блока 2
  lab618::CMemoryManager<TestStruct> mm(2);

          SUBCASE("Test newObject()") {
    // Проверяем выделение нового объекта
    TestStruct *p1 = mm.newObject();
            REQUIRE(p1 != nullptr);
            REQUIRE(p1->number == 1);
            REQUIRE(p1->dt == 0.0);

    // Проверяем что возвращаемый указатель не равен другому выделенному указателю
    TestStruct *p2 = mm.newObject();
            REQUIRE(p2 != nullptr);
            REQUIRE(p2 != p1);
            REQUIRE(p2->number == 1);
            REQUIRE(p2->dt == 0.0);


    for (int i = 0; i < 2; ++i) {
      mm.newObject();
    }
    TestStruct *p3 = mm.newObject();

  }

          SUBCASE("Test deleteObject() with isDeleteElementsOnDestruct = false") {
    // Проверяем удаление объекта с isDeleteElementsOnDestruct = false
    TestStruct *p1 = mm.newObject();
            REQUIRE(p1 != nullptr);
    bool result = mm.deleteObject(p1);

            REQUIRE(result == true);

    // Проверяем удаление нулевого указателя с isDeleteElementsOnDestruct = false
    bool result2 = mm.deleteObject(nullptr);
            REQUIRE(result2 == false);


  }

          SUBCASE("Test deleteObject() with isDeleteElementsOnDestruct = true") {
    // Создаем менеджер с размером блока 2 и isDeleteElementsOnDestruct = true
    lab618::CMemoryManager<TestStruct> mm2(2, true);

    // Проверяем удаление объекта с isDeleteElementsOnDestruct = true
    TestStruct *p1 = mm2.newObject();
            REQUIRE(p1 != nullptr);
    bool result = mm2.deleteObject(p1);
            REQUIRE(result == true);

    // Проверяем удаление нулевого указателя с isDeleteElementsOnDestruct = true
    bool result2 = mm2.deleteObject(nullptr);
            REQUIRE(result2 == false);

    // Проверяем удаление объекта который не был выделен менеджером
    TestStruct obj;
    bool result3 = mm2.deleteObject(&obj);
            REQUIRE(result3 == false);
  }
}
