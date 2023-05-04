#ifndef MEMORY_MANAGER_HEAD_H_2023_02_10
#define MEMORY_MANAGER_HEAD_H_2023_02_10

#include <iostream>
#include <cstring>
#include <unordered_set>

#define NO_FREE_CELL_INDICATOR 0xFFFFFFFF

namespace lab618 {

    template<class T>
    class CMemoryManager {
    private:
        struct block {
            block(T *pdata) : pdata(pdata) {
                firstFreeIndex = 0;
                usedCount = 0;
            }
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
        CMemoryManager(int _default_block_size, bool isDeleteElementsOnDestruct = false) : m_blkSize(_default_block_size),
                                                                                           m_pBlocks(nullptr), m_pCurrentBlk(nullptr), m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct)
        {}

        virtual ~CMemoryManager() {
            clear();
        }

        // Получить адрес нового элемента из менеджера
        T *newObject() {
            if (m_pCurrentBlk == nullptr) {
                block *new_block = newBlock();
                new_block->pnext = m_pBlocks;
                m_pBlocks = new_block;
                m_pCurrentBlk = new_block;
            }
            if (m_pCurrentBlk->usedCount == m_blkSize) {
                m_pCurrentBlk = m_pBlocks;
                bool flag = false;
                while (m_pCurrentBlk->usedCount == m_blkSize) {
                    flag = true;
                    m_pCurrentBlk = m_pCurrentBlk->pnext;
                    break;
                }
                if (flag) {
                    block *new_block = newBlock();
                    new_block->pnext = m_pBlocks;
                    m_pBlocks = new_block;
                    m_pCurrentBlk = new_block;
                }
            }
            int free = m_pCurrentBlk->firstFreeIndex;
            int next_free = *reinterpret_cast<int *>(m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex);
            memset(reinterpret_cast<void *>((m_pCurrentBlk->pdata + free)), 0, sizeof(T));
            m_pCurrentBlk->firstFreeIndex = next_free;
            ++m_pCurrentBlk->usedCount;
            T *obj = new((m_pCurrentBlk->pdata + free)) T();
            return obj;
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

            bool* emptyMask = new bool[m_blkSize];
            while (m_pBlocks != nullptr) {
                block *tmp = m_pBlocks;
                if (!m_isDeleteElementsOnDestruct && tmp->usedCount) {
                    throw CException();
                }
                m_pBlocks = m_pBlocks->pnext;
                deleteBlock(tmp, emptyMask);
            }
            delete[] emptyMask;
            m_pBlocks = nullptr;
            m_pCurrentBlk = nullptr;
        }

    private:
        // Создать новый блок данных. применяется в newObject
        block *newBlock() {
            T *data = reinterpret_cast<T *>(new char[m_blkSize * sizeof(T)]);
            auto new_block = new block(data);
            for (int i = 0; i < m_blkSize; ++i) {
                *(reinterpret_cast<int *>(data + i)) = i + 1;
            }
            return new_block;
        }

        // Освободить память блока данных. Применяется в clear
        void deleteBlock(block* p, bool* emptyMask) {
            if (m_isDeleteElementsOnDestruct) {

                std::memset(emptyMask, 0, m_blkSize * sizeof(bool));
                while (p->firstFreeIndex < m_blkSize) {
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