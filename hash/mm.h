#ifndef MEMORY_MANAGER_HEAD_H_2023_02_10
#define MEMORY_MANAGER_HEAD_H_2023_02_10

#define NO_FREE_CELL_INDICATOR 0xFFFFFFFF

#include <cstring>
#include <stdexcept>

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
        class CException : public std::exception
        {
        public:
            CException() noexcept {}
            virtual const char* what() const noexcept override
            {
              return "Not all elements have been deleted before calling clear()";
            }
        };

    public:
        /**
          _default_block_size - количество элементов в блоке данных
          isDeleteElementsOnDestruct - уничтожать елементы в деструкторе менеджера или
          проверять на наличие неосвобожденных функцией deleteObject элементов.
        */
        CMemoryManager(int _default_block_size, bool isDeleteElementsOnDestruct = true) : m_blkSize(_default_block_size),
                                                                                           m_pBlocks(nullptr), m_pCurrentBlk(nullptr), m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct)
        {}

        virtual ~CMemoryManager() noexcept(false) {
          clear();
        }

        // Получить адрес нового элемента из менеджера
        T* newObject() {
          if (m_pCurrentBlk == nullptr) {
            m_pCurrentBlk = newBlock();
          } else if (m_pCurrentBlk->firstFreeIndex == NO_FREE_CELL_INDICATOR) {
            m_pCurrentBlk = m_pBlocks;
            while (m_pCurrentBlk && m_pCurrentBlk->firstFreeIndex == NO_FREE_CELL_INDICATOR) {
              m_pCurrentBlk = m_pCurrentBlk->pnext;
            }
            if (m_pCurrentBlk == nullptr) {
              m_pCurrentBlk = newBlock();
            }
          }

          T* currIndex = nullptr;

          currIndex = m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex;
          m_pCurrentBlk->firstFreeIndex = *reinterpret_cast<uint32_t *>(currIndex);

          ++m_pCurrentBlk->usedCount;
          ConstructElements(currIndex);
          return currIndex;
        }

        // Освободить элемент в менеджере
        bool deleteObject(T *p) {
          block* blk = m_pBlocks;
          while (blk) {
            if (p >= blk->pdata && p < blk->pdata + m_blkSize) {

              DestructElements(p);
              *reinterpret_cast<uint32_t *> (p) = blk->firstFreeIndex;
              --blk->usedCount;
              auto current = p;
              auto start = blk->pdata;
              blk->firstFreeIndex = current - start;
              // if (blk->firstFreeIndex > current - start || blk->firstFreeIndex == NO_FREE_CELL_INDICATOR) {
              //  blk->firstFreeIndex = current - start;
              // }
              return true;
            }
            blk = blk->pnext;
          }
          return false;
        }

        // Очистка данных, зависит от m_isDeleteElementsOnDestruct
        void clear() {
          if (m_isDeleteElementsOnDestruct) {
            bool* emptyMask = new bool[m_blkSize];
            while (m_pBlocks != nullptr) {
              block *tmp = m_pBlocks;
              m_pBlocks = m_pBlocks->pnext;
              deleteBlock(tmp, emptyMask);
            }
            delete[] emptyMask;
          } else {
            for (block *it = m_pBlocks; it != nullptr; it = it->pnext) {
              if (it->usedCount) {
                throw CException();
              }
            }
          }
          m_pBlocks = nullptr;
        }

    private:
        // Создать новый блок данных. применяется в newObject
        block* newBlock() {
          block* p = new block;

          p->pdata = reinterpret_cast<T *>(new char[m_blkSize * sizeof(T)]);
          for (int i = 0; i < m_blkSize - 1; ++i) {
            *reinterpret_cast<uint32_t *>(p->pdata + i) = i + 1;
          }
          *reinterpret_cast<uint32_t *>(p->pdata + m_blkSize - 1) = NO_FREE_CELL_INDICATOR;
          p->pnext = nullptr;
          p->firstFreeIndex = 0;
          p->usedCount = 0;

          if (m_pBlocks == nullptr) {
            m_pBlocks = m_pCurrentBlk = p;
          } else {
            p->pnext = m_pBlocks;
            m_pBlocks = p;
          }

          return p;
        }

        // Освободить память блока данных. Применяется в clear
        void deleteBlock(block* p, bool* emptyMask) {
          if (m_isDeleteElementsOnDestruct) {

            std::memset(emptyMask, 0, m_blkSize * sizeof(bool));
            while (p->firstFreeIndex != NO_FREE_CELL_INDICATOR) {
              emptyMask[p->firstFreeIndex] = 1;
              p->firstFreeIndex = *reinterpret_cast<uint32_t *>(p->pdata + p->firstFreeIndex);
            }
            for (int i = 0; i < m_blkSize; ++i) {
              if (!emptyMask[i]) {
                p->pdata[i].~T();
              }
            }
          }
          delete[] reinterpret_cast<char*>(p->pdata);
          delete p;
          p = nullptr;
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