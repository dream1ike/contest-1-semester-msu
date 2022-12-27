#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#pragma warning(disable:4996)
#define eps 1e-10 
typedef struct {
    double x1;
    double x2;
    int position;
}segment;

int checking_for_overlap(segment a, segment b)
{
    if (a.x1 <= b.x1 <= a.x2 || a.x1 <= b.x2 <= a.x2) return 1;
    if (b.x1 <= a.x1 <= b.x2 || b.x1 <= a.x2 <= b.x2) return 1;
    return 0;
}

void quick_sort(segment* arr, int begin, int end)
{
    int l = begin;
    int r = end;
    int piv = (begin + end)/2;
    do
    {
        while (arr[l].x1 < piv && l < end) l++;
        while (arr[r].x1 > piv && r > begin) r--;
        if (l <= r)
        {
            double temp = arr[l].x1;
            arr[l].x1 = arr[r].x1;
            arr[r].x1 = temp;
        }
    }while (l<= r);
    if (begin < r) quick_sort(arr, begin, r);
    if (l < end) quick_sort(arr, l, end);
}

segment* read_segments_from_file(const char* filename, int* size)
{
    FILE* fin = fopen(filename, "r");
    segment* arr = NULL;
    if(fin)
    {
        if (fscanf(fin, "%d ", size) > 0)
        {
            arr = (segment*)malloc(sizeof(segment) * (*size));
            if (arr)
            {
                int i = 0;
                while(fscanf(fin, "%lf", &arr[i].x1) > 0 && fscanf(fin, "%lf", &arr[i].x2) > 0) {
                    arr[i].position = i;
                }
                if (i < *size)
                {
                    *size = i;
                    segment* temp = (segment*)realloc(arr, i);
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

void write_segments_to_file (const char* filename, segment* arr, int size)
{
	FILE* fout = fopen(filename, "w");
    if(fout)
    {
        for(int i = 0; i < size; i++) fprintf(fout, " {%g,%g} ", arr[i].x1, arr[i].x2);
        fclose(fout);
    }
}

int main()
{
    int size;
    segment* arr = read_segments_from_file("in.txt", &size);
    if(arr)
    {
        quick_sort(arr, 0, size-1);
        write_segments_to_file("out.txt", arr, size);
        free(arr);
    }
    return 0;
}