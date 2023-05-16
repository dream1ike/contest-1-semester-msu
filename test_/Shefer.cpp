#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma warning(disable:4996)

int t0(const char* num, int size) //проверка на t0
{
	for (int i = 0; i < size; i++) 
	{
		if (num[i] == 49) 
		{
			return 1;
		}
	}
	return 0;
}

int t1(const char* num, int size) //проверка на t1
{
	for (int i = 0; i < size; i++) 
	{
		if (num[i] == 48) 
		{
			return 1;
		}
	}
	return 0;
}

int s(const char* num, int size) //функция проверки векторного представления на самодвойственность
{
	int _size = size;
	for (int i = 0; i < (_size / 2); i++) 
	{
		size--;
		if (num[i] != num[size])
		{
			return 1;
		}
	}
	return 0;
}

int* paskals_triangle(const char* num, int size) //функция треугольника паскаля
{
	int _size = size;
	int* _arr = NULL;
	int* arr_triangle = NULL;
	arr_triangle = (int*)malloc(sizeof(int) * size);
	_arr = (int*)malloc(sizeof(int) * size);
	for (int i = 0; i < size; i++) 
	{
		_arr[i] = num[i];
		_arr[i] = _arr[i] % 48;
	}
	if (arr_triangle && _arr) 
	{
		for (int i = 0; i < size; i++) 
		{
			arr_triangle[i] = _arr[0];
			_size--;
			for (int j = 0; j < _size; j++) 
			{
				_arr[j] = _arr[j]^ _arr[j + 1];
			}
		}
	}
	free(_arr);
	return arr_triangle;
}

int answer(int* triangle, int size, int lenght) //проверка функции на линейность
{
	for (int counter; counter < lenght; counter++) 
	{
		if (triangle[counter] == 1)
		{
			int zxc = counter;
			int _counter = 0;
			int j = 0;
			for (int i = 0; i < size; i++) 
			{
				_counter = zxc & 1;
				zxc = zxc >> 1;
				if (_counter == 1) j++;
			}
			if (j > 1) 
			{
				free(triangle);
				return 1;
			}
		}
	}
	free(triangle);
	return 0;
}

int main(int argc, const char* argv[]) //считывание с файла и запись результата в out
{
	if (3)
	{
		FILE* fin = fopen("in.txt", "r");
		FILE* fout = fopen("out.txt", "w");
		if (fin && fout)
		{
			int value = 0;
			int var = 0;
			int lenght = 0;
			char* num = 0;
			if (fscanf(fin, "%d", &value) > 0)
			{
				if (fscanf(fin, "%d ", &var) > 0) 
				{
					lenght = pow(2, var);
					num = (char*)malloc(sizeof(char) * lenght);
					for (int i = 0; i < lenght; i++) 
					{
						fscanf(fin, "%c", &num[i]);
					}
				}
			}
			int ans = t0(num, lenght);
			if (ans == 0) 
			{
				fprintf(fout, "0");
				return 0;
			}
			ans = t1(num, lenght);
			if (ans == 0) 
			{
				fprintf(fout, "0");
				return 0;
			}
			ans = s(num, lenght);
			if (ans == 0) 
			{
				fprintf(fout, "0");
				return 0;
			}
			int* triangle = paskals_triangle(num, lenght);
			ans = answer(triangle, var, lenght);
			fprintf(fout, "%d", ans);
			fclose(fin);
			fclose(fout);
		}
	}
	return 0;
}