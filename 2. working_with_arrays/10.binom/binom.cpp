#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)
double factorial(int n) {
    if (n == 0 || n == 1)return 1;
    return factorial(n - 1) * n;
}
double* binom(int n)
{
    double* arr = (double*)malloc(sizeof(double) * (n + 1));
    if (arr)
    {
        double n_fact = factorial(n);
        arr[0] = 1;
        arr[n] = 1;
        for (int k = 1; k <= n; k++)
        {
            arr[k] = (n_fact) / (factorial(k) * factorial(n - k));
        }
        return arr;
    }
    return NULL;
}
int main(int argc, const char* argv[])
{
    if (3) {
        int n = 0;
        FILE* fin = fopen(argv[1], "r");
        FILE* fout = fopen(argv[2], "w");
        if (fin && fout)
        {
            if (fscanf(fin, "%d", &n) > 0 && n >= 0)
            {
                {
                    double* arr = binom(n);
                    fprintf(fout, "%d ", (n + 1));
                    for (int i = 0; i < n + 1; i++)
                    {
                        fprintf(fout, "%g ", arr[i]);
                    }
                    free(arr);
                }
            }
            else fprintf(fout, "0");
            fclose(fout);
            fclose(fin);
        }
    }
    return  0;
}
//Вычислить биномиальные коэфициенты