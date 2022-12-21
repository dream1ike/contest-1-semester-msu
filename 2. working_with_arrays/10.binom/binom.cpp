#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)
int get_num_from_file(const char* filename)
{
    FILE* fin = fopen(filename, "r");
    int x = 0;
    if (fin)
    {
        if (fscanf(fin, "%d", &x) < 0) return -1;
        fclose(fin);
    }
    return x;
}
double* binom (int x)
{
    int size = x+1;
    double* arr = (double*)malloc(sizeof(double) * size);
    if(arr)
    {
        arr[0] = 1;
        arr[1] = 1;
        arr[size-1] = 1;
        double temp_1 = 1, temp_2 = 1;
        for(int i = 2; i < size; i++)
        {
            arr[i-1] = 1;
            arr[0] = 1;
            for(int j = 1; j < i; j++)
            {
                if (j % 2 != 0)
                {
                    temp_1 = arr[j];
                    arr[j] = temp_1 + temp_2;
                }
                else
                {
                    temp_2 = arr[j];
                    arr[j] = temp_1 + temp_2;
                }
            }
            temp_2 = 1;
        }
    } 
    return arr;
}
int write_to_file (const char* filename, double* arr, int size)
{
    FILE* fout = fopen(filename, "w");
    if (fout)
    {
        if (size == 0)
        {
            fprintf(fout, "0");
            return 0;
        }
        fprintf(fout, "%d ", size);
        for(int i = 0; i < size; i++)
        {
            fprintf(fout, "%.0lf ", arr[i]);
        }
        fclose(fout);
    }
    return 0;
}
int main(int argc, const char* argv[])
{
    if (argc == 3)
    {
        int x = get_num_from_file(argv[1]);
        int size = x+1;
        double* arr = binom(x);
        if (x == -1)
        {
             write_to_file (argv[2], arr, 0);
             return 0;
        }
        write_to_file(argv[2], arr, size);
        free(arr);
    }
    return  0;
}
//Вычислить биномиальные коэфициенты