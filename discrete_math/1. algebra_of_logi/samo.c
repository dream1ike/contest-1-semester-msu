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

void decToBinary(int n, int size, char* binaryNum) {

	// инициализируем массив нулями
	for (int i = 0; i < size; i++) {
		binaryNum[i] = 48; // ASCII код символа '0'
	}

	int i = size - 1;
	while (n > 0 && i >= 0) {
		binaryNum[i] += (n & 1);
		n = n >> 1;
		i--;
	}

}

void create_mirror_set(int sizeofconjunct, char* set)
{
	for (int i = 0; i < sizeofconjunct; i++)
	{
		if (set[i] == '1') set[i] = '0';
		else set[i] = '1';
	}
}

int calc_conjunct(char* conjunct, int sizeofconjunct, char* set)
{
	for (int i = 0; i < sizeofconjunct; i++)
	{
		if (conjunct[i] == '1' && set[i] != '1') return 0;
	}
	return 1;
}

int calc_polynom(int sizeofconjunct, int sizeofpolynom, char** polynom, char* set)
{
	int answer = 0;
	for (int i = 0; i < sizeofpolynom; i++)
	{
		answer = answer ^ calc_conjunct(polynom[i], sizeofconjunct, set);
	}
	return answer;
}

int calc_self_duality(int sizeofconjunct, int sizeofpolynom, char** polynom)
{
	double numberofsets = pow(2, (double)sizeofconjunct-1);
	char* set = (char*)malloc(sizeofconjunct * sizeof(char)); // набор, на котором мы проверяем полином
	int f_set, mirror_f_set; // значения функции на наборе и на зеркальном наборе
	for (int i = 0; i < numberofsets; i++)
	{
		decToBinary(i, sizeofconjunct, set);
		f_set = calc_polynom(sizeofconjunct, sizeofpolynom, polynom, set);
		create_mirror_set(sizeofconjunct, set);
		mirror_f_set = calc_polynom(sizeofconjunct, sizeofpolynom, polynom, set);
		if (f_set == mirror_f_set)
		{
			free(set);
			return 0;
		}
	}
	free(set);
	return 1;
}

void write_number_to_file(const char* filename, int number)
{
	FILE* fout = fopen(filename, "w");
	if (fout)
	{
		fprintf(fout, "%d", number);
		fclose(fout);
	}
}

int main(int argc, char* argv[])
{
	if (argc == 3)
	{
		int sizeofpolynom, sizeofconjunct;
		char** polynom = read_from_file("in.txt", &sizeofconjunct, &sizeofpolynom);
		int answer = calc_self_duality(sizeofconjunct, sizeofpolynom, polynom);
		write_number_to_file("out.txt", answer);
		free_two_demensional_char_arr(polynom, sizeofpolynom);
		return 0;
	}
	return 1;
}

