#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)
double get_num_from_file(const char* filename)
{
    FILE* fin = fopen(filename, "r");
    int x = 0;
    if (fin)
    {
        fscanf(fin, "%d", &x);
        fclose(fin);
    }
    return x;
}
double* binom (int x)
{
    double* arr = NULL;
    int size = x+1;
    arr = (double*)malloc(sizeof(double) * size);
    if (arr && x >= 1)
    {
        arr[0] = 1;
        arr[1] = 1;
        arr[size-1] = 1;
        int temp_1 = 1, temp_2 = 1;
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
void write_to_file (const char* filename, double* arr, int size)
{
    FILE* fout = fopen(filename, "w");
    if (fout)
    {
        fprintf(fout, "%d ", size);
        for(int i = 0; i < size; i++)
        {
            fprintf(fout, "%g ", arr[i]);
        }
        fclose(fout);
    }
}
int main(int argc, const char* argv[])
{
    if (3)
    {
        int x = get_num_from_file("in.txt");
        double* arr = binom(x);
        write_to_file("out.txt", arr, x+1);
        free(arr);
    }
    return  0;
}
//Вычислить биномиальные коэфициенты