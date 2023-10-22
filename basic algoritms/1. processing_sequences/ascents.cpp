#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#pragma warning(disable:4996)
#define eps 1e-10 
int main(int argc, const char* argv[])
{
	if (argc == 3) {
		FILE* fin = fopen("file1.txt", "r");
		FILE* fout = fopen("file2.txt", "w");
		if (fin && fout)
		{
			double elem1, elem2;
			double counter = 0, answer = 0;
			if (fscanf(fin, "%lf", &elem1) > 0)
			{
				while (fscanf(fin, "%lf", &elem2) > 0)
				{
					if (elem1 > elem2)
					{
						elem1 = elem2;
						while (fscanf(fin, "%lf", &elem2) > 0 && elem1 > elem2)
						{
							elem1 = elem2;
						}
					}
					else
					{
						elem1 = elem2;
						while (fscanf(fin, "%lf", &elem2) > 0 && elem1 < elem2)
						{
							elem1 = elem2;
						}
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











