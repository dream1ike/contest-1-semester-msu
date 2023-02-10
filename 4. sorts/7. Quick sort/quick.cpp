#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)
void quick_sort(double* arr, int begin, int end)

{
    int l = begin;
    int r = end;
    int piv = arr[(begin + end)/2];
    do
    {
        while (arr[l] < piv && l < end) l++;
        while (arr[r] > piv && r > begin) r--;
        if (l <= r)
        {
            double temp = arr[l];
            arr[l] = arr[r];
            arr[r] = temp;
            l++;
            r--;
        }
    }while (l<= r);
    if (begin < r) quick_sort(arr, begin, r);
    if (l < end) quick_sort(arr, l, end);
}

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

int main(int argc, const char* argv[])
{
    if (3)
    {
        int size;
        double* arr = read_from_file("in.txt", &size);
        if (arr)
        {
            quick_sort(arr, 0, size-1);
            write_to_file("out.txt", arr, size);
            free(arr);
        }
    }
    return 0;
}