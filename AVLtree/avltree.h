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
            int diff;
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

        bool update(T* pElement) {
          leaf* elem_leaf = findLeaf(m_pRoot, pElement);
          if (elem_leaf == nullptr) {
            add(pElement);
            return false;
          } else {
            elem_leaf->pData = pElement;
            return true;
          }
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

        int getDiff(leaf* pNode) {
          if (pNode != nullptr) {
            return pNode->diff;
          }
          return 0;
        }

        int getDiffDiff(leaf* pNode) {
          return getDiff(pNode->pRight) - getDiff(pNode->pLeft);
        }

        void changeDiff(leaf* pNode) {
          if (getDiff(pNode->pLeft) > getDiff(pNode->pRight)) {
            pNode->diff = getDiff(pNode->pLeft) + 1;
          } else {
            pNode->diff = getDiff(pNode->pRight) + 1;
          }
        }

        leaf* rotateLeft(leaf*& pNode)
        {
          leaf* pRight = pNode->pRight;
          pNode->pRight = pRight->pLeft;
          pRight->pLeft = pNode;
          changeDiff(pNode);
          changeDiff(pRight);
          return pRight;
        }

        leaf* rotateRight(leaf*& pNode)
        {
          leaf* pLeft = pNode->pLeft;
          pNode->pLeft = pLeft->pRight;
          pLeft->pRight = pNode;
          changeDiff(pNode);
          changeDiff(pLeft);
          return pLeft;
        }

        leaf* balanceNode(leaf*& pNode)
        {
          changeDiff(pNode);
          if (getDiffDiff(pNode) == 2)
          {
            if (getDiffDiff(pNode->pRight) < 0)
            {
              pNode->pRight = rotateRight(pNode->pRight);
            }
            pNode = rotateLeft(pNode);
          }
          else if (getDiffDiff(pNode) == -2)
          {
            if (getDiffDiff(pNode->pLeft) > 0)
            {
              pNode->pLeft = rotateLeft(pNode->pLeft);
            }
            pNode = rotateRight(pNode);
          }
          return pNode;
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
            changeDiff(pNode);
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
            changeDiff(pNode);
            balanceNode(pNode);
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

        leaf* findLeaf(leaf* p, const T* pData) {
          if (p == nullptr) {
            return nullptr;
          }
          if (Compare(pData, p->pData) < 0) {
            return findLeaf(p->pLeft, pData);
          }
          if (Compare(pData, p->pData) == 0) {
            return p;
          }
          return findLeaf(p->pRight, pData);
        }

    };

}; // namespace lab618

#endif // #define AVL_HEAD_H_2023_03_30

