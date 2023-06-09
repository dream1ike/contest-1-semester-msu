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

int compare(const void* a, const void* b) {
    const int* arr1 = *(const int**)a;
    const int* arr2 = *(const int**)b;

    int size1 = arr1[0];
    int size2 = arr2[0];

    int i = 1, j = 1;
    while (i <= size1 && j <= size2) {
        if (arr1[i] < arr2[j])
            return -1;
        else if (arr1[i] > arr2[j])
            return 1;
        
        i++;
        j++;
    }

    // Если один подмассив является префиксом другого
    if (i <= size1)
        return 1;
    if (j <= size2)
        return -1;

    return 0;
}

// Функция для сортировки наборов в лексикографическом порядке
void sortLexicographically(int** sets, int numSets) {
    qsort(sets, numSets, sizeof(int*), compare);
}

int convert_to_decimal(int* arr, int k, int n) {
    int result = 0;
    for(int i = 0; i < n; i++) {
        result += arr[i] * pow(k, n - i - 1);
    }
    return result;
}

void print_set(int* set, int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("%d ", set[i]);
    }
    printf("\n");
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

void generate_combinations(int* set, int set_size, int* vec_table, int table_size, int* combination, int n, int k, int index,  int* is_in_set) {
    if(index == n) {
        // Если достигнута нужная длина комбинации, выводим её

        print_set(combination, n);

        if (*is_in_set)
        {
        // узнаем индекс нужного нам элемента в вектор-таблице
        int elem_index = convert_to_decimal(combination, n, k);
        // проверка на вхождение во множество
        *is_in_set = check_number_in_set(set, set_size, vec_table[elem_index]);
        }

        //printf("is_in_set: %d\n", *is_in_set);
        return;
    }
    // Проходим по всем элементам множества и добавляем каждый из них на текущую позицию
    for(int i = 0; i < set_size; i++) {
        combination[index] = set[i];
        generate_combinations(set, set_size, vec_table, table_size, combination, n, k, index + 1, is_in_set);
    }
}

int main(int argc, const char* argv[]) {
    if (3){
        int k, n, table_size;
        int is_in_set = 1;
        FILE* fout = fopen("/Users/timurbajdadaev/Visual_Studio_Code/Contest/file1.txt", "w");


        int* vec_table = read_vector_table("/Users/timurbajdadaev/Visual_Studio_Code/Contest/file2.txt", &k, &n, &table_size);
        int* combination = (int*)malloc(n * sizeof(int));
        int subset_count = 1 << k;
        int** sets = generate_subsets(k, n);
        subset_count--;
        free(sets[subset_count]);
        sortLexicographically(sets, subset_count);
        for (int i = 1; i < subset_count; i++) {
            printf("set: ");
            print_set(&sets[i][1], sets[i][0]);
            generate_combinations(&sets[i][1], sets[i][0], vec_table, table_size, combination, n, k, 0, &is_in_set);

            printf("is_in_set: %d\n", is_in_set);
            is_in_set = 1;
            //free(sets[i]);
        }
    }
    return 0;
}
