#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <string>
#include <fstream>

#include "hash.h"
#include "avltree.h"
#include "sort.h"

const std::string alphabet =
        "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ01234567"
        "89abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

std::vector<int> amount(20);


std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distr7_20(7, 20);
std::uniform_int_distribution<> randSymbol(0, alphabet.size());

std::string RandStr() {
  int length = distr7_20(gen);
  std::string rand_str;
  for (int i = 0; i < length; ++i) {
    rand_str += alphabet[randSymbol(gen)];
  }
  return rand_str;
}

struct Test {
    Test() {}
    std::string first;
    std::string second;
};

using namespace lab618;
using namespace templates;

int TestCmp(const Test* a, const Test* b) {
  if (a->first == b->first) {
    if (a->second == b->second) {
      return 0;
    } else if (a->second < b->second) {
      return 1;
    }
    return -1;
  } else if (a->first < b->first) {
    return 1;
  }
  return -1;
}

unsigned int TestHash(const Test* a) {
  unsigned int hash1 = 0;
  unsigned int hash2 = 0;
  unsigned int mod = 100013425;
  unsigned int r = 7;
  for (char i : a->first) {
    hash1 = (hash1 * r + static_cast<unsigned int>(i)) % mod;
  }
  for (char i : a->second) {
    hash2 = (hash2 * r + static_cast<unsigned int>(i)) % mod;
  }
  return hash1 ^ hash2;
}

Test* binSearch(Test** ppArr, Test* elem, int length,
                int (*Compare)(const Test* pElement, const Test* pElement2)) {
  if (length == 1) {
    if (Compare(ppArr[0], elem) == 0) {
      return ppArr[0];
    }
    return nullptr;
  }
  int mid = length / 2;
  int cmp = Compare(ppArr[mid], elem);
  if (cmp == 0) {
    return ppArr[mid];
  } else if (cmp < 0) {
    return binSearch(ppArr, elem, length - mid, Compare);
  }
  return binSearch(ppArr + mid, elem, length - mid, Compare);
}

int main() {
  for (int i = 1; i < 21; ++i) {
    amount[i - 1] = i * 10000;
  }
  double time;
  std::chrono::steady_clock::time_point start;
  std::chrono::steady_clock::time_point end;
  std::vector<double> times_sorted_creation;
  std::vector<double> times_avl_creation;
  std::vector<double> times_hash_creation;
  std::vector<double> times_sorted_find;
  std::vector<double> times_avl_find;
  std::vector<double> times_hash_find;
  std::vector<double> times_sorted_delete;
  std::vector<double> times_avl_delete;
  std::vector<double> times_hash_delete;
  std::vector<double> times_sorted_find_random;
  std::vector<double> times_avl_find_random;
  std::vector<double> times_hash_find_random;
  std::vector<double> times_avl_remove;
  std::vector<double> times_hash_remove;
  for (int n : amount) {
    std::vector elems = std::vector<Test>(n);
    for (Test& elem : elems) {
      elem.first = RandStr();
      elem.second = RandStr();
    }
    CAVLTree avlTree = CAVLTree<Test, TestCmp>(1024);
    CHash hashTable = CHash<Test, TestHash, TestCmp>(n, 1024);
    Test** arr = new Test*[n];

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < n; ++i) {
      arr[i] = &elems[i];
    }
    mergeSort(arr, n, TestCmp);
    end = std::chrono::steady_clock::now();
    times_sorted_creation.push_back(
            std::chrono::duration<double>(end - start).count());

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < n; ++i) {
      avlTree.add(&elems[i]);
    }
    end = std::chrono::steady_clock::now();
    times_avl_creation.push_back(
            std::chrono::duration<double>(end - start).count());

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < n; ++i) {
      hashTable.add(&elems[i]);
    }
    end = std::chrono::steady_clock::now();
    times_hash_creation.push_back(
            std::chrono::duration<double>(end - start).count());


    std::vector randElems2n = std::vector<Test>(2 * n);
    for (Test& i : randElems2n) {
      i.first = RandStr();
      i.second = RandStr();
    }

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < n; ++i) {
      Test* tmp = binSearch(arr, &elems[i], n, TestCmp);
    }
    end = std::chrono::steady_clock::now();
    times_sorted_find.push_back(
            std::chrono::duration<double>(end - start).count());

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < n; ++i) {
      Test* tmp = avlTree.find(elems[i]);
    }
    end = std::chrono::steady_clock::now();
    times_avl_find.push_back(
            std::chrono::duration<double>(end - start).count());

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < n; ++i) {
      Test* tmp = hashTable.find(elems[i]);
    }
    end = std::chrono::steady_clock::now();
    times_hash_find.push_back(
            std::chrono::duration<double>(end - start).count());

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < 2 * n; ++i) {
      Test* tmp = binSearch(arr, &randElems2n[i], n, TestCmp);
    }

    // pizda
    end = std::chrono::steady_clock::now();
    times_sorted_find_random.push_back(
            std::chrono::duration<double>(end - start).count());

    start = std::chrono::steady_clock::now();
    for (const Test& i : randElems2n) {
      Test* tmp = avlTree.find(i);
    }
    end = std::chrono::steady_clock::now();
    times_avl_find_random.push_back(
            std::chrono::duration<double>(end - start).count());

    start = std::chrono::steady_clock::now();
    for (const Test& i : randElems2n) {
      Test* tmp = hashTable.find(i);
    }
    end = std::chrono::steady_clock::now();
    times_hash_find_random.push_back(
            std::chrono::duration<double>(end - start).count());

    start = std::chrono::steady_clock::now();
    delete[] arr;
    end = std::chrono::steady_clock::now();
    times_sorted_delete.push_back(
            std::chrono::duration<double>(end - start).count());

    start = std::chrono::steady_clock::now();
    avlTree.clear();
    end = std::chrono::steady_clock::now();
    times_avl_delete.push_back(
            std::chrono::duration<double>(end - start).count());

    start = std::chrono::steady_clock::now();
    hashTable.clear();
    end = std::chrono::steady_clock::now();
    times_hash_delete.push_back(
            std::chrono::duration<double>(end - start).count());

    for (int i = 0; i < n; ++i) {
      avlTree.add(&elems[i]);
    }

    for (int i = 0; i < n; ++i) {
      hashTable.add(&elems[i]);
    }

    start = std::chrono::steady_clock::now();
    for (const Test& i : elems) {
      bool tmp = avlTree.remove(i);
    }
    end = std::chrono::steady_clock::now();
    times_avl_remove.push_back(
            std::chrono::duration<double>(end - start).count());

    start = std::chrono::steady_clock::now();
    for (const Test& i : elems) {
      bool tmp = avlTree.remove(i);
    }
    end = std::chrono::steady_clock::now();
    times_hash_remove.push_back(
            std::chrono::duration<double>(end - start).count());

    std::cout << "Completed : " << n << std::endl;

    // TODO : замерять позаписывать в файлы и нарисовать графики
  }
  std::ofstream out;


  out.open("data/sort_creation");
  for (double i : times_sorted_creation) {
    out << i << '\n';
  }
  out.close();
  out.open("data/avl_creation");
  for (double i : times_avl_creation) {
    out << i << '\n';
  }
  out.close();
  out.open("data/hash_creation");
  for (double i : times_hash_creation) {
    out << i << '\n';
  }
  out.close();
  out.open("data/sort_find");
  for (double i : times_sorted_find) {
    out << i << '\n';
  }
  out.close();
  out.open("data/avl_find");
  for (double i : times_avl_find) {
    out << i << '\n';
  }
  out.close();
  out.open("data/hash_find");
  for (double i : times_hash_find) {
    out << i << '\n';
  }
  out.close();
  out.open("data/sort_find_random");
  for (double i : times_sorted_find_random) {
    out << i << '\n';
  }
  out.close();
  out.open("data/avl_find_random");
  for (double i : times_avl_find_random) {
    out << i << '\n';
  }
  out.close();
  out.open("data/hash_find_random");
  for (double i : times_hash_find_random) {
    out << i << '\n';
  }
  out.close();
  out.open("data/sort_delete");
  for (double i : times_sorted_delete) {
    out << i << '\n';
  }
  out.close();
  out.open("data/avl_delete");
  for (double i : times_avl_delete) {
    out << i << '\n';
  }
  out.close();
  out.open("data/hash_delete");
  for (double i : times_hash_delete) {
    out << i << '\n';
  }
  out.close();

  out.open("data/avl_remove");
  for (double i : times_avl_remove) {
    out << i << '\n';
  }
  out.close();

  out.open("data/hash_remove");
  for (double i : times_hash_remove) {
    out << i << '\n';
  }
  out.close();
}