#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#pragma warning(disable:4996)
typedef struct
{
	double x1;
	double y1;
	double x2;
	double y2;
} segment;
typedef struct
{
	double x;
	double y;
}vector;
segment* read_segments_to_arr(const char* fileName, int* quantity_points)
{
	FILE* fin = fopen(fileName, "r");
	if (fin)
	{
		if (fscanf(fin, "%d", quantity_points) > 0)
		{
			segment* arr = (segment*)malloc(sizeof(segment) * (*quantity_points));
			if (arr)
			{
				int i = 0;
				if ((fscanf(fin, "%lf", &arr[i].x1) > 0) && (fscanf(fin, "%lf", &arr[i].y1) > 0)) {
					while ((fscanf(fin, "%lf", &arr[i].x2) > 0) && (fscanf(fin, "%lf", &arr[i].y2) > 0))
					{
						i++;
						arr[i].x1 = arr[i - 1].x2;
						arr[i].y1 = arr[i - 1].y2;
					}
				}
				*quantity_points = i;
				arr = (segment*)realloc(arr, sizeof(segment) * (*quantity_points));
				fclose(fin);
				return arr;

			}
		}
		fclose(fin);
	}
	return NULL;
}
int check_intersection_for_segments(segment AB, segment CD)
{
	vector _AB = { AB.x2 - AB.x1, AB.y2 - AB.y1 };
	vector _CD = { CD.x2 - CD.x1, CD.y2 - CD.y1 };
	vector _AC = { CD.x1 - AB.x1, CD.y1 - AB.y1 };
	vector _AD = { CD.x2 - AB.x1, CD.y2 - AB.y1 };
	vector _CB = { AB.x2 - CD.x1, AB.y2 - CD.y1 };
	vector _CA = { AB.x1 - CD.x1, AB.y1 - CD.y1 };
	double AB_AC = (_AB.x * _AC.y) - (_AB.y * _AC.x);
	double AB_AD = (_AB.x * _AD.y) - (_AB.y * _AD.x);
	double CD_CB = (_CD.x * _CB.y) - (_CD.y * _CB.x);
	double CD_CA = (_CD.x * _CA.y) - (_CD.y * _CA.x);
	if (((AB_AC * AB_AD) < 0) && ((CD_CB * CD_CA) < 0)) return 1;
	else return 0;
}
int check_intersection_for_broken_straight_line(segment* arr, int quantity_segments)
{
	for (int i = 0; i < quantity_segments; i++)
	{
		for (int j = i + 1; j < quantity_segments; j++)
		{
			if (check_intersection_for_segments(arr[i], arr[j]) == 1) return 1;
		}
	}
	return 0;
}
int main(int argc, const char* argv[])
{
	if (argc == 3)
	{
		FILE* fout = fopen(argv[2], "w");
		if (fout)
		{
			int size;
			segment* arr = read_segments_to_arr(argv[1], &size);
			if (arr)
			{
				fprintf(fout, "%d", check_intersection_for_broken_straight_line(arr, size));
				free(arr);
			}
			fclose(fout);
		}
	}
	return 0;
}
//Самопересечение ломанной