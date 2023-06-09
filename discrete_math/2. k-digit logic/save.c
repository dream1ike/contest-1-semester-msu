#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int check_number_in_set(int* set, int set_size, int number)
{
    for(int i = 0; i < set_size; i++)
    {
        if (number == set[i]) return 1;
    }
    return 0;
}

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

int count_bits(int n) {
    int count = 0;
    while(n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

int** generate_subsets(int k, int n) {
    int num_sets = 1 << k;
    int **sets = (int**)malloc(num_sets * sizeof(int*));
    for(int i = 0; i < num_sets; i++) {
        int *set = (int*)malloc((n+1) * sizeof(int));
        int set_size = 0;
        for(int j = 0; j < k; j++) {
            if(i & (1 << j)) {
                set[set_size+1] = j;
                set_size++;
            }
        }
        set[0] = set_size;
        sets[i] = set;
    }
    return sets;
}



void free_two_demensional_int_arr(int** arr, int sizeofarr) // done
{
    for (int i = 0; i < sizeofarr; i++) // очистка массива
    {
        free(arr[i]);
    }
    free(arr);
}

int convert_to_decimal(int* arr, int k, int n) {
    int result = 0;
    for(int i = 0; i < n; i++) {
        result += arr[i] * pow(k, n - i - 1);
    }
    return result;
}

void print_arr(int* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void generate_combinations_and_check(int* set, int set_size, int* vec_table, int table_size, int* combination, int n, int index, int k, int* is_in_set) {
    if(index == n) {
        // Если достигнута нужная длина комбинация, возвращаем ее
        if (*is_in_set)
        {
            int index = convert_to_decimal(combination, k, set_size); // получили индекс нужного нам набора
            *is_in_set = check_number_in_set(set, set_size, vec_table[index]);
            print_arr(combination, set_size);
        }
        return;
    }
    // Проходим по всем элементам множества и добавляем каждый из них на текущую позицию
    for(int i = 0; i < set_size; i++) {
        combination[index] = set[i];
        generate_combinations_and_check(set, set_size, vec_table, table_size, combination, n, index + 1, k, is_in_set);
    }
}


int main(int argc, const char* argv[]) {
    FILE* fout = fopen("/Users/timurbajdadaev/Visual_Studio_Code/Contest/file1.txt", "w");
    if (3){
    int k,n;
    int table_size;
    int* vec_table = read_vector_table("/Users/timurbajdadaev/Visual_Studio_Code/Contest/file2.txt",&k, &n, &table_size);
    printf("k: %d\nn: %d\n", k,n);
    for(int i = 0; i < table_size; i++)
    {
        printf("%d ", vec_table[i]);
    }
    
    
    printf("\n");
    int subset_size = 1 << (k); 
    int **sets = generate_subsets(k,n);
    int* combination = (int*)malloc(n * sizeof(int));
    int is_in_set = 1;
    printf("\n");

    // проверка на вхождение
    for (int i = 1; i < subset_size - 1; i++)
    {
        generate_combinations_and_check(&sets[i][1], sets[i][0], vec_table, table_size, combination, n, 0, k, &is_in_set);
        printf("--------\n");
        print_arr(&sets[i][1], sets[i][0]);
        printf("--------\n");
        is_in_set = 1;
    }
    free_two_demensional_int_arr(sets, subset_size);
    fclose(fout);
    }
    return 0;
}
