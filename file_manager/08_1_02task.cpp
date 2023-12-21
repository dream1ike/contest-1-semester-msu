#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "os_file.h"
#pragma warning(disable:4996)

// g++ -x c++ -std=c++11 *main* *task* -fsanitize=address -o out
// DANGER! - места, где возможны утечки памяти

// Константы
#define MAX_DIR_COUNT 128
#define PATH_LENGHT 128
#define NAME_SIZE 32
#define ERROR_CODE -1


// Основная структура элемента
typedef struct element
{
    char* name;
    int size;
    int isfile;
    int number_of_kids;
    element** kids;
    element* parent;
}element;

// Структура Файлового менеджера

typedef struct FileManager
{
    int max_size;
    int curr_size;
    element* root;
    element* current_root;
}FileManager;

FileManager MANAGER;
// Вспомогательные функции
int IsNameCorrect(char* name);

void add_kid_to_parent(element* parent, element* new_element);

element* create_element(const char* name, int size, int isfile, element* parent);

int containsAsterisk(const char* str);

int match(const char* pattern, const char* string);

element* find_element(const char* name, element* parent_path);

element* go_to_path(const char* path);

char* extract_last_name_from_path(const char* path);

void remove_element(element* need_to_remove);

int remove_kid_element(element* parent_dir, char* remove_elem_name);

void remove_recursive(element* destroyDir);

// Функция для обмена указателями на элементы
void swap(element** a, element** b);

// Функция для сравнения элементов по полю name
int compareElements(const element* a, const element* b);

// Функция для сортировки пузырьком
void bubbleSort(element** arr, int size);

char** get_all_names_from_dir(element* parent_path, int dir_first);

element* filter_elemets(element* parent_path, char* name);

// Основные функции

int my_create(int disk_size);

int my_destroy();
//
int my_create_dir(const char* path);
//
int my_create_file(const char* path, int size);
//
int my_remove(const char* path, int recursive);
//
int my_change_dir(const char* path);
//
void my_get_curr_dir(char* dst);
//
int my_list(const char* path, int dir_first);

// Сетап
void setup_file_manager(file_manager_t* fm)
{
    MANAGER.max_size = ERROR_CODE;
    MANAGER.curr_size = ERROR_CODE;
    MANAGER.root = NULL;
    MANAGER.current_root = NULL;

    fm->create = my_create;
    fm->destroy = my_destroy;
    fm->create_dir = my_create_dir;
    fm->create_file = my_create_file;
    fm->remove = my_remove;
    fm->change_dir = my_change_dir;
    fm->get_cur_dir = my_get_curr_dir;
    fm->list = my_list;
}

// реализация вспомогательных функций
int IsNameCorrect(char* name)
{
    if (name == NULL || name[0] == '\0') {
        // Проверка на нулевой указатель или пустую строку
        return 0;
    }

    // Проверка, начинается ли имя с "." или ".."
    if (name[0] == '.' && (name[1] == '\0' || (name[1] == '.' && name[2] == '\0'))) {
        return 0;  // Имя начинается с "." или ".."
    }
    else {
        return 1;  // Имя не начинается с "." или ".."
    }
}

int checkFileManagerAvilability(int size_of_new_element)
{
    if (MANAGER.curr_size + size_of_new_element <= MANAGER.max_size && MANAGER.max_size != -1) return 1;
    return ERROR_CODE;
}

int containsAsterisk(const char* str) 
{
    while (*str != '\0') {
        if (*str == '*') {
            return 1; // Символ '*' найден
        }
        str++;
    }
    return 0; // Символ '*' не найден
}

element* find_element(const char* name, element* parent_path)
{
    if (!strcmp(".", name))
    {
        return parent_path;
    }
    if (!strcmp("..", name))
    {
        return parent_path->parent;
    }
    int kids_size = parent_path->number_of_kids;
    element** current_dir_kids = parent_path->kids;
    for (int i = 0; i < kids_size; i++)
    {
        if (!strcmp(name, current_dir_kids[i]->name))
        {
            return current_dir_kids[i];
        }
    }
    return NULL;
}

char** getDirectories(char* path, size_t* count)
{
    // Инициализируем strtok с разделителем "/"
    char* token = strtok(path, "/");
    
    // Создаем массив для хранения имен директорий
    char** directories = (char**)malloc(MAX_DIR_COUNT * sizeof(char*));
    *count = 0;

    // Перебираем подстроки и сохраняем их в массив
    while (token != NULL && *count < MAX_DIR_COUNT) {
        directories[*count] = strdup(token);
        (*count)++;

        // Получаем следующую подстроку
        token = strtok(NULL, "/");
    }
    free(token);
    return directories;
}

element* go_to_path(const char* path)
{
    // "/dir1/dir2/dir3/./../file1"
    // "dir1/dir2/dir3/./././.."
    
    
    // Прорабатываем ошибки типа пустой строки на вход
    if (path[0] == '\0') return NULL;
    
    // Определяем, абсолютный ли путь
    element* current_dir;
    if (path[0] == '/' && strlen(path) == 1) return MANAGER.root;
    int isAbsolutePath = (path[0] == '/');
    if (isAbsolutePath) current_dir = MANAGER.root;
    else current_dir = MANAGER.current_root;
    if ((!current_dir->number_of_kids) && (strlen(path) > 2) && (path[0] != '.' && path[1] != '.')) return current_dir;

    // Выделяем память под копию пути (нужно для strtok)
    char* copy = strdup(path);
    
    // количество имен в пути
    size_t count = 0;
    
    // массив для хранения имен директорий
    char** directories = getDirectories(copy, &count);
    if (directories[0] == NULL) return current_dir;
    int i = 0;
    // Обрабатываем все имена директорий, кроме последней
    for (; i < count - 1; ++i)
    {
        // Ситуации с точками
        if (!strcmp(directories[i], "."))
        {
            free(directories[i]);
            continue;
        }
        if (!strcmp(directories[i], ".."))
        {
            if (current_dir->parent == NULL)
            {
                current_dir = NULL;
                break;
            }
            current_dir = current_dir->parent;
            free(directories[i]);
            continue;
        }

        // Проход по директориям
        if (!current_dir->number_of_kids)
        {
            if (i != count - 1) current_dir = NULL;
            break;
        }
        else
        {
            // если найденное имя не является директорией
            if (current_dir->isfile)
            {
                current_dir = NULL;
                break;
            }
            // если у найденной директории дети есть и все окей
            current_dir = find_element(directories[i], current_dir);
            
            if (current_dir == NULL)
            {
                break;
            }
            else
            {
                if (current_dir->isfile)
                {
                    current_dir = NULL;
                    break;
                }
            }
        }
        free(directories[i]);
    }
    while (i < count)
    {
        free(directories[i]);
        i++;
    }
    free(directories);
    free(copy);
    return current_dir;

}

void add_kid_to_parent(element* parent, element* new_element)
{
    if (parent == NULL) return;
    element** new_parent_kids = (element**)malloc(sizeof(element*) * (parent->number_of_kids + 1));
    for (int i = 0; i < parent->number_of_kids; i++)
    {
        new_parent_kids[i] = parent->kids[i];
    }
    // DANGER!
    new_parent_kids[parent->number_of_kids] = new_element;
    if (parent->kids != NULL)
    {
        free(parent->kids);
    }
    parent->kids = new_parent_kids;
    parent->number_of_kids++;
    return;
}

element* create_element(const char* name, int size, int isfile, element* parent)
{
    element* new_element = (element*)malloc(sizeof(element));
    if (!new_element) return NULL;
    new_element->name = strdup(name);
    new_element->size = size;
    new_element->isfile = isfile;
    new_element->kids = NULL;
    new_element->parent = parent;
    new_element->number_of_kids = 0;
    add_kid_to_parent(parent, new_element);
    return new_element;
}

char* extract_last_name_from_path(const char* path) {
    char* copy = strdup(path);
    if (strcmp(copy, "/") == 0) return copy;
    char* token = strtok(copy, "/");

    char* lastDirectory = NULL;  // Динамический массив для имени

    while (token != NULL) {
        if (lastDirectory != NULL) 
        {
            // Увеличиваем размер динамического массива на длину текущего токена и дополнительный символ '\0'
            lastDirectory = (char*)realloc(lastDirectory, strlen(token) + 1);
        } else
        {
            // Выделяем память для первого токена
            lastDirectory = (char*)malloc(strlen(token) + 1);
        }

        // Копируем токен в динамический массив
        strcpy(lastDirectory, token);

        token = strtok(NULL, "/");
    }

    free(copy);
    // Возвращаем указатель на динамический массив
    return lastDirectory;
}

// Функция для объединения массива строк в одну строку с разделителями
void joinStringsReverseWithRoot(char** strings, size_t count, const char* delimiter, char* dst) {
    size_t totalLength = 1; // Начальная длина для слеша в корне

    // Вычисляем общую длину строки
    for (size_t i = 0; i < count; ++i) {
        totalLength += strlen(strings[i]);
    }

    // Учитываем разделители
    totalLength += (count - 1) * strlen(delimiter);

    // Начинаем с корня
    strcpy(dst, "/");

    // Копируем каждую строку в результат в обратном порядке, добавляя разделители
    for (size_t i = count; i > 0; --i) {
        strcat(dst, strings[i - 1]);

        // Добавляем разделитель, если это не последний элемент
        if (i > 1) {
            strcat(dst, delimiter);
        }
    }
}

void remove_element(element* need_to_remove)
{
    if (need_to_remove->isfile) MANAGER.curr_size -= need_to_remove->size;
    if (MANAGER.current_root == need_to_remove) MANAGER.current_root = MANAGER.root;
    free(need_to_remove->kids);
    free(need_to_remove->name);
    if (need_to_remove->parent != NULL) need_to_remove->parent->number_of_kids--;
    free(need_to_remove);
}

int remove_kid_element(element* parent_dir, char* remove_elem_name) 
{
    if (parent_dir == NULL && !strcmp(remove_elem_name, "/"))
    {
        remove_element(MANAGER.root);
        return 1;
    }
    if (parent_dir->number_of_kids == 0) {
        // Если массив пуст, ничего делать не нужно
        return 0;
    }
    element** kids = parent_dir->kids;
    int size = parent_dir->number_of_kids;
    // Находим индекс элемента, который нужно удалить

    int indexOfElement = -1;
    for (int i = 0; i < size; ++i) {
        if (!strcmp(kids[i]->name, remove_elem_name)) 
        {
            indexOfElement = i;
            break;
        }
    }

    if (indexOfElement == -1) {
        // Если элемент не найден, ничего делать не нужно
        return 0;
    }

    // Если у нас один ребенок
    if (size - 1 == 0)
    {
        remove_element(parent_dir->kids[indexOfElement]);
        return 1;
    }
    // Создаем новый массив
    element** new_kids = (element**)malloc(sizeof(element*) * ((size) - 1));
    // копируем все, кроме последнего элемента
    int shift = 0;
    for (int i = 0; i < (size) - shift; i++)
    {
        if (i == indexOfElement)
        {
            shift++;
            remove_element(kids[i]);
        }
        if (i < (size)-shift)
        {
            new_kids[i] = kids[i + shift];
        }
    }
    free(parent_dir->kids);
    parent_dir->kids = new_kids;
    return 1;
}

void remove_recursive(element* node)
{
    if (node->number_of_kids == 0)
    {
        remove_kid_element(node->parent, node->name);
    }
    else
    {
        for (; node->number_of_kids != 0;)
        {
            if (node->kids[0]->isfile)
            {
                remove_kid_element(node, node->kids[0]->name);
            }
            if (node->number_of_kids && !node->kids[0]->isfile)
            {
                remove_recursive(node->kids[0]);
            }
        }
        remove_kid_element(node->parent, node->name);
    }
}

int match(const char* pattern, const char* string)
{
    while (*pattern != '\0' && *string != '\0') {
        if (*pattern == '*') {
            // Обработка звездочки
            while (*pattern == '*') {
                pattern++;
            }

            if (*pattern == '\0') {
                // Звездочка в конце шаблона - считаем, что она соответствует остатку строки
                return 1;
            }

            while (*string != '\0' && *string != *pattern) {
                string++;
            }
            // Если дошли до конца строки, но символы не совпали
            if (*string == '\0' && *pattern != '\0')
            {
                return 0;
            }
        }
        else if (*pattern == *string) {
            pattern++;
            string++;
        }
        else {
            // Если символы не совпадают
            return 0;
        }
    }
    return 1;
}

// Функция для обмена указателями на элементы
void swap(element** a, element** b)
{
    element* temp = *a;
    *a = *b;
    *b = temp;
}

// Функция для сравнения элементов по полю name
int compareElements(const element* a, const element* b)
{
    return strcmp(a->name, b->name);
}

// Функция для сортировки пузырьком
void bubbleSort(element** arr, int size)
{
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
            arr_index++;
        }
        free(arr_dirs);
        free(arr_files);

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

element* filter_elemets(element* parent_path, char* name)
{
    element* filtered_parent_path = (element*)malloc(sizeof(element));
    filtered_parent_path->isfile = 0;
    filtered_parent_path->parent = NULL;
    filtered_parent_path->size = 0;
    filtered_parent_path->number_of_kids = 0;
    filtered_parent_path->kids = (element**)malloc(sizeof(element*) * parent_path->number_of_kids);
    int filtered_index = 0;
    for (int i = 0; i < parent_path->number_of_kids; i++)
    {
        if (match(name, parent_path->kids[i]->name))
        {
            filtered_parent_path->kids[filtered_index] = parent_path->kids[i];
            filtered_index++;
        }
    }
    // Если не нашлось по данному шаблону
    if (!filtered_index)
    {
        free(filtered_parent_path->kids);
        return NULL;
    }
    filtered_parent_path->number_of_kids = filtered_index;
    filtered_index = 0;
    if (filtered_parent_path->number_of_kids < parent_path->number_of_kids)
    {
        filtered_parent_path->kids = (element**)realloc(filtered_parent_path->kids, 
                                    sizeof(element*) * filtered_parent_path->number_of_kids);
    }
    return filtered_parent_path;
}
// реализация основных функций
int my_create(int disk_size)
{
    // Если уже создан fm
    if (!(MANAGER.max_size == ERROR_CODE)) return 0;
    if (disk_size < 0) return 0;
    MANAGER.max_size = disk_size;
    MANAGER.curr_size = 0;
    MANAGER.root = create_element("/", 0, 0, NULL);
    MANAGER.current_root = MANAGER.root;
    return 1;
}

int my_create_dir(const char* path)
{
    // Если не создан fm
    if (checkFileManagerAvilability(0) == ERROR_CODE) return 0;
    element* parentPath = go_to_path(path);
    if (parentPath == NULL) return 0;
    char* name = extract_last_name_from_path(path);
    if (!IsNameCorrect(name))
    {
        free(name);
        return 0;
    }
    element* elem = find_element(name, parentPath);
    // если такой элемент уже есть
    if (elem)
    {
        free(name);
        return 0;
    }
    create_element(name, 0, 0, parentPath);
    free(name);
    return 1;
}

int my_create_file(const char* path, int size)
{
    if (checkFileManagerAvilability(size) == ERROR_CODE) return 0;
    element* parentPath = go_to_path(path);
    if (parentPath == NULL) return 0;
    char* name = extract_last_name_from_path(path);
    if (!IsNameCorrect(name))
    {
        free(name);
        return 0;
    }
    element* elem = find_element(name, parentPath);
    // если такой элемент уже есть
    if (elem)
    {
        free(name);
        return 0;
    }
    create_element(name, size, 1, parentPath);
    MANAGER.curr_size += size;
    free(name);
    return 1;
}

int my_change_dir(const char* path)
{
    if (checkFileManagerAvilability(0) == ERROR_CODE) return 0;
    element* parentPath = go_to_path(path);
    if (parentPath == NULL) return 0;
    // if (!parentPath->number_of_kids) return 0;
    char* name = extract_last_name_from_path(path);
    parentPath = find_element(name, parentPath);
    if (parentPath == NULL)
    {
        free(name);
        return 0;
    }
    MANAGER.current_root = parentPath;
    free(name);
    return 1;
}

void my_get_curr_dir(char* dst)
{
    // Проверки
    if (checkFileManagerAvilability(0) == ERROR_CODE) return;
    
    element* curr_dir = MANAGER.current_root;
    
    // Если мы сразу находимся в корне
    if (!strcmp("/", curr_dir->name))
    {
        *dst = '/';
        return;
    }
    
    // чтоб постоянно не использовать realloc
    char** all_folders = (char**)malloc(sizeof(char*) * MAX_DIR_COUNT);
    size_t count = 0;
    // Создать массив всех имен перемещаясь по parent, пока не встречу корень
    while (strcmp("/", curr_dir->name) && count < MAX_DIR_COUNT)
    {
        all_folders[count] = strdup(curr_dir->name);
        curr_dir = curr_dir->parent;
        count++;
    }
    // Проходиться с конца массива до начала, объединяя соединяя все строки /
    joinStringsReverseWithRoot(all_folders, count, "/", dst);
    
    // Очищение памяти
    for (int i = 0; i < count; i++)
    {
        free(all_folders[i]);
    }
    free(all_folders);
    return;
}

int my_destroy()
{
    if (checkFileManagerAvilability(0) == ERROR_CODE) return 0;
    if (!my_remove("/", 1)) return 0;
    MANAGER.max_size = ERROR_CODE;
    MANAGER.curr_size = ERROR_CODE;
    MANAGER.root = NULL;
    MANAGER.current_root = NULL;
    return 1;
}

int my_remove(const char* path, int recursive)
{
    int status_code = 1;
    // Проверки
    // 0. проверка файлового
    // 1. проверить, если у элемента есть дети, вернуть 0
    // 2. если элемент это файл - отнять от текущего размера размер файла
    // алгоритм работы
    // проверяем файловый
    if (checkFileManagerAvilability(0) == ERROR_CODE && status_code) status_code = 0;

    // переходим по пути к родителю (проверка на корректность пути)
    element* parentDir = go_to_path(path);
    if (parentDir == NULL && status_code) status_code = 0;

    // если у родителя нет детей + последнее имя = имя родителя (когда нам подается root или что-то подобное) ->
    // -> очищаем элемент, возвращаем 1
     char* name = extract_last_name_from_path(path);
    if (parentDir->number_of_kids == 0 && strcmp(parentDir->name, name) == 0 && status_code)
    {
        remove_element(parentDir);
        free(name);
        return 1;
    }
    element* need_to_remove = NULL;
    if (!strcmp(parentDir->name, name))
    {
        need_to_remove = parentDir;
    }
    else
    {
        need_to_remove = find_element(name, parentDir);
        // Если не нашли нужный элемент
        if (need_to_remove == NULL)
        {
            free(name);
            return 0;
        }
    }
    // если у элемента есть дети и recursive != 0, вернуть 0
    if (need_to_remove->number_of_kids != 0 && recursive == 0 && status_code) status_code = 0;
    // 
    // 
    // 
    // если все окей
    if (!recursive && status_code)
    {
        if (!remove_kid_element(parentDir, name)) status_code = 0;
    }
    else if (recursive && status_code)
    {
        remove_recursive(need_to_remove);
    }
    free(name);
    return status_code;
}

int my_list(const char* path, int dir_first)
{
    if (checkFileManagerAvilability(0) == ERROR_CODE) return 0;
    element* parent_path = go_to_path(path);
    if (parent_path == NULL) return 0;
    char* name = extract_last_name_from_path(path);
    // Если нет *
    if (!containsAsterisk(name))
    {
        element* last_elem = find_element(name, parent_path);
        // Если элемент не найден
        if (last_elem == NULL)
        {
            free(name);
            return 0;
        }

        
        // Если элемент файл - выводим просто path
        if (last_elem->isfile)
        {
            printf("%s", path);
            free(name);
            printf("\n");
            return 1;
        }

        // Если элемент - директория
        printf("%s:\n", path);
        char** names = get_all_names_from_dir(last_elem, dir_first);
        for (int i = 0; i < last_elem->number_of_kids; i++)
        {
            printf("%s\n", names[i]);
            free(names[i]);
        }
        free(names);
    }
    else
    {
        element* filtered_parent_path = filter_elemets(parent_path, name);
        if (filtered_parent_path == NULL)
        {
            free(name);
            return 0;
        }
        printf("%s:\n", path);
        char** names = get_all_names_from_dir(filtered_parent_path, dir_first);
        for (int i = 0; i < filtered_parent_path->number_of_kids; i++)
        {
            printf("%s ", names[i]);
            free(names[i]);
        }
        free(filtered_parent_path->kids);
        free(filtered_parent_path);
        free(names);
    }
    free(name);
    printf("\n");
    return 1;

}
