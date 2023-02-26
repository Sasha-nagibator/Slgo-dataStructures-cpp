#include <iostream>

#include "list.h"

using namespace lab618;

struct ComplexStructure {
  int a = 1;
  std::string s = "Hello";
  double b = 2;
  friend std::ostream &operator<<(std::ostream &os, const ComplexStructure &cs);
};

std::ostream &operator<<(std::ostream &os, const ComplexStructure &cs) {
  os << cs.a << " " << cs.s << " " << cs.b << std::endl;
  return os;
}

template <typename T, template <typename> typename List>
void printList(List<T> &list) {
  typename List<T>::CIterator it = list.begin();
  while (it.isValid()) {
    std::cout << *it << " ";
    ++it;
  }
  std::cout << std::endl;
}

int main() {
  ComplexStructure CS;
  ComplexStructure CS2 = {4, "World", 5};
  ComplexStructure another = {7, "!", 8};

  CSingleLinkedList<ComplexStructure> myList;

  myList.pushBack(CS);
  myList.pushBack(CS2);
  myList.pushFront(another);

  printList(myList);
  std::cout << "Size: " << myList.getSize() << std::endl;

  typename CSingleLinkedList<ComplexStructure>::CIterator it = myList.begin();
  ++it;
  myList.erase(it);

  printList(myList);
  std::cout << "Size: " << myList.getSize() << std::endl;

  myList.popFront();
  printList(myList);
  std::cout << "Size: " << myList.getSize() << std::endl;

  myList.clear();
  printList(myList);
  std::cout << "Size: " << myList.getSize() << std::endl;

  myList.pushBack(CS);
  myList.pushBack(CS2);
  myList.pushFront(another);
  myList.popFront();
  printList(myList);

  CDualLinkedList<int> myListDual;

  // Add some elements to the list
  for (int i = 0; i < 5; ++i) {
    myListDual.pushBack(i);
  }

  for (int i = 10; i < 15; ++i) {
    myListDual.pushFront(i);
  }

  // Test size() method
  std::cout << "List size: " << myListDual.getSize() << std::endl;

  // Test iterator
  printList(myListDual);

  // Test popFront() method
  int front = myListDual.popFront();
  std::cout << "Popped front element: " << front << std::endl;
  std::cout << "List size: " << myListDual.getSize() << std::endl;

  // Test popBack() method
  int back = myListDual.popBack();
  std::cout << "Popped back element: " << back << std::endl;
  std::cout << "List size: " << myListDual.getSize() << std::endl;

  // Test erase() method
  auto itDual = myListDual.begin();
  std::cout << "Now list looks like this: ";
  printList(myListDual);
  ++itDual;
  myListDual.erase(itDual);
  std::cout << "List after erasing 1st element: ";
  printList(myListDual);

  // Test eraseAndNext() method
  itDual = myListDual.begin();
  ++itDual;
  myListDual.eraseAndNext(itDual);
  std::cout << "List after erasing 1st element and moving to next element: ";
  printList(myListDual);
  std::cout << "Iterator points to: " << *itDual << std::endl;

  // Test erase() method
  itDual = myListDual.begin();
  ++itDual;
  myListDual.erase(itDual);
  std::cout << "List after erasing 1st element: ";
  printList(myListDual);
  std::cout << "Iterator points to: " << *itDual << std::endl;

  // Test clear() method
  myListDual.clear();
  std::cout << "List after clearing: ";
  printList(myListDual);
  return 0;
}
