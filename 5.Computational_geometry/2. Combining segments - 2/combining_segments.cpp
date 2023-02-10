#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#pragma warning(disable:4996)
#define eps 1e-10 
typedef struct {
    double x1;
    double x2;
}segment;

int checking_for_overlap(segment a, segment b)
{
    if (a.x1 <= b.x1  && b.x1 <= a.x2 || a.x1 <= b.x2 && b.x2 <= a.x2) return 1;
    return 0;
}

void bubble_sort(segment** arr, int size)
{
    segment* temp;
    for(int i = 0; i < size-1; i++)
    {
        for(int j = 0; j < size-1-i; j++)
        {
            if (arr[j+1] -> x1 < arr[j] -> x1)
            {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

segment* read_segments_from_file(const char* filename, int* size)
{
    FILE* fin = fopen(filename, "r");
    segment* arr = NULL;
    if(fin)
    {
        if (fscanf(fin, "%d ", size) > 0)
        {
            arr = (segment*)malloc(sizeof(segment) * (*size));
            if (arr)
            {
                int i = 0;
                while(fscanf(fin, "%lf", &arr[i].x1) > 0 && fscanf(fin, "%lf", &arr[i].x2) > 0) {
                    i++;
                }
                if (i < *size)
                {
                    *size = i;
                    segment* temp = (segment*)realloc(arr, i);
                    if (temp)
                    {
                        arr = temp;
                        free(temp);
                    }
                }
            }
        }
        fclose(fin);
    }
    return arr;
}

void write_segments_to_file (const char* filename, segment** arr, int begin, int end)
{
	FILE* fout = fopen(filename, "w");
    if(fout)
    {
        for(int i = begin; i <= end; i++) fprintf(fout, " {%g,%g} ", arr[i] -> x1, arr[i] -> x2);
        fclose(fout);
    }
}

double union_size(segment** seg_max, int begin, int end)
{
    double size = 0;
    double min_left = seg_max[begin] -> x1;
    for (int i = begin; i <= end; i++) if (min_left > seg_max[i] -> x1) min_left = seg_max[i] -> x1;
    double max_right = seg_max[begin] -> x2;
    for(int i = begin; i <= end; i++) if (max_right < seg_max[i] -> x2) max_right = seg_max[i] -> x2;
    size = max_right - min_left;
    return size;
}

segment** combining_segments (segment* arr, int size)
{
    segment** seg_max = NULL;
    seg_max = (segment**)malloc(sizeof(segment) * (size+1));
    if (seg_max)
    {
        for(int i = 0; i < size; i++) seg_max[i] = &arr[i];
        bubble_sort(seg_max, size);
        int j = 0;
        int max_lenght = 0;
        int index_begin;
        int index_end;
        for (int i = 0; i < size-1; i++)
        {
            for (j = i+1; checking_for_overlap(*seg_max[i], *seg_max[j]) > 0 && j < size; j++);
            j--;
            int temp = union_size(seg_max, i, j);
            if (max_lenght < temp)
            {
                max_lenght = temp;
                index_begin = i;
                index_end = j;
            }
            i = j;
        }
        printf("%d %d", index_begin, index_end);
    }
    return seg_max; // последний элемент в массиве - индекс нужного отрезка отрезков
}
// кароч тут я пытался сделать следущее
// во первых, записал в массив указателей на структуры адреса отрезков и в последующем отсортировал их
// после этого в цикле я проверял отрезки на пересечение (первый со всеми, второй со всеми кроме первого и тд)
// (см функцию checking_for_overlap)
// если не встречаю пересекающиеся отрезки, передаю индексы начала и конца наших пересекающихся отрезков в функицю
// которая определяет размер объединения
// далее сравниваю этот размер со всеми предыдущими, если он окажется самым большим, запоминаю индексы начала и конца моих отрезков
// далее я не написал так как слишком опустошен и бухой, но кароче задумка такая
// записываю эти индексы максимального объединения в конец моего массива адресов структур отрезков и передаю их куда то
// чтобы их отсортировали и вернули в том виде, в котором они записывались изначально (это возможно благодаря арифметике указателей)
// кстати вот 132 строчка подумай, может можно как то без сортировки это все дело решить итак просто программа тяжелая
// далее передаю индексы начала и конца объединения(которые уже поставили на свои места и отсортировали) чтобы их записали в файл
// я ебал братишка я так ненавижу эту жизнь ты бы знал типа у меня нет никакого стимула идти дальше и все остальное меня уже не вдохновляет
// меня не вдохновляют ни заоблачные зарплаты, которые я буду получать в будущем, ни развитое мышление, которое я получу, если буду учиться
// все что я сейчас хочу это умереть. тихо спокойно и больше никогда ничего не чувствовать
// ведь когда я чувствую радость, я понимаю, что она закончится, когда я чувствую грусть, я понимаю, что скоро почувствую радость 
// и так по кругу. я потерял смысл жить, учиться, работать и самое страшное
// никто мне его не сможет вернуть.
int main()
{
    int size;
    segment* arr = read_segments_from_file("in.txt", &size);
    if(arr)
    {
        segment** arr_segments = combining_segments(arr, size);
        // for(int i = arr_segments[size] -> x1; i <= arr_segments[size] -> x2; i++) printf(" {%g, %g} ", arr_segments[i] -> x1, arr_segments[i] -> x2);
        write_segments_to_file("out.txt", arr_segments, 0, size-1);
        printf("\n\n %d ", checking_for_overlap(*arr_segments[2], *arr_segments[3]));
        free(arr);
        free(arr_segments);
    }
    return 0;
}
