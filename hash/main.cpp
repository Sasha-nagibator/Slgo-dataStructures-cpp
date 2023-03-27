#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "hash.h"

struct TestData
{
    int key = 25;
    int value = 3;
    std::string s = "asdfsdf";
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

TEST_CASE("CHash stress test") {
  const int N = 20000;
  lab618::CHash<TestData, hashFunc, compare> hash(10, 32);

  std::vector<TestData> vec_str(N);
  for (int i = 0; i < N; ++i) {
    std::string new_string = generateRandomString(32);
    vec_str[i] = TestData{i % 5, i % 7 , new_string};
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


TEST_CASE("CHashTest - AddAndGet")
{
  lab618::CHash<TestData, hashFunc, compare> hash(10, 32);

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

TEST_CASE("CHashTest - Remove")
{
  lab618::CHash<TestData, hashFunc, compare> hash(10, 32);

  TestData data1 = { 1, 11 };
  TestData data2 = { 2, 22 };

          CHECK(hash.add(&data1));
          CHECK(hash.add(&data2));
          CHECK_FALSE(hash.remove(TestData{ 3, 33 }));
          CHECK(hash.remove(data1));

          CHECK(nullptr == hash.find(data1));
          CHECK(&data2 == hash.find(data2));
}

TEST_CASE("CHashTest - Clear")
{
  lab618::CHash<TestData, hashFunc, compare> hash(10, 32);

  TestData data1 = { 1, 11 };
  TestData data2 = { 2, 22 };

          CHECK(hash.add(&data1));
          CHECK(hash.add(&data2));

  hash.clear();

          CHECK(nullptr == hash.find(data1));
          CHECK(nullptr == hash.find(data2));
}