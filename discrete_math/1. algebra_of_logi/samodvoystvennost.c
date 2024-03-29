#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int str_to_int(char* conjunct, int numberofvar) // массив символов преобразует в число, где каждый бит - переменная
{
    int final_conjunct = 0;
    int i = 0;
    while (i < numberofvar)
    {
        if (conjunct[i] == '1') final_conjunct++;
        i++;
        if (i < numberofvar) final_conjunct = final_conjunct << 1;
    }
    return final_conjunct;
}

int* read_polinom(const char* filename, int* sizeofarr, int* numberofvar) // функция, которая считывает полином с массива
{
    FILE* fin = fopen(filename, "r");
    int* polinom = NULL;
    char skip;
    int k;
    if (fin)
    {
        fscanf(fin, "%d", &k);
        fscanf(fin, "%d", numberofvar);
        fscanf(fin, "%d", sizeofarr);
        fscanf(fin, "%c", &skip);
        polinom = (int*)malloc(sizeof(int) * (*sizeofarr));
        if (polinom)
        {
            char* conjunct = (char*)malloc(sizeof(char) * (*numberofvar));
            if (conjunct)
            {
                for (int i = 0; i < (*sizeofarr); i++)
                {
                    for (int j = 0; j < *numberofvar; j++) // считываем конъюнкт
                    {
                        fscanf(fin, "%c", &conjunct[j]);
                    }
                    fscanf(fin, "%c", &skip); // пропускаем пробел
                    fscanf(fin, "%c", &skip); // пропускаем коэффицент, так как он постоянно 1
                    fscanf(fin, "%c", &skip); // пропускаем символ '\n' 
                    polinom[i] = str_to_int(conjunct, *numberofvar); // записываем уже готовый конъюнкт в наш массив
                }
                free(conjunct);
            }
            
        }

    fclose(fin);
    }
    return polinom;
}

int calc_conjunct(int conjunct, int set) // вычисляет значение конъюнкта на конкретном наборе
{
    if (((conjunct & set) == conjunct )&& (conjunct != 0)) return 1;
    return 0;
}

int calc_polinom(int* polinom, int set, int sizeofarr) // вычисляет значение полинома на конкретном наборе
{
    int answer = 0;
    for (int i = 0; i < sizeofarr; i++)
    {
        answer = answer ^ calc_conjunct(polinom[i], set);
        //printf("polinom = [%d], set = [%d], answer = [%d] \n", polinom[i], set, answer);
    }
    //printf("End of session \n");
    return answer;
}

int calc_self_duality(int sizeofarr, int* polinom, int numberofvar) // основная функция
{
    int number_of_sets = (1 << (numberofvar));
    for (int set = 0; set < number_of_sets/2; set++) // счетчик, который отвечает за наборы, на которых мы проверяем ф-ю
    {
        int polinom1 = calc_polinom(polinom, set, sizeofarr);
        int polinom2 = calc_polinom(polinom, number_of_sets - set - 1, sizeofarr); 
        //printf("polinom1 = [%d]\npolinom2 = [%d]\n\n", polinom1, polinom2);
        if (polinom1 == polinom2) return 0;
    }
    return 1;
}

void calc_and_write_table(int sizeofarr, int* polinom, int numberofvar)// test_func
{
    int high = 1 << numberofvar;
    for (int set = 0; set < high/2; set++)
    {
        printf("%d", calc_polinom(polinom, high - 1 - set, sizeofarr));
    }
    for (int set = high/2; set < high; set++)
    {
        printf("%d", calc_polinom(polinom, high - 1 - set, sizeofarr));
    }
}

void write_to_file(const char* filename, int answer)
{
    FILE* fout = fopen(filename, "w");
    if (fout)
    {
        fprintf(fout, "%d", answer);
        fclose(fout);
    }
}

int main(int argc, const char* argv[])
{
    clock_t start_time = clock();
    if (3)
    {
        int sizeofarr = 0;
        int numberofarr = 0;
        int* polinom = read_polinom("in.txt", &sizeofarr, &numberofarr);
        int answer = calc_self_duality(sizeofarr, polinom, numberofarr);
        // calc_and_write_table(sizeofarr, polinom, numberofarr);
        write_to_file("out.txt", answer);
        free(polinom);
    }
    clock_t end_time = clock(); 
    double elapsed_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
     printf("Elapsed time: %f seconds\n", elapsed_time);
    return 0;
} 