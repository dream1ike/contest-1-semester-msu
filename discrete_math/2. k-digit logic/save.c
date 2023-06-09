#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#pragma warning(disable:4996)

//основные функции:
int* read_vector_table(const char* filename, int* k, int* n, int* vec_size);

int** generate_subsets(int k, int n);

int check_number_in_set(int* set, int set_size, int number);

int convert_to_decimal(int* arr, int k, int n);

void generate_combinations(int* set, int set_size, int* combination, int n, int index, int* vec_table, int table_size, int k, int* is_in_set);

int compare(int* arr1, int* arr2);

void bubble_sort(int** sets, int subset_count);


int main(int argc, const char* argv[]) {
    if (argc == 3) {
        FILE* fout = fopen(argv[2], "w");
        int k, n, table_size;
        int is_in_set = 1;
        int* vec_table = read_vector_table(argv[1], &k, &n, &table_size);
        int** subsets = generate_subsets(k, n);
        int subset_size = (1 << k) - 2;
        int* combination = (int*)malloc(n * sizeof(int));
        bubble_sort(subsets, subset_size);
        for (int i = 0; i < subset_size; i++)
        {
            
            generate_combinations(
                &subsets[i][1], subsets[i][0],
                combination, n, 0, vec_table, table_size, k, &is_in_set);
            if (is_in_set)
            {
                for (int j = 1; j <= subsets[i][0]; j++)
                {
                    fprintf(fout, "%d", subsets[i][j]);
                }
                fprintf(fout, "\n");
            }
            is_in_set = 1;
        }
        free(vec_table);
    }
    return 0;
}



//основные функции:
int* read_vector_table(const char* filename, int* k, int* n, int* vec_size) {
    FILE* fin = fopen(filename, "r");
    char trash;
    fscanf(fin, "%d", k);
    fscanf(fin, "%d", n);
    fscanf(fin, "%c", &trash);
    // Вычисляем количество строк в таблице истинности
    *vec_size = (int)pow((*k), (*n));
    int* table = (int*)malloc(sizeof(int) * (*vec_size));
    for (int i = 0; i < *vec_size; i++)
    {
        fscanf(fin, "%c", &trash);
        if (trash >= 65) table[i] = trash - 55; // расшифровываем буквы
        else table[i] = trash - '0';
    }

    return table;
}


int** generate_subsets(int k, int n) {
    int num_sets = (1 << k) - 2;
    int** sets = (int**)malloc(num_sets * sizeof(int*));
    for (int i = 1; i <= num_sets; i++) {
        int* set = (int*)malloc((n + 1) * sizeof(int));
        int set_size = 0;
        for (int j = 0; j < k; j++) {
            if (i & (1 << j)) {
                set[set_size + 1] = j;
                set_size++;
            }
        }
        set[0] = set_size;
        sets[i-1] = set;
    }
    return sets;
}

int compare(int* arr1, int* arr2) {
    int len1 = arr1[0];
    int len2 = arr2[0];

    int min_len;
    if (len1 < len2) {
        min_len = len1;
    }
    else {
        min_len = len2;
    }

    for (int i = 1; i <= min_len; i++) {
        if (arr1[i] != arr2[i]) {
            return arr1[i] - arr2[i];
        }
    }
    return len1 - len2;
}

void bubble_sort(int** sets, int subset_count) {
    for (int i = 0; i < subset_count - 1; i++) {
        for (int j = 0; j < subset_count - 1 - i; j++) {
            if (compare(sets[j], sets[j + 1]) > 0) {
                int* temp = sets[j];
                sets[j] = sets[j + 1];
                sets[j + 1] = temp;
            }
        }
    }
}

int check_number_in_set(int* set, int set_size, int number)
{
    for (int i = 0; i < set_size; i++)
    {
        if (number == set[i]) return 1;
    }
    return 0;
}

int convert_to_decimal(int* arr, int k, int n) {
    int result = 0;
    for (int i = 0; i < n; i++) {
        result += arr[i] * pow(k, n - i - 1);
    }
    return result;
}

void generate_combinations(int* set, int set_size, int* combination, int n, int index, int* vec_table, int table_size, int k, int* is_in_set) {
    if (index == n) {
        // Если достигнута нужная длина комбинации, делаем проверку на вхождение
        if (*is_in_set)
        {
        int elem_index = convert_to_decimal(combination, k, n);
        *is_in_set = check_number_in_set(set, set_size, vec_table[elem_index]);

        }
        return;
    }
    // Проходим по всем элементам множества и добавляем каждый из них на текущую позицию
    for (int i = 0; i < set_size; i++) {
        combination[index] = set[i];
        generate_combinations(set, set_size, combination, n, index + 1, vec_table, table_size, k, is_in_set);
    }
}



