#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma warning(disable:4996)
#define eps 1e-10
int main(int argc, char* argv[]) {
	if (argc == 3)
	{
		FILE* fin = fopen(argv[1], "r");
		FILE* fout = fopen(argv[2], "w");
		if (fin && fout)
		{
			int counter = 0, descending_sequence = 0, size = 0;
			double el_1, el_2;
			if (fscanf(fin, "%lf", &el_1) > 0) {
				counter++;
				size++;
				while (fscanf(fin, "%lf", &el_2) > 0) {
					if (el_1 < el_2 && fabs(el_1 - el_2) > eps) 
					{
						counter++;
					}
					size++;
					el_1 = el_2;
				}
				if (size == 1) counter = 1;
				if (size > 1 && counter == 0) counter++;
			}
			fprintf(fout, "%d", counter);
			fclose(fin);
			fclose(fout);
		}
	}
	return 0;
}
//Количество различных элементов неубывающей последовательности.