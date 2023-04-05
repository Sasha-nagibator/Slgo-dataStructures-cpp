#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "avltree.h"

int compareInt(const int* a, const int* b)
{
  if (*a < *b) return -1;
  if (*a > *b) return 1;
  return 0;
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