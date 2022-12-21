#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)
long int reverse_num(long int x)
{
    unsigned int n = 0;
    for (long int i = 0; i < 8; i++)
    {
        if ((x & 1) == 1)
        {
            n = n | 1 << (8-i-1);
        }
        x = x >> 1;
    }
    return n;
}
int get_num_from_file(const char* filename)
{
    FILE* fin = fopen(filename, "r");
    int x = 0;
    if (fin)
    {
        if (fscanf(fin, "%d", &x) < 0) return -1;
        fclose(fin);
    }
    return x;
}
int mirrors(int x, const char* filename)
{
    FILE* fout = fopen(filename, "w");
    if (fout)
    {
        if (x <= 0)
        {
            fprintf(fout, "0");
            return 0;
        }
        for(long int j = 1; j <=x; j++)
        {
            fprintf(fout, "%ld ", ((j / 256) << 24)+ (reverse_num(j/256) << 16) + ((j % 256) << 8) + reverse_num(j % 256));
        }
        }
        fclose(fout);
        return 0;
    }

int main(int argc, const char* argv[])
{
    if (argc == 3)
    {
    int x = get_num_from_file(argv[1]);
    mirrors(x, argv[2]);
    }
    return 0;
}