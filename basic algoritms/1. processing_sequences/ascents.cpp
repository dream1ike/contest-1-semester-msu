#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#pragma warning(disable:4996)
#define eps 1e-10 
int main(int argc, const char* argv[])
{
	if (argc == 3) {
		FILE* fin = fopen(argv[1], "r");
		FILE* fout = fopen(argv[2], "w");
		if (fin && fout)
		{
			double elem1, elem2;
			double counter = 0, answer = 0;
			if (fscanf(fin, "%lf", &elem1) > 0)
			{
				while (fscanf(fin, "%lf", &elem2) > 0)
				{
					if (elem1 < elem2)
					{
						counter++;
					}
					else// if (counter > 0) 
// если у нас последовательность из одинаковых элементов, answer будет прибавляться каждый элемент
					{
						counter = 0;
						answer++;
					}
					elem1 = elem2;
				}
				if (counter > 0)
				{
					counter = 0;
					answer++;
				}
			}
			fprintf(fout, "%g ", answer);
			fclose(fin);
			fclose(fout);
		}
	}
	return 0;
}
//Количество возрастающих участков последовательности











