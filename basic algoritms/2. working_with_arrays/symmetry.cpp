#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma warning(disable:4996)
#define eps 1e-10
int symmetry(double* arr, int size) {
    if (size == 0) { return -1; }
    for (int i = 0; i < size; i++) {
        if (fabs(arr[i] - arr[size-1 - i]) > eps) {
            return 0;
        }
        if (i == size-1-i) return 1;
    }
    return 1;
}
double* read_from_file(const char* fileName, int* size)
{
    FILE* fin = fopen(fileName, "r");
    if (fin)
    {
        double* arr = NULL;
        int i = 0;
        if ((fscanf(fin, "%d", size) > 0))
        {
            if (*size > 0)
            {
                arr = (double*)malloc(sizeof(double) * (*size));
                if (arr) {
                    for (; (i < *size) && (fscanf(fin, "%lf", &arr[i]) > 0); i++);
                    *size = i;
                }
                else return NULL;
            }
            else
            {
                *size = 0;
                return NULL;
            }
        }
        return arr;
        fclose(fin);
    }
    return NULL;
}
void write_to_file(const char* fileName, int* arr, int size) {
    FILE* fout = fopen(fileName, "w");
    if (fout) {
        if (size > 0) {
            for (int i = 0; i < size; i++) {
                fprintf(fout, "%d", arr[i]);
            }
        }
        fclose(fout);
    }
}
int main(int argc, char* argv[]) {
    if (argc == 3) {
        double* arr = NULL;
        int size = 0;
        arr = read_from_file(argv[1], &size);
        int answer = symmetry(arr, size);
        if (answer == -1)
        {
            answer = 0;
            write_to_file(argv[2], &answer, 1);
        }
        write_to_file(argv[2], &answer, 1);
        free(arr);
    }
    return 0;
}
//Симметричны ли значения элементов массива