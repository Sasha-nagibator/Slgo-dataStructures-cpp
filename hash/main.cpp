#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "hash.h"
#include "doctest/doctest.h"

using namespace lab618;

class TestClass {
public:
    TestClass() = default;
    TestClass(unsigned int x, int y) : x(x), y(y) {}
    TestClass& operator=(const TestClass& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
    ~TestClass() = default;
    unsigned int x = 0;
    int y = 0;
};

int CmpTestClass(const TestClass *first, const TestClass *second) {
    return static_cast<int>(first->x - second->x);
}

unsigned int hashTestClass(const TestClass *x) {
    return x->x;
}

unsigned int hash(const unsigned int *x) {
    return *x;
}

int Cmp(const unsigned int *first, const unsigned int *second) {
    return static_cast<int>(*first - *second);
}

TEST_CASE("Adding elements") {
CHash hashTable = CHash<unsigned int, hash, Cmp>(100, 100);
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

TEST_CASE("Updating elements") {
CHash hashTable = CHash<unsigned int, hash, Cmp>(100, 100);
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

TEST_CASE("Removing elements") {
CHash hashTable = CHash<unsigned int, hash, Cmp>(100, 100);
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
CHECK(hashTable.add(data + i) == false);
}
for (unsigned int i = 0; i < amount / 2; ++i) {
CHECK(hashTable.add(data + i) == true);
}
delete[] data;
delete[] data_check;
}

TEST_CASE("Update and Add with Class with untracked fields") {
CHash hashTable = CHash<TestClass, hashTestClass, CmpTestClass>(100, 100);
int amount = 1000;
int y = 1;
TestClass *data = new TestClass[amount];
TestClass *data_check = new TestClass[amount];
for (unsigned int i = 0; i < amount; ++i) {
data[i] = TestClass(i, y);
data_check[i] = TestClass(i, y);
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

struct TestData
{
    int key = 0;
    int value = 0;
};

unsigned int hashFunc(const TestData* pElement) {
    unsigned int hash = 0;
    for (size_t i = 0; i < pElement->key; i++)
    {
        hash = hash * 31 + (*pElement).key;
    }
    return hash;
}

int compare(const TestData* pElement, const TestData* pElement2) {
    return pElement->key - pElement2->key;
}

std::string generateRandomString(int length) {
    srand(time(NULL));
    std::string result;
    for (int i = 0; i < length; i++) {
        char c = rand() % 26 + 'a';
        result += c;
    }
    return result;
}





TEST_CASE("CHashTest - AddAndGet")
{
    lab618::CHash<TestData, hashFunc, compare> hash(10, 4);

    TestData data1 = { 1, 11 };
    TestData data2 = { 2, 22 };
    TestData data3 = { 11, 33 };

    CHECK(hash.add(&data1));
    CHECK(hash.add(&data2));
    CHECK(hash.add(&data3));

    CHECK(&data1 == hash.find(data1));
    CHECK(&data2 == hash.find(data2));
    CHECK(&data3 == hash.find(data3));
    CHECK(nullptr == hash.find(TestData{ 4, 44 }));
}

TEST_CASE("CHashTest - Update")
{
    lab618::CHash<TestData, hashFunc, compare> hash(10, 32);

    TestData data1 = { 1, 11 };
    TestData data2 = { 2, 22 };

    CHECK_FALSE(hash.update(&data1));
    CHECK_FALSE(hash.update(&data2));

    data1.value = 111;

    CHECK(hash.update(&data1));
    CHECK(&data1 == hash.find(data1));
    CHECK(nullptr == hash.find(TestData{ 4, 44 }));
}

TEST_CASE("CHash stress test") {
    const int N = 1000;
    lab618::CHash<TestData, hashFunc, compare> hash(10, 32);

    std::vector<TestData> vec_str(N);
    for (int i = 0; i < N; ++i) {

        vec_str[i] = TestData{i % 5, i % 7};
        hash.add(&vec_str[i]);
    }

    unsigned found = 0;
    for (auto &str : vec_str) {
        auto *ptr = hash.find(str);
        if (ptr != nullptr) {
            ++found;
        }
    }

    CHECK(found == N);
}

/*
TEST_CASE("CHashTest - Clear")
{
    lab618::CHash<TestData, hashFunc, compare> hash(10, 32);

    TestData data1 = { 1, 11 };
    TestData data2 = { 2, 22 };

    CHECK(hash.add(&data1));
    CHECK(hash.add(&data2));

    hash.clear();
    CHECK(hash.add(&data2));
    CHECK(nullptr == hash.find(data1));
    CHECK(&data2 == hash.find(data2));
}
*/
TEST_CASE("CHashTest - Remove")
{
    lab618::CHash<TestData, hashFunc, compare> hash(10, 32);
    /*
    TestData data;
    for (int i = 0; i < 1000; ++i) {
      data = {i, i + 1};
      hash.add(&data);
    }
     */
    TestData data1 = { 1, 11 };
    TestData data2 = { 2, 22 };

    CHECK(hash.add(&data1));
    CHECK(hash.add(&data2));
    CHECK_FALSE(hash.remove(TestData{ 3, 33 }));
    CHECK(hash.remove(data1));

    CHECK(nullptr == hash.find(data1));
    CHECK(&data2 == hash.find(data2));

    /*
    for (int i = 0; i < 1000; ++i) {
      TestData data = {i, i + 1};
      TestData* found = hash.find(data);
      hash.remove(*found);
    }
     */

}

namespace {
    unsigned int dummy_hash(const std::string *str) {
        return str->size();
    }

    int dummy_compare(const std::string *str1, const std::string *str2) {
        return str1->compare(*str2);
    }
}

TEST_CASE("Basic operations") {

    lab618::CHash<std::string, dummy_hash, dummy_compare> hash_table(10, 10);

    std::string str1 = "hello";
    REQUIRE(hash_table.add(&str1));
    std::string str2 = "world";
    REQUIRE(hash_table.add(&str2));

    std::string str3 = "hello";
    bool updated = hash_table.update(&str3);
    REQUIRE(updated);

    std::string *found = hash_table.find(str1);
    REQUIRE(found != nullptr);
    CHECK(*found == str3);

    bool removed = hash_table.remove(str1);
    REQUIRE(removed);
    found = hash_table.find(str1);
    REQUIRE(found == nullptr);

    hash_table.clear();
    found = hash_table.find(str2);
    REQUIRE(found == nullptr);
}

TEST_CASE("Handling collisions") {

    lab618::CHash<std::string, dummy_hash, dummy_compare> hash_table(2, 10);

    std::string str1 = "aa";
    REQUIRE(hash_table.add(&str1));
    std::string str2 = "bb";
    REQUIRE(hash_table.add(&str2));
    std::string str3 = "cc";
    REQUIRE(hash_table.add(&str3));

    std::string *found1 = hash_table.find(str1);
    REQUIRE(found1 != nullptr);
    CHECK(*found1 == str1);
    std::string *found2 = hash_table.find(str2);
    REQUIRE(found2 != nullptr);
    CHECK(*found2 == str2);
    std::string *found3 = hash_table.find(str3);
    REQUIRE(found3 != nullptr);
    CHECK(*found3 == str3);

    bool removed = hash_table.remove(str1);
    REQUIRE(removed);
    found1 = hash_table.find(str1);
    REQUIRE(found1 == nullptr);
    found2 = hash_table.find(str2);
    REQUIRE(found2 != nullptr);
    CHECK(*found2 == str2);
    found3 = hash_table.find(str3);
    REQUIRE(found3 != nullptr);
    CHECK(*found3 == str3);
}

unsigned int intHash(const int *pKey) {
    return *pKey;
}

int intCompare(const int *pKey1, const int *pKey2) {
    return *pKey1 - *pKey2;
}

TEST_CASE("Custom unordered_map vs std::unordered_map") {

    lab618::CHash<int, intHash, intCompare> customMap(10000, 10000);
    std::unordered_map<int, int> stdMap;

    for (int i = 0; i < 100000; i++) {
        customMap.add(new int(i));
        stdMap[i] = i;
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000; i++) {
        customMap.find(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto customTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000; i++) {
        stdMap.find(i);
    }
    end = std::chrono::high_resolution_clock::now();
    auto stdTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Custom time: " << customTime.count() << std::endl;
    std::cout << "Std time: " << stdTime.count() << std::endl;
}
