#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define eps 1e-10
int main(int argc, char* argv[]) {
	if (3) {
		FILE* fin = fopen("in.txt", "r");
		FILE* fout = fopen("out.txt", "w");
		int max = 0;
		if (fin && fout) {
			double elem1, elem2;
			int counter = 0;
			if (fscanf(fin, "%lf", &elem1) > 0)
			{
				while (fscanf(fin, "%lf", &elem2) > 0)
				{
					if (elem1 < elem2)
					{
						counter+=2;
						elem1 = elem2;
						while (fscanf(fin, "%lf", &elem2) > 0 && elem1 < elem2)
						{
							counter++;
							elem1 = elem2;
						}
					}
					if (max < counter)
					{
						max = counter;
						counter = 0;
					}
					else counter = 0;
					elem1 = elem2;
				}
			}
			else
			{
				fprintf(fout, "0");
				return 0;
			}
			fprintf(fout, "%d", max);
			fclose(fin);
			fclose(fout);
		}
	}
	return 0;
}
//Длина наибольшего возрастающего участка.
//Если для любого \(i: a_i<=a_{i+1}\), то считаем длиной возрастающего участка 0.