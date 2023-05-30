#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#pragma warning(disable:4996)

void free_two_demensional_char_arr(char** arr, int sizeofarr); // done

char* read_from_file(const char* filename, int* numberofval, int* numberofone); // done

char* decToBinary(int n, int size); // done

char** find_one_and_group(int numberofval, char* vec_of_val, int numberofone); // done

char* merge_vectors(char* vec1, char* vec2, int size);

char** quine_minimizing(char** vec_table, int table_size, int numberofval, int* result_size, char* flag); // in progress

int compare_two_vectors(char* vec1, char* vec2, int size); // done


int main(int argc, const char* argv[])
{
    if (1)
    {
        int numberofval, numberofone;
        char* vec_of_val = read_from_file("/Users/timurbajdadaev/Visual_Studio_Code/Contest/file2.txt", &numberofval, &numberofone);
        int table_size = numberofone;
		// вывод входной строки
		int size = 1 << numberofval;
		printf("%d\n", numberofone);
        for (int i = 0; i < size; i++)
        {
            printf("%c", vec_of_val[i]);
        }
        printf("\n\n");
		// конец вывода
        char** vec_table = find_one_and_group(numberofval, vec_of_val, numberofone);
		int result_size = 0;
        char flag = 'C';
		while (flag != 'S') char** vec_table = quine_minimizing(vec_table, table_size, numberofval, &result_size, &flag);
		// вывод массива
        for(int i = 0; i < result_size; i++)
        {
            for(int j = 0; j < numberofval; j++)
            {
                printf("%c", vec_table[i][j]);
            }
            printf("\n");
        }
		// конец вывода массива

        free_two_demensional_char_arr(vec_table, table_size);
    }
    return 0;
}

char** quine_minimizing(char** vec_table, int table_size, int numberofval, int* result_size, char* flag)
{
    *flag = 'S';
	char** result = NULL;
	*result_size = 0;
	int differences = 0;
	for(int i = 0; i < table_size; i++)
	{
		for (int j = i; j < table_size; j++)
		{
			if (abs(vec_table[i][numberofval] - vec_table[j][numberofval]) == 1) // соседние (по единичкам) подгруппы
			{
				differences = compare_two_vectors(vec_table[i], vec_table[j], numberofval);
				if (differences == 1)
				{
                    *flag = 'C';
					*result_size += 1;
					result = (char**)realloc(result, (*result_size) * sizeof(char*));
					result[*result_size - 1] = merge_vectors(vec_table[i], vec_table[j], numberofval);
				}
			}
		}
	}
    if (*flag == 'S')
    {
        *result_size = table_size;
        return vec_table;
    }
	free_two_demensional_char_arr(vec_table, table_size);
	return result;
}

void free_two_demensional_char_arr(char** arr, int sizeofarr) // done
{
	for (int i = 0; i < sizeofarr; i++) // очистка массива
	{
		if (arr[i]) free(arr[i]);
	}
	free(arr);
}

char* read_from_file(const char* filename, int* numberofval, int* numberofone) // done
{
    FILE* fin = fopen(filename, "r");
    (* numberofone) = 0;
    if (fin)
    {
        char trash;
        fscanf(fin, "%d", numberofval); // считываем k-значность логики
        fscanf(fin, "%d", numberofval); // кол-во переменных
        fscanf(fin, "%c", &trash); // считываем пробел
        int size = 1 << (*numberofval);
        char* vec_of_val = (char*)malloc(sizeof(char) * size);
        for (int i = 0; i < size; i++)
        {
            fscanf(fin, "%c", &vec_of_val[i]);
            if (vec_of_val[i] == '1') (*numberofone)++;
        }
        fclose(fin);
        return vec_of_val;
    }
    return NULL;
}

char* decToBinary(int n, int size) 
{
    char* binaryNum = (char*)malloc((size + 1) * sizeof(char));
    char number_of_ones = '0';

    // инициализируем массив нулями
    for (int i = 0; i < size; i++) {
        binaryNum[i] = '0';
    }

    int i = size - 1;
    while (n > 0 && i >= 0) {
        if ((n & 1) == 1) number_of_ones++; // считаем кол-во единичек
        binaryNum[i] = (n & 1) + '0'; // устанавливаем биты числа и переводим их в символы '0' или '1
        n >>= 1; // сдвигаем число на одну позицию вправо
        i--;
    }
    
    binaryNum[size] = number_of_ones; // записываем в виде символа кол-во единичек (для красоты)
    return binaryNum;
}

char** find_one_and_group(int numberofval, char* vec_of_val, int numberofone) // done
{
    char** vec_table = (char**)malloc(sizeof(char*) * numberofone);
    int j = 0;
    int size = 1 << numberofval;
    for (int i = 0; i < size; i++)
    {
        if (vec_of_val[i] == '1')
        {
            vec_table[j] = decToBinary(i, numberofval);
            j++;
        }
    }
    free(vec_of_val);
    return vec_table;
}

char* merge_vectors(char* vec1, char* vec2, int size) 
{
    char* result = (char*)malloc((size + 1) * sizeof(char));
    
    int i;
    for(i = 0; i < size; i++) {
        if(vec1[i] != vec2[i]) {
            result[i] = '*';
        } else {
            result[i] = vec1[i];
        }
    }
    // записываем в конец количество единиц в наборе
    if (vec1[i] > vec2[i]) result[i] = vec1[i]-1;
    else result[i] = vec2[i]-1;
    
    return result;
}

int compare_two_vectors(char* vec1, char* vec2, int size) // done
{
    int differences = 0; // счетчик, который отвечает за количество разных элементов
    for (int i = 0; i < size; i++)
    {
        if (vec1[i] != vec2[i]) differences++;
    }
	return differences;
}