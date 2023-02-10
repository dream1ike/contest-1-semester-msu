#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#include <string.h>
#pragma warning(disable:4996)

int removeDuplicates(int* nums, int numsSize){
    int i = 0, j = 1;
    int k = numsSize;
    while (i < numsSize -1)
    {
        
    }
    return k;
}


int* read_from_file (const char* filename, int* size)
{
    int* arr = NULL;
    FILE* fin = fopen(filename, "r");
    if (fin)
    {
        if (fscanf(fin, "%d", size) > 0 && (*size) > 0)
        {
            arr = (int*)malloc(sizeof(int) * (*size));
            if (arr)
            {
                int i = 0;
                while (fscanf(fin, "%d", &arr[i]) > 0) i++;
                if ((*size) < i)
                {
                    int* temp = (int*)realloc(arr, sizeof(int) * i);
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
int main()
{
    int size;
    int* nums = read_from_file("in.txt", &size);
	return removeDuplicates(nums, size);
}