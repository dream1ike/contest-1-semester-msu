#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#pragma warning(disable:4996)
#define MANAGER_SIZE
// Структура Элемента
typedef struct element
{
    char* name;
    int size;
    char isfile;
    int number_of_kids;
    struct element** kids;
    struct element* parent;

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

int free_two_demension_char_arr(char** arr);

void free_elements_array(element** arr, int number_of_kids); // free all dynamic memory

element* create_element(char name[], int size, int isfile, element* parent); // (checked) creating element

int checkFileManagerAvailability(int file_size);

int is_valid_path(const char* path); // (checked) returns 0 if not ok, 1 if ok 

int is_path_absolute(const char* path);  // (checked) (helped go_to_path) 

char* get_first_dirname_from_path(const char* path, int* t); // (checked) returns arr with dir names (helped go_to_path) 

element* find_elem(char** directoryNames, int size_directoryNames, element* root); // returns elem, that we trying to find (unsafe) (helped go_to_path) 

element* go_to_path(const char* path, char** last_dir_name); // (checked) main func, returns elem in path (second last), that we trying to find (safe) 

void add_kid_to_kids(element* parent, element* kid); // (checked) 

void delete_element(element* node);

void remove_recursive(element* node);

char** secure_char_array_expansion(char** arr, int curr_size, int necessary_size);

char* create_path(element* curr_dir);

// ---------------------------------------------

// Initializing main functions
int my_create(int disk_size);

int my_destroy();

int my_create_dir(const char* path);

int my_create_file(const char* path, int file_size);

int my_remove(const char* path, int recursive);

int my_get_cur_dir(char* dst);

void setup_file_manager(file_manager_t *fm);
// ---------------------------------------------

// Main
int main()
{
    MANAGER.current_dir = NULL;
    MANAGER.current_size = 0;
    MANAGER.root = 0;
    MANAGER.size = 0;
    int result = 0;
    result = my_create(100);
    //_CrtDumpMemoryLeaks();
    result = my_create_dir("/dir1");
    //_CrtDumpMemoryLeaks();
    result = my_create_file("/file1.txt", 10);
    //_CrtDumpMemoryLeaks();
    result = my_create_file("/file2.txt", 90);
    _CrtDumpMemoryLeaks();
    result = my_create_file("/file3.txt", 90);
    //_CrtDumpMemoryLeaks();
    char* dst = NULL;
    my_get_cur_dir(dst);
    my_destroy();
    return 0;
}
// ---------------------------------------------

// Setup func
void setup_file_manager(file_manager_t *fm) {
    MANAGER.current_dir = NULL;
    MANAGER.current_size = 0;
    MANAGER.root = 0;
    MANAGER.size = 0;
    fm->create = my_create;
    fm->destroy = my_destroy;
    fm->get_cur_dir = my_get_cur_dir;
    fm->create_dir = my_create_dir;
}
// ---------------------------------------------

// Supporting func

int free_two_demension_char_arr(char** arr)
{
    int size = sizeof(arr) / sizeof(char*);
    for (int i = 0; i < size; i++)
    {
        free(arr[i]);
    }
    free(arr);
    return 1;
}

int checkFileManagerAvailability(int file_size)
{
    return !((MANAGER.size < MANAGER.current_size + file_size) && MANAGER.root);
}

int is_valid_path(const char* path) // worked!
{
    int last_slash_index = -2; // -2 чтобы уж точно не было случайной разницы между индексами
    if (strlen(path) == 0) return 0;
    int count_slashes = 0;
    for (int i = 0; i < strlen(path); i++)
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
            // если встрет€тс€ два слеша подр€д
            if (abs(last_slash_index - i) == 1) return 0;
            last_slash_index = i;
        }
        // проверка на правильность .. и .
        if (path[i] == '.')
        {
            int j = i + 1;
            int counter = 1;
            for (; j < strlen(path) && path[j] == '.'; j++) counter++;
            if (counter > 2) return 0;
        }
    }
    // ќбработка случа€ "/dir1/dir2/", когда есть слеш в конце
    if (path[strlen(path) - 1] == '/') count_slashes--;
    return count_slashes;
}

int is_path_absolute(const char* path) // checked
{
    return path[0] == '/';
}

char* get_first_dirname_from_path(const char* path, int* t) // t - место, откуда нужно начать парсить (после слеша) (checked)
{
    char name_dir_static[33];
    int size_of_name_dir = 0;
    //  опируем им€ во временный массив
    while (((path[*t] != '/') || (*t == 0)) && (*t < strlen(path)))
    {
        name_dir_static[size_of_name_dir] = path[*t];
        size_of_name_dir++;
        if ((*t == 0) && (path[*t] == '/')) break;
        *t += 1;
    }
    // ѕропускаем слеш
    *t += 1;
    int size_dirName = size_of_name_dir + 1;
    name_dir_static[size_of_name_dir] = '\0';
    
    char* dirName = (char*)malloc(sizeof(char) * (size_dirName));
    strcpy(dirName, name_dir_static);
    /*for (int i = 0; i < size_of_name_dir; i++)
    {
        dirName[i] = name_dir_static[i];
    }*/
    // dirName[size_of_name_dir] = '\0';
    return dirName;

}

char** parser(const char* path, int number_of_slashes, char** last_dir_name) // checked
{
    char** directoryNames = (char**)malloc(sizeof(char*) * number_of_slashes);
    int i = 0;
    // t отвечает за перемещение по строке, которую хотим запарсить
    int t = i;

    // i < number_of_slashes-1 потому, что мы специально не считываем последнюю директорию
    // это нужно дл€ create_dir
    while (i < number_of_slashes)
    {
        directoryNames[i] = get_first_dirname_from_path(path, &t);
        i++;
    }
    // им€ последней директории (нужно дл€ create_dir)
    *last_dir_name = get_first_dirname_from_path(path, &t);
    return directoryNames;
}

element* find_elem(char** directoryNames, int size_directoryNames, element* root)
{
    int result = 1;
    element* result_directory = root;
    for (int i = 0; i < size_directoryNames; i++)
    {
        if (!strcmp(directoryNames[i], "/"))
        {
            return MANAGER.root;
        }

        if (!strcmp(directoryNames[i], ".")) continue;

        if (!strcmp(directoryNames[i], ".."))
        {
            result_directory = result_directory->parent;
        }
        for (int j = 0; j < result_directory->number_of_kids; j++)
        {
            element* kid = result_directory->kids[j];
            if (!strcmp(directoryNames[i], kid->name))
            {
                // Если нашли директорию с таким именем
                result_directory = kid;
                break;
            }
        }
        // ≈сли не нашли нужную директорию
        if (strcmp(directoryNames[i], result_directory->name)) return NULL;
        //free_two_demension_char_arr(directoryNames);
        //return result;
    }
    return result_directory;
}

element* go_to_path(const char* path, char** last_dir_name) // last_dir_name нужен дл€ create_dir
{
    int number_of_slashes = is_valid_path(path);
    if (number_of_slashes)
    {
        char** directoryNames = parser(path, number_of_slashes, last_dir_name);
        if (!directoryNames) return 0;
        element* r = MANAGER.root;
        element* result = find_elem(directoryNames, number_of_slashes, is_path_absolute(path) ? MANAGER.root : MANAGER.current_dir);
        free_two_demension_char_arr(directoryNames);
        return result;
    }
    else
    {
        return 0;
    }
}

element* create_element(char name[], int size, int isfile, element* parent)
{
    element* elem = (element*)malloc(sizeof(element));
    if (elem)
    {
        elem->name = name;
        elem->size = size;
        elem->isfile = isfile;
        elem->number_of_kids = 0;
        elem->kids = NULL;
        elem->parent = parent;
        if (parent)
            parent->number_of_kids += 1;
    }
    return elem;
}

void add_kid_to_kids(element* parent, element* kid) // изменить
{
    parent->number_of_kids++;
    int size_kids_old = sizeof(parent->kids) / sizeof(element*);
    element** new_kids_arr = (element**)malloc((parent->number_of_kids) * sizeof(element*));
    for (int i = 0; i < size_kids_old; i++)
    {
        new_kids_arr[i] = parent->kids[i];
    }
    new_kids_arr[size_kids_old] = kid;
    free_elements_array(parent->kids, parent->number_of_kids);
    parent->kids = new_kids_arr;
    parent->kids[parent->number_of_kids - 1] = kid;
}

void free_elements_array(element** arr, int number_of_kids)
{
    for (int i = 0; i < number_of_kids; i++)
    {
        free(arr[i]);
    }
}

void delete_element(element* node)
{
    free(node->name);
    free_elements_array(node->kids, node->number_of_kids);
    MANAGER.current_size -= node->size;
    free(node->parent);
}

void remove_recursive(element* node)
{
    if (node->number_of_kids == 0)
    {
        delete_element(node);
        free(node);
    }
    else
    {
        for (int i = 0; i < node->number_of_kids; i++)
        {
            remove_recursive(node->kids[i]);
        }
    }
}

char** secure_char_array_expansion(char** arr, int curr_size, int necessary_size) // првоерить тщательно
{
    char** new_arr = (char**)malloc(sizeof(char*) * necessary_size);
    if (!new_arr) return NULL;
    for (int i = 0; i < curr_size; i++)
    {
        new_arr[i] = arr[i];
    }
    return new_arr;
}

char* create_path(element* curr_dir)
{
    // собираем полный путь в массив (идем с нашей директории, поэтому он будет задом наперед)
    // собираем в одну строчку задом наперед со всеми /
    // либо
    // с начала ищем нужную директорию
    element* root = curr_dir;
    char** whole_path_reversed = (char**)malloc(sizeof(char*));
    int path_length = 0;
    do
    {
        whole_path_reversed = secure_char_array_expansion(whole_path_reversed, path_length, path_length + 1);
        if (!whole_path_reversed) return NULL;
        whole_path_reversed[path_length] = root->name;
        root = root->parent;
        path_length++;
    } while (root != NULL);
    // создаем с запасом
    char* whole_path[129];
    whole_path[0] = '\0';
    // проверить, охватывает ли это весь путь, или надо написать i >= path_lenght
    if (path_length >= 129) return NULL;
    for (int i = path_length - 1; i >= path_length-1; i--)
    {
        strcat(whole_path, whole_path_reversed[i]);
    }
    whole_path[path_length] = '\0';
    int len = strlen(whole_path);
    // len + 1 потому что еще есть '/0'
    char* result = (char*)malloc(sizeof(char) * (len + 1));
    if (!result) return NULL;
    strcpy(result, whole_path);
    return result;
}
// ---------------------------------------------

// My func

int my_create(int disk_size)
{
    // проверка, если менеджер уже создавался
    if (!checkFileManagerAvailability(0)) return 0;
    MANAGER.size = disk_size;
    element* root = create_element("/", 0, 0, NULL);
    MANAGER.root = root;
    MANAGER.current_dir = root;
    return 1;
}

int my_destroy()
{
    // будет ли работать my_destroy
    if (!checkFileManagerAvailability(0)) return 0;
    if (!MANAGER.root) return 0;
    remove_recursive(MANAGER.root);
    return 1;
}

int my_create_dir(const char* path)
{
    if (!checkFileManagerAvailability(0)) return 0;
    char* last_dir_name;
    element* curr_dir = go_to_path(path, &last_dir_name);
    _CrtDumpMemoryLeaks();
    if (!curr_dir) return 0;
    element* new_dir = create_element(last_dir_name, 0, 0, curr_dir);
    add_kid_to_kids(curr_dir, new_dir);
    return 1;
}

int my_create_file(const char* path, int file_size)
{
    if (!checkFileManagerAvailability(file_size)) return 0;
    MANAGER.current_size += file_size;
    char* last_file_name;
    element* curr_dir = go_to_path(path, &last_file_name);
    if (!curr_dir) return 0;
    element* new_file = create_element(last_file_name, file_size, 1, curr_dir);
    add_kid_to_kids(curr_dir, new_file);
    return 1;
}

int my_remove(const char* path, int recursive)
{
    if (!checkFileManagerAvailability(0)) return 0;
    char* last_file_name;
    element* curr_dir = go_to_path(path, &last_file_name);
    if (!curr_dir) return 0;
    curr_dir = find_elem(&last_file_name, 1, curr_dir);
    if (recursive) remove_recursive(curr_dir);
    else delete_element(curr_dir);
    return 1;
}

int my_get_cur_dir(char* dst) {
    if (!checkFileManagerAvailability(0)) return 0;
    dst = create_path(MANAGER.current_dir);
    return 1;
}
//************************************************


