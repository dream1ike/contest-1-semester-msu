#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "os_mem.h"
#include <iostream>
#pragma warning(disable:4996)

typedef struct element
{
    char name[32];
    int numb_of_links;
    element* links[16];
    // g++ -x c++ -std=c++11 *main.cpp *task.cpp -fsanitize=address -o out
}element;

element* ARR = NULL;
int NUM_OBJ_MAX = 0;
int NUM_OBJ_CURR = 0;
// Инициализация вспомогательных функций
int removeElement(element* links[], int size, const char* targetName);

void copy_plus_vibe(element* destination, element* old_arr); // очень вайбовая фукнция, спасла весь код

int checkMemoryManagerAvilability(int new_elem); // делает основные проверки

void delete_all_links_to_obj_from_each_of_element_arr(char* name, int size);

void sort_strings_in_element(element* arr, int size); // сортирует строки

void good_realloc(int new_size, int old_size); // нормальный реаллок без утечек (в теории)

int find_element(element* arr, const char* name, int arr_size); // возвращает индекс элемента

void destroy_elem_and_shift_array(int index, int size); // удаляет с массива элемент
// ************************************


// Инициализация моих функций
int my_create(int size, int num_pages);

int my_destroy();

int my_create_object(const char* name);

int my_destroy_object(const char* name);

void my_print_objects();

int my_link(const char* object1_name, const char* object2_name);

void print_link_counts();
// ************************************
// Инициализация мемори менеджера

void setup_memory_manager(memory_manager_t* mm) {
    mm->create = my_create;
    mm->destroy = my_destroy;
    mm->create_object = my_create_object;
    mm->destroy_object = my_destroy_object;
    mm->print_objects = my_print_objects;
    mm->link = my_link;
    mm->print_link_counts = print_link_counts;
}

// ************************************

// мейн ого
// ************************************

// Работа вспомогательных функций

int removeElement(element* links[], int size, const char* targetName)
{
    // Поиск элемента по имени
    int indexToRemove = -1;
    for (int i = 0; i < size; ++i) {
        if (links[i] != NULL) {
            if (!strcmp(links[i]->name, targetName))
            {
                indexToRemove = i;
                break;
            }
        }
        else
        {
            break;
        }
    }
    // Если элемент найден, производим удаление и сдвиг
    if (indexToRemove != -1) {
        // Сдвигаем оставшиеся элементы
        for (int i = indexToRemove; i < size - 1; ++i)
        {
            links[i] = links[i + 1];
        }
        // Последний элемент делаем NULL, так как мы его сдвинули
        links[size - 1] = NULL;
        return 1;
    }
    return 0;
}

void delete_all_links_to_obj_from_each_of_element_arr(char* name, int size)
{
    size = NUM_OBJ_CURR;
    for (int i = 0; i < size; i++)
    {
        if (ARR[i].links[0] != NULL)
        {
            removeElement(ARR[i].links, 16, name);
        }
    }
}
// links[0] = ***
// links[1] = NULL

void copy_plus_vibe(element* destination, element* old_arr)
{
    for (int j = 0; j < 16; j++)
    {
        destination->links[j] = old_arr->links[j];
    }
    // memcpy(void *dst, const void *src, size_t n) - аналог copy_plus_vibe
    strcpy(destination->name, old_arr->name);
    destination->numb_of_links = old_arr->numb_of_links;
}

int checkMemoryManagerAvilability(int new_elem)
{
    return ((NUM_OBJ_MAX) && (NUM_OBJ_CURR + new_elem <= NUM_OBJ_MAX));
}

// заменить
void good_realloc(int new_size, int old_size)
{
    if (!old_size)
    {
        // Если создается первый элемент
        ARR = (element*)malloc(sizeof(element));
        return;
    }
    // выделять в 2 раза больше памяти (с запасом) для высокой скорости
    // realloc на меньшее количество замедляет программу.
    element* new_arr = (element*)malloc(sizeof(element) * new_size);
    for (int i = 0; i < old_size; i++)
    {
        copy_plus_vibe(&new_arr[i], &ARR[i]);
    }
    free(ARR);
    ARR = new_arr;
}

void sort_strings_in_element(element* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (strcmp(arr[i].name, arr[j].name) > 0) {
                // Если текущая строка больше следующей, меняем их местами
                element temp;
                copy_plus_vibe(&temp, &arr[i]);
                copy_plus_vibe(&arr[i], &arr[j]);
                copy_plus_vibe(&arr[j], &temp);
            }
        }
    }
}

int find_element(element* arr, const char* name, int arr_size)
{
    int i = 0;
    for (; i < arr_size; i++)
    {
        if (!strcmp(arr[i].name, name))
        {
            return i;
        }
    }
    return -1;
}

void destroy_elem_and_shift_array(int index, int size)
{
    int shift = 0;
    int new_size = size - 1;
    // подправить.
    element* new_arr = (element*)malloc(sizeof(element) * (new_size));
    if (ARR[index].numb_of_links != 0)
    {
        delete_all_links_to_obj_from_each_of_element_arr(ARR[index].name, 16);
    }
    // Удаляем все ссылки
    for (int j = 0; j < 16; j++)
    {
        if (ARR[index].links[j] != NULL)
        {
            ARR[index].links[j]->numb_of_links -= 1;
            ARR[index].links[j] = NULL;
        }
        else
        {
            break;
        }
    }
    for (int i = 0; i < size; i++)
    {
        if (i == index)
        {
            // нашли удаляемый элемент
            shift++;
            continue;
        }
        copy_plus_vibe(&new_arr[i - shift], &ARR[i]);
        // 0-0 1-1 2-2 3-4 4-5
    }
    free(ARR);
    if (new_size > 0)
    {
        ARR = (element*)malloc(sizeof(element) * (new_size));
        if (ARR)
        {
            for (int i = 0; i < new_size; i++)
            {
                copy_plus_vibe(&ARR[i], &new_arr[i]);
            }
        }
    }
    free(new_arr);
}

// ************************************

// Работа моих функций
int my_create(int size, int num_pages)
{
    if (NUM_OBJ_MAX) return 0;
    NUM_OBJ_MAX = size;
    return 1;
}

int my_destroy()
{
    if (!checkMemoryManagerAvilability(0)) return 0;
    NUM_OBJ_MAX = 0;
    if (NUM_OBJ_CURR == 0) return 1;
    NUM_OBJ_CURR = 0;
    free(ARR);
    return 1;
}

int my_create_object(const char* name)
{
    if (!checkMemoryManagerAvilability(1)) return 0;
    if (find_element(ARR, name, NUM_OBJ_CURR) != -1) return 0;
    int new_size = NUM_OBJ_CURR + 1;
    good_realloc(new_size, NUM_OBJ_CURR);
    element new_element;
    strcpy(new_element.name, name);
    new_element.numb_of_links = 0;
    for (int i = 0; i < 16; i++)
    {
        new_element.links[i] = NULL;
    }
    copy_plus_vibe(&ARR[new_size - 1], &new_element);
    NUM_OBJ_CURR++;
    return 1;
}

int my_destroy_object(const char* name)
{
    if (!checkMemoryManagerAvilability(0)) return 0;
    int elem_index = find_element(ARR, name, NUM_OBJ_CURR);
    if (elem_index == -1) return 0;
    destroy_elem_and_shift_array(elem_index, NUM_OBJ_CURR);
    NUM_OBJ_CURR--;
    return 1;
}

int my_link(const char* object1_name, const char* object2_name)
{
    if (!checkMemoryManagerAvilability(0)) return 0;
    int parent_index = find_element(ARR, object1_name, NUM_OBJ_CURR);
    int child_index = find_element(ARR, object2_name, NUM_OBJ_CURR);
    if ((parent_index == -1) || (child_index == -1)) return 0;
    element* link = ARR[parent_index].links[0]; // Начнем с первого элемента
    int j = 0;
    while (link)
    {
        j++;
        if (j >= 16)
        {
            // В массиве links не осталось доступных мест
            break;
        }
        link = ARR[parent_index].links[j];
    }
    if (j < 16)
    {
        ARR[parent_index].links[j] = &ARR[child_index]; // Установить на ARR[child_index]
    }
    ARR[child_index].numb_of_links++;
    return 1;
}

void my_print_objects()
{
    // Создаем копию массива для сортировки
    element* sorted_arr = (element*)malloc(sizeof(element) * NUM_OBJ_CURR);
    // Копируем содержимое arr в sorted_arr
    for (int i = 0; i < NUM_OBJ_CURR; i++)
    {
        copy_plus_vibe(&sorted_arr[i], &ARR[i]);
    }
    // Сортируем sorted_arr по имени
    sort_strings_in_element(sorted_arr, NUM_OBJ_CURR);

    // Выводим отсортированные элементы
    for (int i = 0; i < NUM_OBJ_CURR; i++)
    {
        printf("%s\n", sorted_arr[i].name);
    }

    // Освобождаем память, выделенную для sorted_arr
    free(sorted_arr);

}

void print_link_counts()
{
    // Создаем копию массива для сортировки
    element* sorted_arr = (element*)malloc(sizeof(element) * NUM_OBJ_CURR);

    // Копируем содержимое arr в sorted_arr
    for (int i = 0; i < NUM_OBJ_CURR; i++)
    {
        copy_plus_vibe(&sorted_arr[i], &ARR[i]);
    }

    // Сортируем sorted_arr по имени
    sort_strings_in_element(sorted_arr, NUM_OBJ_CURR);

    // Выводим отсортированные элементы
    for (int i = 0; i < NUM_OBJ_CURR; i++)
    {
        printf("%s %d\n", sorted_arr[i].name, sorted_arr[i].numb_of_links);
    }

    // Освобождаем память, выделенную для sorted_arr
    free(sorted_arr);
}
// ************************************