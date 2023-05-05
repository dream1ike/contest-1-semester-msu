

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main()
{
    char command[100], fileName[20], c;
    int i, j;

    while(1) {

        printf("\n$ ");  // приглашение команды

        gets(command);  // ввод команды

        for(i = 0; command[i] != ' ' && command[i] != '\0'; i++);  // поиск пробела

        j = 0;

        while(command[i] == ' ')  // пропуск всех пробелов
            i++;

        while(command[i] != '\0') {  // сохранение имени файла
            fileName[j++] = command[i++];
        }

        fileName[j] = '\0';

        if (strcmp(command, "ls") == 0) {   // ls - вывод списка файлов/каталогов

            system("ls");   // system() - вызывает shell-комманду ls

            continue;   // continue - просто игнорирует оставшуюся часть цикла, идя на следующую итерацию.

        } else if (strcmp(command, "cat") == 0) {   // cat - print contents of a file to the screen.

            FILE *fp;   // FILE - тип, о