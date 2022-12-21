#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)
void quicksort(int* l, int* r) {
  if (r - l <= 1) return;
  int z = *(l + (r - l) / 2);
  int* ll = l, *rr = r - 1;
  while (ll <= rr) {
    while (*ll < z) ll++;
    while (*rr > z) rr--;
    if (ll <= rr) {
      swap(*ll, *rr);
      ll++;
      rr--;
    }
  }
  if (l < rr) quicksort(l, rr + 1);
  if (ll < r) quicksort(ll, r);
}
void quickinssort(int* l, int* r) {
  if (r - l <= 32) {
    insertionsort(l, r);
    return;
  }
  int z = *(l + (r - l) / 2);
  int* ll = l, *rr = r - 1;
  while (ll <= rr) {
    while (*ll < z) ll++;
    while (*rr > z) rr--;
    if (ll <= rr) {
      swap(*ll, *rr);
      ll++;
      rr--;
    }
  }
  if (l < rr) quickinssort(l, rr + 1);
  if (ll < r) quickinssort(ll, r);
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
                        free(temp);
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
        for(int i = 0; i < size; i++) fprintf(fout, "%g ", arr[i]);
        fclose(fout);
    }
}
int main(int argc, const char* argv[])
{
    if (argc == 3)
    {
        int size;
        double* arr = read_from_file(argv[1], &size);
        quick_sort(arr, 0, size-1);
        write_to_file(argv[2], arr, size);
        free(arr);
    }
    return 0;
}