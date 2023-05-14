#ifndef SORT_HEAD_H_2023_03_15
#define SORT_HEAD_H_2023_03_15


namespace templates
{
    /** Определение типа функции сравнения элементов
    \return
    \li > 0  - pElem1 меньше pElem2
    \li 0   - элементы pElem1 и pElem2 равны
    \li < 0 - pElem1 больше pElem2
    */
    typedef int (CompareSortType)(const void *pElem1, const void *pElem2);

    void mergeSortHelper(void **ppArray, void **tempArray, int length, templates::CompareSortType *pCompareFunc);

    void mergeSort(void **ppArray, int length, CompareSortType pCompareFunc) {
      void **tempArray = new void *[length];
      mergeSortHelper(ppArray, tempArray, length, pCompareFunc);
      delete[] tempArray;
    }

    void mergeSortHelper(void **ppArray, void **tempArray, int length, templates::CompareSortType *pCompareFunc) {
      if (length == 1) {
        return;
      }

      int half = length / 2;
      void **left = ppArray;
      void **right = ppArray + half;

      mergeSortHelper(left, tempArray, half, pCompareFunc);
      mergeSortHelper(right, tempArray + half, length - half, pCompareFunc);

      for (int i = 0, j = 0; i + j < length;) {
        if (i == half) {
          tempArray[i + j] = right[j];
          ++j;
        } else if (j == length - half) {
          tempArray[i + j] = left[i];
          ++i;
        } else if (pCompareFunc(left[i], right[j]) > 0) {
          tempArray[i + j] = left[i];
          ++i;
        } else {
          tempArray[i + j] = right[j];
          ++j;
        }
      }

      for (int i = 0; i < length; ++i) {
        ppArray[i] = tempArray[i];
      }


    }

    void heapSort(void **ppArray, int length, CompareSortType pCompareFunc);

    template <class T>
    void mergeSort(T **ppArray, int length, int (pCompareFunc)(const T *pElem1, const T *pElem2))
    {
      mergeSort(reinterpret_cast<void**>(ppArray), length, (CompareSortType*)pCompareFunc);
    }

    template <class T>
    void heapSort(T **ppArray, int length, int (pCompareFunc)(const T *pElem1, const T *pElem2))
    {
        heapSort(reinterpret_cast<void**>(ppArray), length, (CompareSortType*)pCompareFunc);
    }

    void mergeSortHelper(void **ppArray, void **tempArray, int length, CompareSortType pCompareFunc);
}; // namespace templates

#endif // #define SORT_HEAD_H_2023_03_15

