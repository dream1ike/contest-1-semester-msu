#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#pragma warning(disable:4996)
#define eps 1e-10 
double* read_from_file(const char* fileName, int* size, double* x)
{
	FILE* fin = fopen(fileName, "r");
	if (fin)
	{
		if (fscanf(fin, "%lf", x) > 0)
		{
			if (fscanf(fin, "%d", size) > 0)
			{
				int i = 0;
				double* arr = (double*)malloc(sizeof(double) * (*size));
				if (arr)
				{
					while (fscanf(fin, "%lf", &arr[i]) > 0 && i < *size)
					{
						i++;
					}
					return arr;
				}
			}
			else return NULL;
		}
		else return NULL;
		fclose(fin);
	}
	return NULL;
}
void write_to_file(const char* fileName, double* arr, int size)
{
	FILE* fout = fopen(fileName, "w");
	if (fout)
	{
		if (arr)
		{
			fprintf(fout, "%d ", size);
			for (int i = 0; i < size; i++)
			{
				fprintf(fout, "%g ", arr[i]);
			}
			free(arr);
		}
		else fprintf(fout, "0");
		fclose(fout);
	}
}
void half_of_sum_of_x_elems(double* arr, int size, double x)
{
	int i = 0, begin, end;
	double sum = 0;
	while (i < size)
	{
		if (fabs(arr[i] - x) < eps)
		{
			begin = i;
			i++;
			while (fabs(arr[i] - x) < eps) i++;
			end = i - 1;
			if (begin - 1 >= 0) sum += arr[begin - 1];
			if (end + 1 < size) sum += arr[end + 1];
			while (begin <= end)
			{
				arr[begin] = sum / 2;
				begin++;
			}
			sum = 0;
		}
		i++;
	}
}
int main(int argc, const char* argv[])
{
	if (argc == 3) {
		int size = 0;
		double x = 0;
		double* arr = read_from_file(argv[1], &size, &x);
		half_of_sum_of_x_elems(arr, size, x);
		write_to_file(argv[2], arr, size);
	}
	return 0;
}
//Для заданного числа x aзаменить элементы каждого x-отрезка на полусумму элементов, прилегающих к этому отрезку справа и слева.