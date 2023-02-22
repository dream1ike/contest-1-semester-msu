#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#include <string.h>
#pragma warning(disable:4996)

double* read_from_file (const char* filename, int* size)
{
    double* arr = NULL;
    FILE* fin = fopen(filename, "r");
    if (fin)
    {
        if (fscanf(fin, "%d", size) > 0 && (*size) > 0)
        {
            arr = (double*)malloc(sizeof(double) * (*size));
            if (arr)
            {
                int i = 0;
                while (fscanf(fin, "%lf", &arr[i]) > 0) i++;
                if ((*size) < i)
                {
                    double* temp = (double*)realloc(arr, sizeof(double) * i);
                    *size = i;
                    if (temp)
                    {
                        arr = temp;
                    }
                }
            }
        }
        fclose(fin);
    }
    return arr;
}

void write_to_file(const char* filename, double* arr, int size)
{
    FILE* fout = fopen(filename, "w");
    if (fout)
    {
        fprintf(fout, "%d ", size);
        for(int i = 0; i < size; i++) fprintf(fout, "%g ", arr[i]);
        fclose(fout);
    }
}

void selection_sort(double* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        int maxPos = i;
        for (int j = i + 1; j < size; j++)
        {
            if (arr[maxPos] < arr[j])
                maxPos = j;
            double tmp = arr[maxPos];
            arr[maxPos] = arr[j];
            arr[j] = tmp;
        }
    }
}
int main(int argc, const char* argv[])
{
    if (argc == 3)
    {
        int size;
        double* arr = read_from_file(argv[1], &size);
        selection_sort(arr, size);
        write_to_file(argv[2], arr, size);
    }
    return 0;
}
// Сортировка Выбором 