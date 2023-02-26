#ifndef TEMPLATES_LIST_2023_02_02
#define TEMPLATES_LIST_2023_02_02

#include <stdexcept>

namespace lab618 {
template <class T>
class CSingleLinkedList {
 private:
  struct leaf {
    T data;
    leaf *pNext;

    leaf(T &_data, leaf *_pNext = nullptr) : data(_data), pNext(_pNext) {}
  };

  leaf *m_pBegin, *m_pEnd;

 public:
  class CIterator {
   public:
    CIterator();

    CIterator(leaf *p);

    CIterator(const CIterator &src);

    ~CIterator();

    CIterator &operator=(const CIterator &src);

    bool operator!=(const CIterator &it) const;

    void operator++();

    T &getData();

    T &operator*();

    leaf *getLeaf();

    void setLeaf(leaf *p);

    void setLeafPreBegin(leaf *p);

    bool isValid();

   private:
    leaf *m_pCurrent;
    leaf *m_pBegin;
  };

  CSingleLinkedList();

  ~CSingleLinkedList();

  void pushBack(T &data);

  void pushFront(T &data);

  T popFront();

  void erase(CIterator &it);

  int getSize();

  void clear();

  CIterator begin();
};

template <class T>
CSingleLinkedList<T>::CIterator::CIterator()
    : m_pCurrent(nullptr), m_pBegin(nullptr) {}

template <class T>
CSingleLinkedList<T>::CIterator::CIterator(leaf *p)
    : m_pCurrent(p), m_pBegin(p) {}

template <class T>
CSingleLinkedList<T>::CIterator::CIterator(const CIterator &src)
    : m_pCurrent(src.m_pCurrent), m_pBegin(src.m_pBegin) {}

template <class T>
CSingleLinkedList<T>::CIterator::~CIterator() {}

template <class T>
typename CSingleLinkedList<T>::CIterator &
CSingleLinkedList<T>::CIterator::operator=(const CIterator &src) {
  if (this != &src) {
    m_pCurrent = src.m_pCurrent;
    m_pBegin = src.m_pBegin;
  }
  return *this;
}

template <class T>
bool CSingleLinkedList<T>::CIterator::operator!=(const CIterator &it) const {
  return (m_pCurrent != it.m_pCurrent);
}

template <class T>
void CSingleLinkedList<T>::CIterator::operator++() {
  if (m_pCurrent != nullptr) m_pCurrent = m_pCurrent->pNext;
}

template <class T>
T &CSingleLinkedList<T>::CIterator::getData() {
  return m_pCurrent->data;
}

template <class T>
T &CSingleLinkedList<T>::CIterator::operator*() {
  return m_pCurrent->data;
}

template <class T>
typename CSingleLinkedList<T>::leaf *
CSingleLinkedList<T>::CIterator::getLeaf() {
  return m_pCurrent;
}

template <class T>
void CSingleLinkedList<T>::CIterator::setLeaf(leaf *p) {
  m_pCurrent = p;
}

template <class T>
void CSingleLinkedList<T>::CIterator::setLeafPreBegin(leaf *p) {
  m_pBegin = p;
}

template <class T>
bool CSingleLinkedList<T>::CIterator::isValid() {
  return (m_pCurrent != nullptr);
}

template <class T>
CSingleLinkedList<T>::CSingleLinkedList()
    : m_pBegin(nullptr), m_pEnd(nullptr) {}

template <class T>
CSingleLinkedList<T>::~CSingleLinkedList() {
  clear();
}

template <class T>
void CSingleLinkedList<T>::pushBack(T &data) {
  leaf *pNew = new leaf(data, nullptr);
  if (!m_pBegin) {
    m_pBegin = pNew;
  } else {
    m_pEnd->pNext = pNew;
  }
  m_pEnd = pNew;
}

template <class T>
void CSingleLinkedList<T>::pushFront(T &data) {
  leaf *pNew = new leaf(data, m_pBegin);
  m_pBegin = pNew;
  if (!m_pEnd) {
    m_pEnd = pNew;
  }
}

template <class T>
T CSingleLinkedList<T>::popFront() {
  if (!m_pBegin) {
    throw std::out_of_range("List is empty");
  }

  T data = m_pBegin->data;
  leaf *pDel = m_pBegin;
  m_pBegin = m_pBegin->pNext;
  delete pDel;

  if (!m_pBegin) {
    m_pEnd = nullptr;
  }

  return data;
}

template <class T>
void CSingleLinkedList<T>::erase(CIterator &it) {
  if (!it.isValid()) {
    throw std::out_of_range("Invalid iterator");
  }

  leaf *pDel = it.getLeaf();
  if (pDel == m_pBegin) {
    m_pBegin = pDel->pNext;
  } else {
    // Находим элемент перед удаляемым
    CIterator prevIt = begin();
    while (prevIt.getLeaf() && prevIt.getLeaf()->pNext != pDel) {
      ++prevIt;
    }
    prevIt.getLeaf()->pNext = pDel->pNext;
  }

  if (pDel == m_pEnd) {
    m_pEnd = it.getLeaf()->pNext;
  }
}

template <class T>
int CSingleLinkedList<T>::getSize() {
  int size = 0;
  leaf *current = m_pBegin;
  while (current != nullptr) {
    ++size;
    current = current->pNext;
  }
  return size;
}

template <class T>
void CSingleLinkedList<T>::clear() {
  while (m_pBegin != nullptr) {
    leaf *temp = m_pBegin;
    m_pBegin = m_pBegin->pNext;
    delete temp;
  }
}

template <class T>
typename CSingleLinkedList<T>::CIterator CSingleLinkedList<T>::begin() {
  return CIterator(m_pBegin);
}

//-------------------------CDualLinkedList-----------------------------------

template <class T>
class CDualLinkedList {
 private:
  struct leaf {
    // Данные
    T data;
    // Указатель на предыдущий / следующий лист списка
    leaf *pNext, *pPrev;

    leaf(T &_data, leaf *_pPrev, leaf *_pNext)
        : data(_data), pPrev(_pPrev), pNext(_pNext) {}
  };

 public:
  class CIterator {
   public:
    CIterator();

    CIterator(leaf *p);

    CIterator(const CIterator &src);

    ~CIterator();
    CIterator &operator=(const CIterator &src);

    bool operator!=(const CIterator &it) const;

    void operator++();

    void operator--();

    T &getData();

    T &operator*();

    leaf *getLeaf();

    // применяется в erase и eraseAndNext
    void setLeaf(leaf *p);

    // применяется в erase и eraseAndNext
    void setLeafPreBegin(leaf *p);

    // применяется в erase и eraseAndNext
    void setLeafPostEnd(leaf *p);

    bool isValid();

   private:
    //храним голову списка, если мы находимся перед началом
    leaf *m_pBegin;
    // храним текущее положение
    leaf *m_pCurrent;
    //храним конец списка, если мы находимся после конца
    leaf *m_pEnd;
  };

 public:
  CDualLinkedList();

  virtual ~CDualLinkedList();

  void pushBack(T &data);

  T popBack();

  void pushFront(T &data);

  T popFront();

  // изменяет состояние итератора. выставляет предыдущую позицию.
  void erase(CIterator &it);

  // изменяет состояние итератора. выставляет следующую позицию.
  void eraseAndNext(CIterator &it);

  int getSize();

  void clear();
  CIterator begin();

  CIterator end();

 private:
  //храним голову и хвост списка
  leaf *m_pBegin, *m_pEnd;

  // применяется в erase и eraseAndNext, чтобы не копировать код
  void updateAndDeleteLeaf(CIterator &it, leaf *pTemp);
};

template <class T>
CDualLinkedList<T>::CIterator::CIterator()
    : m_pBegin(nullptr), m_pCurrent(nullptr), m_pEnd(nullptr) {}

template <class T>
CDualLinkedList<T>::CIterator::CIterator(leaf *p)
    : m_pBegin(nullptr), m_pCurrent(p), m_pEnd(nullptr) {}

template <class T>
CDualLinkedList<T>::CIterator::CIterator(const CIterator &src)
    : m_pBegin(src.m_pBegin), m_pCurrent(src.m_pCurrent), m_pEnd(src.m_pEnd) {}

template <class T>
CDualLinkedList<T>::CIterator::~CIterator() {}

template <class T>
typename CDualLinkedList<T>::CIterator &
CDualLinkedList<T>::CIterator::operator=(const CIterator &src) {
  m_pBegin = src.m_pBegin;
  m_pCurrent = src.m_pCurrent;
  m_pEnd = src.m_pEnd;
  return *this;
}

template <class T>
bool CDualLinkedList<T>::CIterator::operator!=(const CIterator &it) const {
  return m_pCurrent != it.m_pCurrent;
}

template <class T>
void CDualLinkedList<T>::CIterator::operator++() {
  if (!isValid()) {
    throw std::runtime_error("Invalid iterator");
  }

  if (m_pCurrent)
    m_pCurrent = m_pCurrent->pNext;
  else
    m_pCurrent = m_pBegin;
}

template <class T>
void CDualLinkedList<T>::CIterator::operator--() {
  if (!isValid()) {
    throw std::runtime_error("Invalid iterator");
  }

  if (m_pCurrent)
    m_pCurrent = m_pCurrent->pPrev;
  else
    m_pCurrent = m_pEnd;
}

template <class T>
T &CDualLinkedList<T>::CIterator::getData() {
  if (!isValid()) {
    throw std::runtime_error("Invalid iterator");
  }

  return m_pCurrent->data;
}

template <class T>
T &CDualLinkedList<T>::CIterator::operator*() {
  return getData();
}

template <class T>
typename CDualLinkedList<T>::leaf *CDualLinkedList<T>::CIterator::getLeaf() {
  if (!isValid()) {
    throw std::runtime_error("Invalid iterator");
  }

  return m_pCurrent;
}

template <class T>
void CDualLinkedList<T>::CIterator::setLeaf(
    typename CDualLinkedList<T>::leaf *p) {
  m_pCurrent = p;
}

template <class T>
void CDualLinkedList<T>::CIterator::setLeafPreBegin(
    typename CDualLinkedList<T>::leaf *p) {
  m_pBegin = nullptr;
  m_pCurrent = nullptr;
  m_pEnd = p;
}

template <class T>
void CDualLinkedList<T>::CIterator::setLeafPostEnd(
    typename CDualLinkedList<T>::leaf *p) {
  m_pBegin = p;
  m_pCurrent = nullptr;
  m_pEnd = nullptr;
}

template <class T>
bool CDualLinkedList<T>::CIterator::isValid() {
  return (m_pCurrent != nullptr);
}

template <class T>
CDualLinkedList<T>::CDualLinkedList() : m_pBegin(nullptr), m_pEnd(nullptr) {}

template <class T>
CDualLinkedList<T>::~CDualLinkedList() {
  clear();
}

template <class T>
void CDualLinkedList<T>::pushBack(T &data) {
  leaf *pNew = new leaf(data, m_pEnd, nullptr);
  if (!m_pBegin) m_pBegin = pNew;
  if (m_pEnd) m_pEnd->pNext = pNew;
  m_pEnd = pNew;
}

template <class T>
T CDualLinkedList<T>::popBack() {
  if (!m_pEnd) throw std::runtime_error("List is empty");

  T data = m_pEnd->data;
  leaf *pTemp = m_pEnd;

  m_pEnd = m_pEnd->pPrev;
  if (m_pEnd)
    m_pEnd->pNext = nullptr;
  else
    m_pBegin = nullptr;

  delete pTemp;
  return data;
}

template <class T>
void CDualLinkedList<T>::pushFront(T &data) {
  leaf *pNew = new leaf(data, nullptr, m_pBegin);
  if (!m_pEnd) m_pEnd = pNew;
  if (m_pBegin) m_pBegin->pPrev = pNew;
  m_pBegin = pNew;
}

template <class T>
T CDualLinkedList<T>::popFront() {
  if (!m_pBegin) throw std::runtime_error("List is empty");

  T data = m_pBegin->data;
  leaf *pTemp = m_pBegin;

  m_pBegin = m_pBegin->pNext;
  if (m_pBegin)
    m_pBegin->pPrev = nullptr;
  else
    m_pEnd = nullptr;

  delete pTemp;
  return data;
}

template <class T>
void CDualLinkedList<T>::updateAndDeleteLeaf(CIterator &it, leaf *pTemp) {
  if (pTemp->pPrev)
    pTemp->pPrev->pNext = pTemp->pNext;
  else
    m_pBegin = pTemp->pNext;

  if (pTemp->pNext)
    pTemp->pNext->pPrev = pTemp->pPrev;
  else
    m_pEnd = pTemp->pPrev;

  it.setLeaf(pTemp->pNext);
  delete pTemp;
}

template <class T>
void CDualLinkedList<T>::erase(CIterator &it) {
  if (!it.isValid()) {
    throw std::runtime_error("Invalid iterator");
  }

  leaf *pTemp = it.getLeaf();
  updateAndDeleteLeaf(it, pTemp);
  it.setLeaf(pTemp->pPrev);
}

template <class T>
void CDualLinkedList<T>::eraseAndNext(CIterator &it) {
  if (!it.isValid()) {
    throw std::runtime_error("Invalid iterator");
  }

  leaf *pTemp = it.getLeaf();
  it.setLeaf(pTemp->pNext);
  updateAndDeleteLeaf(it, pTemp);
}

template <class T>
int CDualLinkedList<T>::getSize() {
  int size = 0;
  leaf *pTemp = m_pBegin;
  while (pTemp) {
    ++size;
    pTemp = pTemp->pNext;
  }
  return size;
}

template <class T>
void CDualLinkedList<T>::clear() {
  while (m_pBegin) {
    leaf *pTemp = m_pBegin;
    m_pBegin = m_pBegin->pNext;
    delete pTemp;
  }

  m_pEnd = nullptr;
}

template <class T>
typename CDualLinkedList<T>::CIterator CDualLinkedList<T>::begin() {
  return CIterator(m_pBegin);
}

template <class T>
typename CDualLinkedList<T>::CIterator CDualLinkedList<T>::end() {
  return CIterator(m_pEnd);
}

};      // namespace lab618
#endif  //#ifndef TEMPLATES_LIST_2022_02_03
