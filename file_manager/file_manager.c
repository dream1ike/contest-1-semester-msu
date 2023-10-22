#include <stdio.h>
#include "os_file.h"

// Структура Элемента
typedef struct
{
    char* name;
    int size;
    char isfile;
    int number_of_kids;
    element** kids;
    element* parent;

} element;

// Структура Файлового менеджера
typedef struct{
    element* root;      // Корневая директория
    int size;
    element* current_dir;   // Текущая директория
    int current_size;
} FileManager;

FileManager MANAGER;

// Initializing supporting functions
int is_valid_path(char* path); // returns 0 if not ok, 1 if ok

int is_path_absolute(char* path); // (helped go_to_path)

char** parser(char* path, int number_of_slashes) // returns arr with dir names (helped go_to_path)

element* create_element(char* name, int size, int isfile, element* parent);

element* find_elem(char** directoryNames, element* root) // returns elem, that we trying to find (unsafe) (helped go_to_path)

element* go_to_path(char* path); // main func, returns elem in path (second last), that we trying to find (safe)

// ---------------------------------------------

// Initializing main functions
int my_create_dir(const char* path);

int my_create(int disk_size);

void my_get_cur_dir(char* dst);

void setup_file_manager(file_manager_t *fm);
// ---------------------------------------------

// Main
int main()
{
    file_manager_t fm;

    setup_file_manager(&fm);
    int result = fm.create(10);
    result = fm.destroy();
    return 0;
}
// ---------------------------------------------

// Setup func
void setup_file_manager(file_manager_t *fm) {
    fm->create = my_create;
    fm->destroy = my_destroy;
    fm->get_cur_dir = my_get_cur_dir;
    fm->create_dir = my_create_dir;
}
// ---------------------------------------------

// Supporting func
int is_valid_path(char* path) // worked!
{
    int last_slash_index = -2; // -2 чтобы уж точно не было случайной разницы между индексами
    if (strlen(path) == 0) return 0;
    int count_slashes = 0;
    for(int i = 0; i < strlen(path); i++)
    {
        // проверка, состоит ли пусть только из цифр, букв, / _ .
        // '.'       46
        // '/'       47
        // '0' - '9' 48 - 57
        // 'A' - 'Z' 65 - 90 
        // '_'       95
        // 'a' - 'z' 97 - 122
        if ((path[i] < 46) || ((57 < path[i]) && (path[i] < 65)) || ((90 < path[i]) && (path[i] < 95)) 
         || (path[i] > 122) && (path[i] != 96)) return 0;

        // проверка на правильность /
        if (path[i] == '/') 
        {
            count_slashes++;
            // если встретятся два слеша подряд
            if (abs(last_slash_index - i) == 1) return 0;
            last_slash_index = i;
        }
        // проверка на правильность .. и .
        if (path[i] == '.')
        {
            int j = i+1;
            int counter = 1;
            for(; j < strlen(path) && path[j] == '.'; j++) counter++;
            if (counter > 2) return 0;
        }
    }
    // Обработка случая "/dir1/dir2/", когда есть слеш в конце
    if (path[strlen(path)-1] == '/') count_slashes--;
    return count_slashes;
}

int is_path_absolute(char* path)
{
    return path[0] == '/';
}

char* get_first_dirname_from_path(char* path, int* t) // t - место, откуда нужно начать парсить (после слеша)
{
    char name_dir_static[33];
    int size_of_name_dir = 0;
    // Копируем имя во временный массив
    while ((path[*t] != '/') && (*t < strlen(path)))
    {
        name_dir_static[size_of_name_dir] = path[*t];
        size_of_name_dir++;
        *t += 1;
    }
    // Пропускаем слеш
    *t += 1;
    char* dirName = (char*)malloc(sizeof(char) * size_of_name_dir);
    for (int i = 0; i < size_of_name_dir; i++)
    {
        dirName[i] = name_dir_static[i];
    }
    return dirName;
    
}

char** parser(char* path, int number_of_slashes, char** last_dir_name)
{
    char** directoryNames = (char**)malloc(sizeof(char*) * number_of_slashes);
    int i = 0;
    if (path[0] == '/')
    {
        directoryNames[0] = (char*)malloc(sizeof(char));
        directoryNames[0][0] = '/';
        i++;
    }
    // Временная переменная, которая нужна для вложенного while
    int t = i;
    // i < number_of_slashes потому, что мы специально не считываем последнюю директорию
    // это нужно для create_dir
    while (i < number_of_slashes)
    {
        directoryNames[i] = get_first_dirname_from_path(path, &t);
        i++;
    }
    // имя последней директории (нужно для create_dir)
    *last_dir_name = get_first_dirname_from_path(path, &t);
    return directoryNames;
}

element* find_elem(char** directoryNames, element* root)
{
    // Возможно рекурсивная функция. Подумай
}

element* go_to_path(char* path, char** last_dir_name) // last_dir_name нужен для create_dir
{
    element* root = MANAGER.current_dir;
    int number_of_slashes = is_valid_path(path);
    if (number_of_slashes)
    {
        if (is_path_absolute(path))
        {
            root = MANAGER.root;
            number_of_slashes--;
        }
        char** directoryNames = parser(path, number_of_slashes, last_dir_name);
        if (!directoryNames) return 0;
        return find_elem(directoryNames, root);
    }
    else
    {
        return 0;
    }
}

element* create_element(char* name, int size, int isfile, element* parent)
{
    element* elem = (element*)malloc(sizeof(element));
    elem->name = name;
    elem->size = size;
    elem->isfile = isfile;
    elem->number_of_kids = 0;
    elem->kids = NULL;
    elem->parent = parent;
    return elem;
}
// ---------------------------------------------

// My func
int my_create_dir(const char* path)
{
    char* last_dir_name;
    element* curr_dir = go_to_path(path, &last_dir_name);
    curr_dir->number_of_kids++;
    element* new_dir = (element*)malloc(sizeof(element));

}

int my_create(int disk_size)
{
    // Проверка, если менеджер уже создавался
    if (MANAGER.root) return 0;
    MANAGER.size = disk_size;
    element* root = create_element("/", 0, 0, NULL);
    MANAGER.root = root;
    MANAGER.current_dir = root;
    return 1;
}

//************************************************

int my_destroy()
{
    return 1;
}

void my_get_cur_dir(char* dst) {
    strcpy(dst, "test");
}
