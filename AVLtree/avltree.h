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
            int diff;  // difference in height
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

        // Utility function to update height of a node
        void updateHeight(leaf *pNode) {
          int heightLeft = pNode->pLeft ? pNode->pLeft->diff + 1 : 0;
          int heightRight = pNode->pRight ? pNode->pRight->diff + 1 : 0;
          pNode->diff = heightLeft - heightRight;
        }

        void rotateLeft(leaf*& pNode)
        {
          leaf* pRight = pNode->pRight;
          pNode->pRight = pRight->pLeft;
          pRight->pLeft = pNode;
          updateHeight(pNode);
          updateHeight(pRight);
          pNode = pRight;
        }

        void rotateRight(leaf*& pNode)
        {
          leaf* pLeft = pNode->pLeft;
          pNode->pLeft = pLeft->pRight;
          pLeft->pRight = pNode;
          updateHeight(pNode);
          updateHeight(pLeft);
          pNode = pLeft;
        }

        void bigRotateLeft(leaf*& pNode)
        {
          rotateRight(pNode->pRight);
          rotateLeft(pNode);
        }

        void bigRotateRight(leaf*& pNode)
        {
          rotateLeft(pNode->pLeft);
          rotateRight(pNode);
        }

        bool addRecursively(leaf*& pNode, T* pElement)
        {
          if (pNode == nullptr)
          {
            pNode = m_Memory.newObject();
            pNode->pData = pElement;
            pNode->pLeft = pNode->pRight = nullptr;
            pNode->diff = 0;
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
          }
          else
          {
            isHeightIncreased = addRecursively(pNode->pRight, pElement);
          }

          if (isHeightIncreased)
          {
            updateHeight(pNode);
            balanceNode(pNode);
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
            if (pNode->pLeft == nullptr && pNode->pRight == nullptr)
            {
              m_Memory.deleteObject(pNode);
              pNode = nullptr;
              isHeightDecreased = true;
            }
            else if (pNode->pLeft == nullptr || pNode->pRight == nullptr)
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
                pNode->diff--;
              }
            }
          }
          else if (compResult < 0)
          {
            isHeightDecreased = removeRecursively(pNode->pLeft, element);
            if (isHeightDecreased)
            {
              pNode->diff++;
            }
          }
          else
          {
            isHeightDecreased = removeRecursively(pNode->pRight, element);
            if (isHeightDecreased)
            {
              pNode->diff--;
            }
          }

          if (isHeightDecreased && pNode != nullptr)
          {
            updateHeight(pNode);
            balanceNode(pNode);
          }

          return isHeightDecreased;
        }


        void balanceNode(leaf*& pNode)
        {
          if (pNode->diff == -2)
          {
            if (pNode->pRight != nullptr && pNode->pRight->diff <= 0)
            {
              rotateLeft(pNode);
            }
            else
            {
              bigRotateLeft(pNode);
            }
          }
          else if (pNode->diff == 2)
          {
            if (pNode->pLeft != nullptr && pNode->pLeft->diff >= 0)
            {
              rotateRight(pNode);
            }
            else
            {
              bigRotateRight(pNode);
            }
          }
        }

        leaf* findMinNode(leaf* pNode)
        {
          while (pNode->pLeft != nullptr)
          {
            pNode = pNode->pLeft;
          }
          return pNode;
        }

    };

}; // namespace lab618

#endif // #define AVL_HEAD_H_2023_03_30

