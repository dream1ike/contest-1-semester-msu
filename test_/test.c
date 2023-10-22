#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include <math.h>
#pragma warning(disable:4996)
// Структура Элемента
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



// For parser and get_first_dirname_from_path
int main() {
    int number_of_slashes = 3;
    char* last_name;
    char** names = parser("/123/asdf/ogo", number_of_slashes, &last_name);

    for (int i = 0; i < number_of_slashes; i++)
    {
        for (int j = 0; j < strlen(names[i]); j++)
        {
            printf("%c", names[i][j]);
        }
        printf("\n");
        
    }
    printf("----\n");
    for (int j = 0; j < strlen(last_name); j++)
    {
        printf("%c", last_name[j]);
    }
    return 0;
}
