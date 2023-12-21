#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct element {
    char *name;
    int size;
    int isfile;
    int number_of_kids;
    struct element **kids;
    struct element *parent;
} element;

// Функция для обмена указателями на элементы
void swap(element **a, element **b) {
    element *temp = *a;
    *a = *b;
    *b = temp;
}

// Функция для сравнения элементов по полю name
int compareElements(const element *a, const element *b) {
    return strcmp(a->name, b->name);
}

// Функция для сортировки пузырьком
void bubbleSort(element **arr, int size) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            // Сравниваем элементы по полю name
            if (compareElements(arr[j], arr[j + 1]) > 0) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}
char** get_all_names_from_dir(element* parent_path, int dir_first)
{
    char** names = NULL;
    int size = parent_path->number_of_kids;
    element** arr = (element**)malloc(sizeof(element*) * size);
    
    // Копируем во временный массив, чтобы не перемешались указатели
    for (int i = 0; i < size; i++)
    {
        arr[i] = parent_path->kids[i];
    }

    bubbleSort(arr, size);

    if (dir_first)
    {
        int dirs_number = 0;
        int files_number = 0;

        // Считаем, сколько всего файлов и директорий
        for (int i = 0; i < size; i++)
        {
            if (!arr[i]->isfile) dirs_number++;
            else files_number++;
        }

        // Создаем отдельный массив для файлов и директорий
        element** arr_dirs = (element**)malloc(sizeof(element*) * dirs_number);
        element** arr_files = (element**)malloc(sizeof(element*) * files_number);
        int dirs_count = 0;
        int files_count = 0;
        for (int i = 0; i < size; i++)
        {
            if (!arr[i]->isfile)
            {
                arr_dirs[dirs_count] = arr[i];
                dirs_count++;
            }
            else
            {
                arr_files[files_count] = arr[i];
                files_count++;
            }
        }
        
        // Соединяем в один массив
        int arr_index = 0;
        
        for (int i = 0; i < dirs_count; i++)
        {
            arr[arr_index] = arr_dirs[i];
            arr_index++;
        }
        
        for (int i = 0; i < files_count; i++)
        {
            arr[arr_index] = arr_files[i];
        }
        
    }
    names = (char**)malloc(sizeof(char*) * size);
    if (names)
    {
        for (int i = 0; i < size; i++)
        {
            names[i] = strdup(arr[i]->name);
        }
    }
    free(arr);
    return names;
    
}
// Функция для печати массива элементов
void printElements(element **elements, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%s\n", elements[i]->name);
    }
}

int main() {
    // Пример использования

    // Создаем несколько элементов
    element elem1 = {"1f", 10, 1, 0, NULL, NULL};
    element elem2 = {"2f", 20, 0, 0, NULL, NULL};
    element elem3 = {"3f", 15, 1, 0, NULL, NULL};
    element elem4 = {"4d", 0, 0, 0, NULL, NULL};
    element elem5 = {"5d", 0, 0, 0, NULL, NULL};

    // Создаем массив указателей на элементы
    element *elements[] = {&elem1, &elem2, &elem3, &elem4, &elem5};
    int size = sizeof(elements) / sizeof(elements[0]);
    int directory_first = 0;
    element parent_path = {"parent", 0, 1, size, elements, NULL};
    // Сортируем и выводим элементы
    char** names = get_all_names_from_dir(&parent_path, 1);

    for (int i = 0; i < parent_path.number_of_kids; i++)
    {
        printf("%s\n", names[i]);
        free(names[i]);
    }
    free(names);
    return 0;
}
