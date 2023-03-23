#include "sort.h"
#include <string>
#include <cstring>
#include <iostream>

using namespace templates;

struct IntContainer {

    IntContainer(int size) : size_(size), data_(new int[size_]) {}


    ~IntContainer() {
      delete[] data_;
    }

    IntContainer(const IntContainer& other) : size_(other.size_), data_(new int[size_]) {
      std::memcpy(data_, other.data_, size_ * sizeof(int));
    }

    IntContainer& operator=(const IntContainer& other) {
      if (this != &other) {
        delete[] data_;
        size_ = other.size_;
        data_ = new int[size_];
        std::memcpy(data_, other.data_, size_ * sizeof(int));
      }
      return *this;
    }

    IntContainer(IntContainer&& other) noexcept : size_(other.size_), data_(other.data_) {
      other.size_ = 0;
      other.data_ = nullptr;
    }

    IntContainer& operator=(IntContainer&& other) noexcept {
      if (this != &other) {
        delete[] data_;
        size_ = other.size_;
        data_ = other.data_;
        other.size_ = 0;
        other.data_ = nullptr;
      }
      return *this;
    }



    int size_ = 0;
    int* data_ = nullptr;
    std::string s = "";

};


int compareT(const IntContainer *ic0, const IntContainer *ic1) {
  if (ic0->size_ > ic1->size_) {
    return 1;
  }
  else if (ic0->size_ == ic1->size_) {
    return 0;
  }
  return -1;
}

int compareV(const void *first, const void *second) {
  return compareT(reinterpret_cast< const IntContainer * >(first),
                  (reinterpret_cast< const IntContainer * >(second)));
}

// heap sort methods

void swap(void **ppArray, int i, int j) {
  void *tmp = ppArray[i];
  ppArray[i] = ppArray[j];
  ppArray[j] = tmp;
}

void heapify(void **ppArray, int length, int i, CompareSortType pCompareFunc) {
  int largest = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;

  if (left < length && pCompareFunc(ppArray[left], ppArray[largest]) > 0) {
    largest = left;
  }

  if (right < length && pCompareFunc(ppArray[right], ppArray[largest]) > 0) {
    largest = right;
  }

  if (largest != i) {
    swap(ppArray, i, largest);
    heapify(ppArray, length, largest, pCompareFunc);
  }
}

void templates::heapSort(void **ppArray, int length, templates::CompareSortType pCompareFunc) {
  for (int i = length / 2 - 1; i >= 0; --i) {
    heapify(ppArray, length, i, pCompareFunc);
  }

  for (int i = length - 1; i >= 0; --i) {
    swap(ppArray, 0, i);
    heapify(ppArray, i, 0, pCompareFunc);
  }
}

// merge sort

void templates::mergeSort(void **ppArray, int length, templates::CompareSortType *pCompareFunc) {
  if (length == 1) {
    return;
  }

  int half = length / 2;
  void **left = ppArray;
  void **right = ppArray + half;

  mergeSort(left, half, pCompareFunc);
  mergeSort(right, length - half, pCompareFunc);

  void **tmp = new void *[length];

  for (int i = 0, j = 0; i + j < length;) {
    if (i == half) {
      tmp[i + j] = right[j];
      ++j;
    } else if (j == length - half) {
      tmp[i + j] = left[i];
      ++i;
    } else if (pCompareFunc(left[i], right[j]) > 0) {
      tmp[i + j] = left[i];
      ++i;
    } else {
      tmp[i + j] = right[j];
      ++j;
    }
  }

  for (int i = 0; i < length; ++i) {
    ppArray[i] = tmp[i];
  }

  delete[] tmp;
}

int main() {
/*
  int len = 15;
  IntContainer** test_arr = new IntContainer*[len];
  std::cout << "merge sort test" << std::endl;
  for (int i = 0; i < len; ++i) {
    test_arr[i] = new IntContainer(i % 4);
  }

  for (int i = 0; i < len; ++i) {
    std::cout << test_arr[i]->size_ << ' ';
  }
  std::cout << std::endl;

  mergeSort(test_arr, len, &compareT);
  for (int i = 0; i < len; ++i) {
    std::cout << test_arr[i]->size_ << ' ';
  }
  for (int i = 0; i < len; ++i) {
    delete test_arr[i];
  }
  delete[] test_arr;

  std::cout << std::endl;
  std::cout << "heap sort test" << std::endl;


  test_arr = new IntContainer*[len];

  for (int i = 0; i < len; ++i) {
    test_arr[i] = new IntContainer(i % 4);
  }

  for (int i = 0; i < len; ++i) {
    std::cout << test_arr[i]->size_ << ' ';
  }
  std::cout << std::endl;

  heapSort(test_arr, len, &compareT);
  for (int i = 0; i < len; ++i) {
    std::cout << test_arr[i]->size_ << ' ';
  }
  for (int i = 0; i < len; ++i) {
    delete test_arr[i];
  }
  delete[] test_arr;
  */
}
