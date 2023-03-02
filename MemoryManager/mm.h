#ifndef MEMORY_MANAGER_HEAD_H_2023_02_10
#define MEMORY_MANAGER_HEAD_H_2023_02_10

namespace lab618 {
template <class T>
class CMemoryManager {
 private:
  struct block {
    // Массив данных блока
    T* pdata;
    // Адрес следующего блока
    block* pnext;
    // Первая свободная ячейка
    int firstFreeIndex;
    // Число заполненных ячеек
    int usedCount;
  };

  inline void ConstructElements(T* pElement) {
    memset(reinterpret_cast<void*>(pElement), 0, sizeof(T));
    ::new (reinterpret_cast<void*>(pElement)) T();
  }

  inline void DestructElements(T* pElement) {
    pElement->~T();
    memset(reinterpret_cast<void*>(pElement), 0, sizeof(T));
  }

 public:
  class CException {
   public:
    CException() {}
  };

 public:
  /**
    _default_block_size - количество элементов в блоке данных
    isDeleteElementsOnDestruct - уничтожать елементы в деструкторе менеджера или
    проверять на наличие неосвобожденных функцией deleteObject элементов.
  */
  CMemoryManager(int _default_block_size,
                 bool isDeleteElementsOnDestruct = false)
      : m_blkSize(_default_block_size),
        m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct) {
    m_pBlocks = newBlock();
    m_pCurrentBlk = m_pBlocks;
  }

  virtual ~CMemoryManager() { clear(); }

  // Получить адрес нового элемента из менеджера
  T* newObject() {

    // Проверяем есть ли свободное место в текущем блоке
    if (m_pCurrentBlk->firstFreeIndex < m_blkSize) {
      T* p = m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex;
      ConstructElements(p);
      m_pCurrentBlk->firstFreeIndex++;
      m_pCurrentBlk->usedCount++;
      return p;
    }

    // Проверяем есть ли неиспользованные блоки
    if (m_pCurrentBlk->pnext != nullptr) {
      m_pCurrentBlk = m_pCurrentBlk->pnext;
      m_pCurrentBlk->firstFreeIndex = 0;
      T* p = m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex;
      ConstructElements(p);
      m_pCurrentBlk->firstFreeIndex++;
      m_pCurrentBlk->usedCount++;
      return p;
    }

    // Создаем новый блок
    block* pNewBlock = newBlock();
    if (pNewBlock == nullptr) {
      throw CException();
    }
    pNewBlock->pnext = m_pCurrentBlk;
    m_pCurrentBlk = pNewBlock;
    m_pCurrentBlk->firstFreeIndex = 0;
    T* p = m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex;
    ConstructElements(p);
    m_pCurrentBlk->firstFreeIndex++;
    m_pCurrentBlk->usedCount++;
    return p;
  }

  // Освободить элемент в менеджере
  bool deleteObject(T* p) {
    if (p == nullptr) {
      return false;
    }

    block* blk = m_pBlocks;

    while (blk != nullptr) {
      if (p >= blk->pdata && p < blk->pdata + m_blkSize) {
        // элемент принадлежит этому блоку
        int index = static_cast<int>(p - blk->pdata);

        if (blk->pdata[index].number == 0 && blk->pdata[index].str.empty() &&
            blk->pdata[index].dt == 0) {
          // элемент уже освобожден
          return false;
        }

        DestructElements(&blk->pdata[index]);
        ConstructElements(&blk->pdata[index]);

        blk->pdata[index].number = 0;
        blk->pdata[index].str = "";
        blk->pdata[index].dt = 0;

        blk->usedCount--;

        if (blk->usedCount == 0) {
          deleteBlock(blk);
        }

        return true;
      }

      blk = blk->pnext;
    }

    return false;  // элемент не найден
  }

  // Очистка данных, зависит от m_isDeleteElementsOnDestruct
  void clear() {
    if (m_isDeleteElementsOnDestruct) {
      for (block* blk = m_pBlocks; blk; blk = blk->pnext) {
        for (int i = 0; i < blk->usedCount; i++) {
          DestructElements(&blk->pdata[i]);
        }
      }
    }

    while (m_pBlocks) {
      block* next = m_pBlocks->pnext;
      deleteBlock(m_pBlocks);
      m_pBlocks = next;
    }

    m_pCurrentBlk = nullptr;
  }

 private:
  // Создать новый блок данных. применяется в newObject
  block* newBlock() {
    block* blk = new block;
    blk->pdata = new T[m_blkSize];
    blk->pnext = nullptr;
    blk->firstFreeIndex = 0;
    blk->usedCount = 0;
    return blk;
  }

  // Освободить память блока данных. Применяется в clear
  void deleteBlock(block* p) {
    delete[] p->pdata;
    delete p;
  }

  // Размер блока
  int m_blkSize;
  // Начало списка блоков
  block* m_pBlocks;
  // Текущий блок
  block* m_pCurrentBlk;
  // Удалять ли элементы при освобождении
  bool m_isDeleteElementsOnDestruct;
};

};  // namespace lab618

#endif  // #define MEMORY_MANAGER_HEAD_H_2023_02_10
