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

void swap_prev_with_curr (double* arr, int i)
{
    double cache = arr[i];
    arr[i] = arr[i-1];
    arr[i-1] = cache;

}

double* sheker_sort (double* arr, int size)
{
    int left_mark = 1;
    int right_mark = size-1;
    while (left_mark <= right_mark)
    {
        for (int i = right_mark; i >= left_mark; i--)
            if (arr[i-1] > arr[i]) swap_prev_with_curr(arr, i);
        left_mark++;
        for (int i = left_mark; i <= right_mark; i++)
            if (arr[i-1] > arr[i]) swap_prev_with_curr(arr, i);
        right_mark--;
    }
    return arr;
}

int main(int argc, const char* argv[])
{
    if (argc == 3)
    {
        int size;
        double* arr = read_from_file(argv[1], &size);
        sheker_sort(arr, size);
        write_to_file(argv[2], arr, size);
    }
    return 0;
}
// Сортировка Шейкером 