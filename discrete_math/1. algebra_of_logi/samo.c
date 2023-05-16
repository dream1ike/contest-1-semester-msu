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
		fscanf(fin, "%d", &trash);
		fscanf(fin, "%d", sizeofconjunct);
		fscanf(fin, "%d", sizeofpolynom);
		fscanf(fin, "%c", &trash);
		polinom = (char**)malloc(sizeof(char*) * (*sizeofpolynom)); // выделяем память под полином
		for (int i = 0; i < sizeofpolynom; i++)
		{
			polinom[i] = (char*)malloc(sizeof(char) * (*sizeofconjunct)); // выделяем память под конъюнкт
			for (int j = 0; j < sizeofconjunct; j++)
			{
				fscanf(fin, "%c", &polinom[i]);
			}
			fscanf(fin, "%c", &trash); // пропускаем пробел
			fscanf(fin, "%c", &trash); // пропускаем коэффицент, так как он постоянно 1
			fscanf(fin, "%c", &trash); // пропускаем символ '\n'
		}
	}
	return polinom;
}

int main(int argc, char* argv[]) 
{
	
	return 0;
}

