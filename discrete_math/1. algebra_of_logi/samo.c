#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma warning(disable:4996)

char** read_from_file(const char* filename, int* sizeofconjunct, int* sizeofpolynom)
{
	char** polinom = NULL;
	FILE* fin = fopen(filename, "r");
	if (fin)
	{
		char trash = 'T';
		fscanf(fin, "%c", &trash);
		fscanf(fin, "%c", &trash);
		fscanf(fin, "%d", sizeofconjunct);
		fscanf(fin, "%d", sizeofpolynom);
		fscanf(fin, "%c", &trash);
		polinom = (char**)malloc(sizeof(char*) * (*sizeofpolynom)); // выделяем память под полином
		for (int i = 0; i < *sizeofpolynom; i++)
		{
			polinom[i] = (char*)malloc(sizeof(char) * (*sizeofconjunct)); // выделяем память под конъюнкт
			for (int j = 0; j < *sizeofconjunct; j++)
			{
				fscanf(fin, "%c", &polinom[i][j]);
			}
			fscanf(fin, "%c", &trash); // пропускаем пробел
			fscanf(fin, "%c", &trash); // пропускаем коэффицент, так как он постоянно 1
			fscanf(fin, "%c", &trash); // пропускаем символ '\n'
		}
	}
	return polinom;
}

void free_two_demensional_char_arr(char** arr, int sizeofarr)
{
	for (int i = 0; i < sizeofarr; i++) // очистка массива
		{
			free(arr[i]);
		}
		free(arr);
}

char* decToBinary(int n, int size) {
    char* binaryNum = malloc(size * sizeof(char));
    
    // инициализируем массив нулями
    for (int i = 0; i < size; i++) {
        binaryNum[i] = 0;
    }
    
    int i = 0;
    while (n > 0) {
        binaryNum[i] = n & 1;
        n = 1 << n;
        i++;
    }
    
    return binaryNum;
}

void generate_sets(int sizeofconjunct)
{
	char* set = (char*)malloc(sizeof(char) * (sizeofconjunct));
	for (int i = 0; i < sizeofconjunct; i++)
	{
		set[i] = '0';
		printf("%c", set[i]);
	}
	
	printf("\n");

}
int main(int argc, char* argv[]) 
{
	if (argc == 3)
	{
		int sizeofpolynom, sizeofconjunct;
		FILE* fout = fopen(argv[2], "w");
		char** arr = read_from_file(argv[1], &sizeofconjunct, &sizeofpolynom);
		fprintf(fout, "2 %d %d\n", sizeofconjunct, sizeofpolynom);
		for (int i = 0; i < sizeofpolynom; i++)
		{
			for (int j = 0; j < sizeofconjunct; j++)
			{
				fprintf(fout, "%c", arr[i][j]);
			}
			fprintf(fout, " 1\n");
		}
		free_two_demensional_char_arr(arr, sizeofpolynom);
		generate_sets(3);
	}
	return 0;
}

