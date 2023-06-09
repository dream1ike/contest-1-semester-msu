#include <stdio.h>
#include <stdlib.h>
// Функция для сравнения двух подмассивов
int compare(const void* a, const void* b) {
    const int* arr1 = *(const int**)a;
    const int* arr2 = *(const int**)b;

    int size1 = arr1[0];
    int size2 = arr2[0];

    int i = 1, j = 1;
    while (i <= size1 && j <= size2) {
        if (arr1[i] < arr2[j])
            return -1;
        else if (arr1[i] > arr2[j])
            return 1;
        
        i++;
        j++;
    }

    // Если один подмассив является префиксом другого
    if (i <= size1)
        return 1;
    if (j <= size2)
        return -1;

    return 0;
}

// Функция для сортировки наборов в лексикографическом порядке
void sortLexicographically(int** sets, int numSets) {
    qsort(sets, numSets, sizeof(int*), compare);
}

// Функция для вывода наборов
void printSets(int** sets, int numSets) {
    for (int i = 0; i < numSets; i++) {
        int size = sets[i][0];
        for (int j = 1; j <= size; j++) {
            printf("%d ", sets[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int sets[][4] = {
        {3, 0, 1, 2},
        {2, 1, 3},
        {3, 0, 2, 3},
        {1, 2},
        {4, 0, 1, 2, 3},
        {2, 0, 1},
        {2, 2, 3},
        {1, 0},
        {1, 1},
        {1, 3},
        {3, 0, 1, 3},
        {2, 0, 3},
        {3, 1, 2, 3},
        {3, 0, 1, 2}
    };

    int numSets = sizeof(sets) / sizeof(sets[0]);

    // Приведение типов двумерного массива
    int* setsPtr[numSets];
    for (int i = 0; i < numSets; i++) {
        setsPtr[i] = sets[i];
    }

    sortLexicographically(setsPtr, numSets);
    printSets(setsPtr, numSets);

    return 0;
}
