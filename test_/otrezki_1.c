#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#define eps 1e-6

double Get_Our_X(FILE *a) {
    double x = 0;
    fscanf(a, "%lf", &x);
    return x;
}

int Get_Our_Size(FILE* a) {
    int size = 0;
    fscanf(a, "%d", &size);
    return size;
}

double* Get_Our_Array(FILE* a, int* size , double* array) {
    int i = 0;
    array = (double*)malloc(sizeof(double) * (*size));
    while (fscanf(a, "%lf", &array[i]) > 0 && i < (*size)){
        i++;
    }
    if (i < (*size)) {
        (*size) = i;
        array = (double*)realloc(array, sizeof(double) * (*size));
    }
    return array;
}

void Get_Our_Other_Array(double* array, int size, double x) {
    int start = 0, end = 0;
    double result = 0;
    for (int i = 0; i < size; i++) {
        if (fabs(array[i] - x) < eps) {
            if (i == 0) {
                start = 0;
            }
            else {
                if (fabs(array[i] - array[i - 1])>eps) {
                    start = i;
                }
            }
            if (start == 0) {
                if (i == size - 1) {
                    result = 0;
                    end = size - 1;
                    if (start != end) {
                        for (; start <= end; start++) {
                            array[start] = result;
                        }
                    }
                    else {
                        array[end] = result;
                    }
                }
                else {
                    if (fabs(array[i] - array[i + 1])>eps) {
                        end = i;
                        result = (array[end + 1]) / 2;
                        if (start != end) {
                            for (; start <= end; start++) {
                                array[start] = result;
                            }
                        }
                        else {
                            array[end] = result;
                        }
                    }
                }
            }
                if (start != 0) {
                    if (i == size - 1) {
                        end = size - 1;
                        result = array[start - 1] / 2;
                        if (start != end) {
                            for (; start <= end; start++) {
                                array[start] = result;
                            }
                        }
                        else {
                            array[end] = result;
                        }
                    }
                    else {
                        if (fabs(array[i] - array[i + 1])>eps) {
                            end = i;
                            result = (array[start - 1] + (array[end + 1])) / 2;
                            if (start != end) {
                                for (; start <= end; start++) {
                                    array[start] = result;
                                }
                            }
                            else {
                                array[end] = result;
                            }
                        }
                    }
                }
        }  
    }
}

int main(int argc,char* argv[])
{
    FILE* a, * b;
    double x = 0;
    int size = 0;
    double* array = NULL;
    if (argc == 3) {
        a = fopen(argv[1], "r");
        b = fopen(argv[2], "w");

        if (a != NULL && b != NULL) {
            x = Get_Our_X(a);
            size = Get_Our_Size(a);
            if (size >= 0) {
                array = Get_Our_Array(a, &size, array);
                if (array != NULL) {
                    Get_Our_Other_Array(array, size, x);
                    if (array != NULL) {
                        fprintf(b, "%d ", size);
                        for (int i = 0; i < size; i++) {
                            fprintf(b, "%g ", array[i]);
                        }
                    }
                }
                else {
                    return -1;
                }
            }
            else {
                return -1;
            }
        }
        if (a != NULL) {
            fclose(a);
        }
        if (b != NULL) {
            fclose(b);
        }
    }  
    free(array);
    return 0;
}


// 3
// 4 3 3 3 3 4 5 6
// while (x == arr[i]);
// begin = 2
// end = 6
// ...
// sum = 4

// от бегина до энда
// заменять элемент на сумму
