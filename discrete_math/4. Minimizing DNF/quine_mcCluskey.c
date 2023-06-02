#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#pragma warning(disable:4996)

char* decToBinary(int n, int size);

char* copy_char_arr(char* original, int size);

void free_two_demensional_char_arr(char** arr, int sizeofarr);

char* merge_vectors(char* vec1, char* vec2, int size);

char** quine_minimizing(char** vec_table, int table_size, int numberofval, int* result_size, char* flag);

int compare_two_vectors(char* vec1, char* vec2, int size);

void write_to_file_two_dementional_char_arr(const char* filename, char** vec, int table_size, int vector_size);

void sort_2d_array(char** arr, int table_size, int vector_size);

char** read_and_find_groups(const char* filename, int* numberofval, int* numberofone); // done

char** delete_similar_arrays(char** vec_table, int table_size, int numberofval, int* result_size);

int main(int argc, const char* argv[])
{
    clock_t start_time = clock();
    if (argc == 3)
    {
        int numberofval, numberofone;
        char** vec_table = read_and_find_groups(argv[1], &numberofval, &numberofone);
        int table_size = numberofone;
        int result_size = 0;
        char flag = 'F';
        while (flag != 'S')
        {
            vec_table = quine_minimizing(vec_table, table_size, numberofval, &result_size, &flag);
            table_size = result_size;
        }
        vec_table = delete_similar_arrays(vec_table, table_size, numberofval, &result_size);
        table_size = result_size;
        sort_2d_array(vec_table, table_size, numberofval);
        write_to_file_two_dementional_char_arr(argv[2], vec_table, result_size, numberofval);
        free_two_demensional_char_arr(vec_table, table_size);
    }
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Elapsed time: %f seconds\n", elapsed_time);
    return 0;
}

char** quine_minimizing(char** vec_table, int table_size, int numberofval, int* result_size, char* flag)
{
    if (*flag != 'F')
        *flag = 'S';
    char** result = NULL;
    *result_size = 0;
    int differences = 0;
    int* number_of_merge = (int*)malloc(sizeof(int) * table_size); // количество слияний определенной строки
    for (int i = 0; i < table_size; i++) number_of_merge[i] = 0;
    for (int i = 0; i < table_size; i++)
    {
        for (int j = i + 1; j < table_size; j++)
        {
            if (abs(vec_table[i][numberofval] - vec_table[j][numberofval]) == 1) // соседние (по единичкам) подгруппы
            {
                differences = compare_two_vectors(vec_table[i], vec_table[j], numberofval);
                if (differences == 1)
                {
                    number_of_merge[i]++;
                    number_of_merge[j]++;
                    if (*flag != 'F') *flag = 'C';
                    *result_size += 1;
                    result = (char**)realloc(result, (*result_size) * sizeof(char*));
                    result[*result_size - 1] = merge_vectors(vec_table[i], vec_table[j], numberofval);
                }
            }
        }
        if (number_of_merge[i] == 0)
        {
            *result_size += 1;
            result = (char**)realloc(result, (*result_size) * sizeof(char*));
            result[*result_size - 1] = copy_char_arr(vec_table[i], numberofval + 1);
        }
    }
    if (*flag == 'F') *flag = 'C';
    free_two_demensional_char_arr(vec_table, table_size);
    return result;
}


char* copy_char_arr(char* original, int size)
{
    char* result = (char*)malloc(sizeof(char*) * size);
    for (int i = 0; i < size; i++) result[i] = original[i];
    return result;
}

void free_two_demensional_char_arr(char** arr, int sizeofarr) // done
{
    for (int i = 0; i < sizeofarr; i++) // очистка массива
    {
        free(arr[i]);
    }
    free(arr);
}
char* decToBinary(int n, int size)
{
    char* binaryNum = (char*)malloc((size + 1) * sizeof(char));
    char number_of_ones = '0';

    // инициализируем массив нулями
    for (int i = 0; i < size; i++) {
        binaryNum[i] = '0';
    }

    int i = size - 1;
    while (n > 0 && i >= 0) {
        if ((n & 1) == 1) number_of_ones++; // считаем кол-во единичек
        binaryNum[i] = (n & 1) + '0'; // устанавливаем биты числа и переводим их в символы '0' или '1
        n >>= 1; // сдвигаем число на одну позицию вправо
        i--;
    }

    binaryNum[size] = number_of_ones; // записываем в виде символа кол-во единичек (для красоты)
    return binaryNum;
}

char** read_and_find_groups(const char* filename, int* numberofval, int* numberofone) // done
{
    FILE* fin = fopen(filename, "r");
    (*numberofone) = 0;
    char** vec_table = NULL;
    if (fin)
    {
        char trash;
        fscanf(fin, "%d", numberofval); // read k-value of logic
        fscanf(fin, "%d", numberofval); // number of variables
        fscanf(fin, "%c", &trash); // read space
        int size = 1 << (*numberofval);
        char* vec_of_val = (char*)malloc(sizeof(char) * size);
        for (int i = 0; i < size; i++)
        {
            fscanf(fin, "%c", &vec_of_val[i]);
            if (vec_of_val[i] == '1')
            {
                (*numberofone)++;
                vec_table = (char**)realloc(vec_table, sizeof(char*) * (*numberofone));
                vec_table[(*numberofone) - 1] = decToBinary(i, *numberofval);
            }
        }
        free(vec_of_val);
        fclose(fin);
    }
    return vec_table;
}


char* merge_vectors(char* vec1, char* vec2, int size)
{
    char* result = (char*)malloc((size + 1) * sizeof(char));

    int i;
    for (i = 0; i < size; i++) {
        if (vec1[i] != vec2[i]) {
            result[i] = '*';
        }
        else {
            result[i] = vec1[i];
        }
    }
    // записываем в конец количество единиц в наборе
    if (vec1[i] > vec2[i]) result[i] = vec1[i] - 1; // записываем '0' или '1'
    else result[i] = vec2[i] - 1;

    return result;
}

int compare_two_vectors(char* vec1, char* vec2, int size) // done
{
    int differences = 0; // счетчик, который отвечает за количество разных элементов

    for (int i = 0; i < size; i++)
    {
        if (vec1[i] != vec2[i]) differences++;
        if (differences > 1) return differences;
    }
    return differences;
}

void write_to_file_two_dementional_char_arr(const char* filename, char** vec, int table_size, int vector_size)
{
    FILE* fout = fopen(filename, "w");
    fprintf(fout, "%d %d\n", vector_size, table_size);
    for (int i = 0; i < table_size; i++)
    {
        for (int j = 0; j < vector_size; j++)
        {
            fprintf(fout, "%c", vec[i][j]);
        }
        fprintf(fout, "\n", vec[i][vector_size]);
    }
}

void sort_2d_array(char** arr, int table_size, int vector_size) {
    int i, j, k;
    for (i = 0; i < table_size - 1; i++) {
        for (j = 0; j < table_size - i - 1; j++) {
            int is_greater = 0;
            for (k = 0; k < vector_size; k++) {
                if (arr[j][k] == '*' && arr[j + 1][k] != '*') {
                    is_greater = 1;
                    break;
                }
                else if (arr[j][k] != '*' && arr[j + 1][k] == '*') {
                    break;
                }
                else if (arr[j][k] > arr[j + 1][k]) {
                    is_greater = 1;
                    break;
                }
                else if (arr[j][k] < arr[j + 1][k]) {
                    break;
                }
            }

            if (is_greater) {
                char* temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

char** delete_similar_arrays(char** vec_table, int table_size, int numberofval, int* result_size) {
    char** result = (char**)malloc(table_size * sizeof(char*));
    int* used = (int*)calloc(table_size, sizeof(int)); // для отметки использованных наборов
    *result_size = 0;

    for (int i = 0; i < table_size; i++) {
        if (used[i] == 0) {
            (*result_size)++;
            result = (char**)realloc(result, (sizeof(char*) * (*result_size)));
            result[*result_size - 1] = copy_char_arr(vec_table[i], numberofval + 1);

            for (int j = i + 1; j < table_size; j++) {
                if (used[j] == 0 && abs(vec_table[i][numberofval] - vec_table[j][numberofval]) == 0) {
                    if (compare_two_vectors(vec_table[i], vec_table[j], numberofval) == 0) { // нулевые различия
                        used[j] = 1; // помечаю одинаковые наборы
                    }
                }
            }
        }
    }
    free(used);
    free_two_demensional_char_arr(vec_table, table_size);
    return result;
}

