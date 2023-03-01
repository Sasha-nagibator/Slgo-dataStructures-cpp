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
    CIterator() : m_pCurrent(nullptr), m_pBegin(nullptr) {}

    CIterator(leaf *p) : m_pCurrent(p), m_pBegin(p) {}

    CIterator(const CIterator &src)
        : m_pCurrent(src.m_pCurrent), m_pBegin(src.m_pBegin) {}

    ~CIterator() {}

    CIterator &operator=(const CIterator &src) {
      if (this != &src) {
        m_pCurrent = src.m_pCurrent;
        m_pBegin = src.m_pBegin;
      }
      return *this;
    }

    bool operator!=(const CIterator &it) const {
      return (m_pCurrent != it.m_pCurrent);
    }

    void operator++() {
      if (m_pCurrent != nullptr) m_pCurrent = m_pCurrent->pNext;
    }

    T &getData() { return m_pCurrent->data; }

    T &operator*() { return m_pCurrent->data; }

    leaf *getLeaf() { return m_pCurrent; }

    void setLeaf(leaf *p) { m_pCurrent = p; }

    void setLeafPreBegin(leaf *p) { m_pBegin = p; }

    bool isValid() { return (m_pCurrent != nullptr); }

   private:
    leaf *m_pCurrent;
    leaf *m_pBegin;
  };

  CSingleLinkedList() : m_pBegin(nullptr), m_pEnd(nullptr) {}

  ~CSingleLinkedList() { clear(); }

  void pushBack(T &data) {
    leaf *pNew = new leaf(data, nullptr);
    if (!m_pBegin) {
      m_pBegin = pNew;
    } else {
      m_pEnd->pNext = pNew;
    }
    m_pEnd = pNew;
  }

  void pushFront(T &data) {
    leaf *pNew = new leaf(data, m_pBegin);
    m_pBegin = pNew;
    if (!m_pEnd) {
      m_pEnd = pNew;
    }
  }

  T popFront() {
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

  void erase(CIterator &it) {
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

  int getSize() {
    int size = 0;
    leaf *current = m_pBegin;
    while (current != nullptr) {
      ++size;
      current = current->pNext;
    }
    return size;
  }

  void clear() {
    while (m_pBegin != nullptr) {
      leaf *temp = m_pBegin;
      m_pBegin = m_pBegin->pNext;
      delete temp;
    }
  }

  CIterator begin() { return CIterator(m_pBegin); }
};

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
    CIterator() : m_pBegin(nullptr), m_pCurrent(nullptr), m_pEnd(nullptr) {}

    CIterator(leaf *p) : m_pBegin(nullptr), m_pCurrent(p), m_pEnd(nullptr) {}

    CIterator(const CIterator &src)
        : m_pBegin(src.m_pBegin),
          m_pCurrent(src.m_pCurrent),
          m_pEnd(src.m_pEnd) {}

    ~CIterator() {}

    CIterator &operator=(const CIterator &src) {
      m_pBegin = src.m_pBegin;
      m_pCurrent = src.m_pCurrent;
      m_pEnd = src.m_pEnd;
      return *this;
    }

    bool operator!=(const CIterator &it) const {
      return m_pCurrent != it.m_pCurrent;
    }

    void operator++() {
      if (!isValid()) {
        throw std::runtime_error("Invalid iterator");
      }

      if (m_pCurrent)
        m_pCurrent = m_pCurrent->pNext;
      else
        m_pCurrent = m_pBegin;
    }

    void operator--() {
      if (!isValid()) {
        throw std::runtime_error("Invalid iterator");
      }

      if (m_pCurrent)
        m_pCurrent = m_pCurrent->pPrev;
      else
        m_pCurrent = m_pEnd;
    }

    T &getData() {
      if (!isValid()) {
        throw std::runtime_error("Invalid iterator");
      }

      return m_pCurrent->data;
    }

    T &operator*() { return getData(); }

    leaf *getLeaf() {
      if (!isValid()) {
        return nullptr;
      }

      return m_pCurrent;
    }

    void setLeaf(typename CDualLinkedList<T>::leaf *p) { m_pCurrent = p; }

    void setLeafPreBegin(typename CDualLinkedList<T>::leaf *p) {
      m_pBegin = nullptr;
      m_pCurrent = nullptr;
      m_pEnd = p;
    }

    void setLeafPostEnd(typename CDualLinkedList<T>::leaf *p) {
      m_pBegin = p;
      m_pCurrent = nullptr;
      m_pEnd = nullptr;
    }

    bool isValid() { return (m_pCurrent != nullptr); }

   private:
    //храним голову списка, если мы находимся перед началом
    leaf *m_pBegin;
    // храним текущее положение
    leaf *m_pCurrent;
    //храним конец списка, если мы находимся после конца
    leaf *m_pEnd;
  };

 public:
  CDualLinkedList() : m_pBegin(nullptr), m_pEnd(nullptr) {}

  ~CDualLinkedList() { clear(); }

  void pushBack(T &data) {
    leaf *pNew = new leaf(data, m_pEnd, nullptr);
    if (!m_pBegin) m_pBegin = pNew;
    if (m_pEnd) m_pEnd->pNext = pNew;
    m_pEnd = pNew;
  }

  T popBack() {
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

  void pushFront(T &data) {
    leaf *pNew = new leaf(data, nullptr, m_pBegin);
    if (!m_pEnd) m_pEnd = pNew;
    if (m_pBegin) m_pBegin->pPrev = pNew;
    m_pBegin = pNew;
  }

  T popFront() {
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

  void erase(CIterator &it) {
    if (!it.isValid()) {
      throw std::runtime_error("Invalid iterator");
    }

    leaf *pTemp = it.getLeaf();
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
    it.setLeaf(pTemp->pPrev);
  }

  void eraseAndNext(CIterator &it) {
    if (!it.isValid()) {
      throw std::runtime_error("Invalid iterator");
    }

    leaf *pTemp = it.getLeaf();
    it.setLeaf(pTemp->pNext);
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

  int getSize() {
    int size = 0;
    leaf *pTemp = m_pBegin;
    while (pTemp) {
      ++size;
      pTemp = pTemp->pNext;
    }
    return size;
  }

  void clear() {
    while (m_pBegin) {
      leaf *pTemp = m_pBegin;
      m_pBegin = m_pBegin->pNext;
      delete pTemp;
    }

    m_pEnd = nullptr;
  }

  CIterator begin() { return CIterator(m_pBegin); }

  CIterator end() { return CIterator(m_pEnd); }

 private:
  //храним голову и хвост списка
  leaf *m_pBegin, *m_pEnd;
};

};      // namespace lab618
#endif  //#ifndef TEMPLATES_LIST_2022_02_03