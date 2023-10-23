#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include <math.h>
#pragma warning(disable:4996)
// —труктура Ёлемента
typedef struct element
{
    char* name;
    int size;
    char isfile;
    int number_of_kids;
    struct element** kids;
    struct element* parent;

} element;

// —труктура ‘айлового менеджера
typedef struct {
    element* root;      //  орнева€ директори€
    int size;
    element* current_dir;   // “екуща€ директори€
    int current_size;
} FileManager;

FileManager MANAGER;

// Supporting func
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
    char* dirName = (char*)malloc(sizeof(char) * (size_of_name_dir+1));
    for (int i = 0; i < size_of_name_dir; i++)
    {
        dirName[i] = name_dir_static[i];
    }
    dirName[size_of_name_dir] = '\0';
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
    element* result_directory = root;
    for (int i = 0; i < size_directoryNames; i++)
    {
        for (int j = 0; j < result_directory->number_of_kids; j++)
        {
            element* kid = result_directory->kids[j];
            if (!strcmp(directoryNames[i], kid->name))
            {
                // ≈сли нашли директорию с таким именем
                result_directory = kid;
                break;
            }
        }
        // ≈сли не нашли нужную директорию
        if (strcmp(directoryNames[i], result_directory->name)) return 0;
    }
    return result_directory;
}

element* go_to_path(const char* path, char** last_dir_name) // last_dir_name нужен дл€ create_dir
{
    element* root = MANAGER.current_dir;
    if (is_path_absolute(path)) root = MANAGER.root;
    int number_of_slashes = is_valid_path(path);
    if (number_of_slashes)
    {
        char** directoryNames = parser(path, number_of_slashes, last_dir_name);
        if (!directoryNames) return 0;
        return find_elem(directoryNames, number_of_slashes, root);
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

void add_kid_to_kids(element* parent, element* kid)
{
    parent->number_of_kids++;
    parent->kids = (element**)realloc(parent->kids, parent->number_of_kids);
    parent->kids[parent->number_of_kids - 1] = kid;
}
// ---------------------------------------------

// My func
int my_create_dir(const char* path)
{
    char* last_dir_name;
    element* curr_dir = go_to_path(path, &last_dir_name);
    if (!curr_dir) return 0;
    element* new_dir = (element*)malloc(sizeof(element));
    add_kid_to_kids(curr_dir, new_dir);
    new_dir->name = last_dir_name;
    new_dir->size = 0;
    new_dir->isfile = 0;
    new_dir->number_of_kids = 0;
    new_dir->kids = NULL;
    new_dir->parent = curr_dir;
    return 1;
}

int my_create(int disk_size)
{
    // ѕроверка, если менеджер уже создавалс€
    if (MANAGER.root) return 0;
    MANAGER.size = disk_size;
    element* root = create_element("/", 0, 0, NULL);
    MANAGER.root = root;
    MANAGER.current_dir = root;
    return 1;
}

int main()
{
    my_create(10);
    my_create_dir("/ogo");
    return 0;
}



// For parser and get_first_dirname_from_path
// int main() {
//     int number_of_slashes = 2;
//     char* last_name;
//     char** names = parser("123/asdf/ogo", number_of_slashes, &last_name);

//     for (int i = 0; i < number_of_slashes; i++)
//     {
//         for (int j = 0; j < strlen(names[i]); j++)
//         {
//             printf("%c", names[i][j]);
//         }
//         printf("\n");

//     }
//     printf("----\n");
//     for (int j = 0; j < strlen(last_name); j++)
//     {
//         printf("%c", last_name[j]);
//     }
//     return 0;
// }
