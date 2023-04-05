#ifndef AVL_HEAD_H_2023_03_30
#define AVL_HEAD_H_2023_03_30

#include "mm.h"

namespace lab618
{
    template <class T, int(*Compare)(const T *pElement, const T* pElement2) >
    class CAVLTree
    {
    private:
        struct leaf
        {
            T* pData;
            leaf *pLeft;
            leaf *pRight;
            int balanceFactor;
        };

    public:
        class CException
        {
        public:
            CException()
            {
            }
        };

    public:
        CAVLTree(int defaultBlockSize) : m_pRoot(nullptr), m_Memory(defaultBlockSize)
        {
        }

        virtual ~CAVLTree()
        {
          clear();
        }

        bool add(T* pElement)
        {
          return addRecursively(m_pRoot, pElement);
        }

        bool update(T* pElement)
        {
          T* pOldElement = find(*pElement);
          if (pOldElement != nullptr)
          {
            *pOldElement = *pElement;
            return true;
          }
          return false;
        }

        T* find(const T& pElement)
        {
          return findRecursively(m_pRoot, pElement);
        }

        bool remove(const T& element)
        {
          return removeRecursively(m_pRoot, element);
        }

        void clear()
        {
          clearRecursively(m_pRoot);
          m_pRoot = nullptr;
        }

    private:
        leaf* m_pRoot;
        CMemoryManager<leaf> m_Memory;

        bool addRecursively(leaf*& pNode, T* pElement)
        {
          if (pNode == nullptr)
          {
            pNode = m_Memory.newObject();
            pNode->pData = pElement;
            pNode->pLeft = pNode->pRight = nullptr;
            pNode->balanceFactor = 0;
            return true;
          }

          int compResult = Compare(pElement, pNode->pData);
          if (compResult == 0)
          {
            return false;
          }

          bool isHeightIncreased = false;
          if (compResult < 0)
          {
            isHeightIncreased = addRecursively(pNode->pLeft, pElement);
            if (isHeightIncreased)
            {
              pNode->balanceFactor--;
            }
          }
          else
          {
            isHeightIncreased = addRecursively(pNode->pRight, pElement);
            if (isHeightIncreased)
            {
              pNode->balanceFactor++;
            }
          }

          if (isHeightIncreased && (pNode->balanceFactor == -2 || pNode->balanceFactor == 2))
          {
            balanceNode(pNode);
            isHeightIncreased = false;
          }

          return isHeightIncreased;
        }

        T* findRecursively(leaf* pNode, const T& pElement)
        {
          if (pNode == nullptr)
          {
            return nullptr;
          }

          int compResult = Compare(&pElement, pNode->pData);
          if (compResult == 0)
          {
            return pNode->pData;
          }
          else if (compResult < 0)
          {
            return findRecursively(pNode->pLeft, pElement);
          }
          else
          {
            return findRecursively(pNode->pRight, pElement);
          }
        }

        void clearRecursively(leaf* pNode)
        {
          if (pNode == nullptr)
          {
            return;
          }

          clearRecursively(pNode->pLeft);
          clearRecursively(pNode->pRight);
          m_Memory.deleteObject(pNode);
        }

        bool removeRecursively(leaf*& pNode, const T& element)
        {
          if (pNode == nullptr)
          {
            return false;
          }

          int compResult = Compare(&element, pNode->pData);
          bool isHeightDecreased = false;
          if (compResult == 0)
          {
            if (pNode->pLeft == nullptr || pNode->pRight == nullptr)
            {
              leaf* pTemp = (pNode->pLeft != nullptr) ? pNode->pLeft : pNode->pRight;
              m_Memory.deleteObject(pNode);
              pNode = pTemp;
              isHeightDecreased = true;
            }
            else
            {
              leaf* pMinNode = findMinNode(pNode->pRight);
              pNode->pData = pMinNode->pData;
              isHeightDecreased = removeRecursively(pNode->pRight, *pMinNode->pData);
              if (isHeightDecreased)
              {
                pNode->balanceFactor--;
              }
            }
          }
          else if (compResult < 0)
          {
            isHeightDecreased = removeRecursively(pNode->pLeft, element);
            if (isHeightDecreased)
            {
              pNode->balanceFactor++;
            }
          }
          else
          {
            isHeightDecreased = removeRecursively(pNode->pRight, element);
            if (isHeightDecreased)
            {
              pNode->balanceFactor--;
            }
          }

          if (isHeightDecreased && (pNode == nullptr || pNode->balanceFactor == -2 || pNode->balanceFactor == 2))
          {
            balanceNode(pNode);
            isHeightDecreased = (pNode->balanceFactor == 0);
          }

          return isHeightDecreased;
        }

        leaf* findMinNode(leaf* pNode)
        {
          while (pNode->pLeft != nullptr)
          {
            pNode = pNode->pLeft;
          }
          return pNode;
        }

        void rotateLeft(leaf*& pNode)
        {
          if (pNode == nullptr || pNode->pRight == nullptr)
          {
            return;
          }

          leaf* pRight = pNode->pRight;
          pNode->pRight = pRight->pLeft;
          pRight->pLeft = pNode;
          pNode = pRight;
        }

        void rotateRight(leaf*& pNode)
        {
          if (pNode == nullptr || pNode->pLeft == nullptr)
          {
            return;
          }

          leaf* pLeft = pNode->pLeft;
          pNode->pLeft = pLeft->pRight;
          pLeft->pRight = pNode;
          pNode = pLeft;
        }

        void balanceNode(leaf*& pNode)
        {
          if (pNode->balanceFactor == 2)
          {
            if (pNode->pRight != nullptr && pNode->pRight->balanceFactor < 0)
            {
              rotateRight(pNode->pRight);
            }
            rotateLeft(pNode);
          }
          else if (pNode->balanceFactor == -2)
          {
            if (pNode->pLeft != nullptr && pNode->pLeft->balanceFactor > 0)
            {
              rotateLeft(pNode->pLeft);
            }
            rotateRight(pNode);
          }

          if (pNode->balanceFactor == 1 || pNode->balanceFactor == -1)
          {
            pNode->balanceFactor = 0;
          }
          else
          {
            pNode->balanceFactor = (pNode->balanceFactor > 0) ? pNode->balanceFactor - 1 : pNode->balanceFactor + 1;
          }
        }

    };

}; // namespace lab618

#endif // #define AVL_HEAD_H_2023_03_30

