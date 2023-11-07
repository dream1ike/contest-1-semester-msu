#include <stdio.h>
#include "os_mem.h"   
#include <stdlib.h>
#include <string.h>
#include <memory.h>

int global_size = 0; // ���������� ���������� - ������������ ���� ������ � ������� ����� ������������� ������

int global_Note_size = 0; // ���������� ����������, ���������� �� ����������� ������� �������� � ������ �����

typedef struct Node {
    int addr;
    int size;
}Node;

Node* array = NULL;

int my_create(int size, int num_pages) // �������� ��������� ������
{
    if (size > 0 && global_size == 0)
    {
        global_size = size;
        return 1;
    }
    return 0; // ����� ��������
}

void realloc_array() // ������� ������� ��������� ������ ��������, ����������� �� �������� ������ (�������� �� �������� ��)
{
    //array = (Node*)realloc(array, (global_Note_size) * sizeof(Node)); // ���������� ������, ����� ���������� //
    if (array == NULL)
    {
        array = (Node*)malloc(global_Note_size * sizeof(Node));
        return;
    }

    Node* tmp_array = (Node*)malloc(global_Note_size * sizeof(Node));

    for (int i = 0; i < global_Note_size; i++)
    {
        tmp_array[i].addr = array[i].addr;
        tmp_array[i].size = array[i].size;
    }

    free(array);

    array = (Node*)malloc(global_Note_size * sizeof(Node));

    for (int i = 0; i < global_Note_size; i++)
    {
        array[i].addr = tmp_array[i].addr;
        array[i].size = tmp_array[i].size;
    }

    free(tmp_array);
}

int my_destroy() // ������� �������� ������� �������� -> �������� ��������� ������
{
    if (global_size == 0) return 0;
    free(array);
    array = NULL;
    global_Note_size = 0;
    global_size = 0;
    return 1; // ����� ��������
}

int my_get_max_block_size() // ������� ���������� ������ �������� ���������� ����� (��������� � ����������� � alloc)
{
    int counter = 0, tmp = 0;
    if (global_Note_size > 0) {

        tmp = global_size - (array[global_Note_size - 1].addr + array[global_Note_size - 1].size);

        for (int i = 0; i < global_Note_size - 1; i++)
        {
            counter = (array[i].addr + array[i].size) - array[i + 1].addr;

            if (counter >= tmp)
            {
                tmp = counter;
            }
        }
        return tmp;
    }

    return global_size;
}

mem_handle_t my_alloc(int block_size) // ������� ���������� �����
{
    if (global_Note_size == 0)
    {
        global_Note_size++;
        realloc_array();
        array[0].addr = 0;
        array[0].size = block_size;
        return { array[0].addr ,array[0].size };
    }
    else
    {
        int counter = 0, tmp = 0, size_for_block, i = 0, result_addr, result_size;
        size_for_block = my_get_max_block_size();
        if (size_for_block >= block_size)
        {
            tmp = size_for_block;
            global_Note_size += 1;
            realloc_array();

            for (i = 0; i < global_Note_size - 2; i++)
            {
                counter = (array[i].addr + array[i].size) - array[i + 1].addr;

                if (counter >= tmp)
                {
                    tmp = counter;
                    if (tmp == size_for_block)
                    {
                        int k = i + 1, x = 1, y = 2;
                        for (k; k < global_Note_size - 1; k++)
                        {
                            array[global_Note_size - x].addr = array[global_Note_size - y].addr;
                            array[global_Note_size - x].size = array[global_Note_size - y].size;
                            x++;
                            y++;
                        }
                        array[i + 1].addr = array[i].addr + array[i].size + 1;
                        array[i + 1].size = block_size;
                        result_addr = array[i + 1].addr;
                        result_size = array[i + 1].size;
                        return { result_addr,result_size }; // ������ �����������
                    }
                }
            }
            array[global_Note_size - 1].addr = array[global_Note_size - 2].addr + array[global_Note_size - 2].size;
            array[global_Note_size - 1].size = block_size;
            return { array[global_Note_size - 1].addr,array[global_Note_size - 1].size };
        }
    }
    return{ 0,0 }; // ����� ��������
}

int my_get_free_space() // ������� ����������� ���� ���������� ������ � ������
{
    int counter = 0, tmp = 0;
    tmp = global_size - (array[global_Note_size - 1].addr + array[global_Note_size - 1].size + 1);
    for (int i = 0; i < global_Note_size - 1; i++)
    {
        counter = (array[i].addr + array[i].size) - array[i + 1].addr;
        tmp += counter;
    }
    return tmp;
}

void my_print_blocks() // ������� ������ ������ (���������������� � �������)
{
    for (int i = 0; i < global_Note_size; i++)
    {
        printf("%d %d", array[i].addr, array[i].size);
    }
}

int my_free(mem_handle_t h) // �������� ����� ������ � �������� ������� � ������ (��� ���� ��, �������� �������� �� �������, ����� �������, ������� �������)
{
    for (int i = 0; i < global_Note_size; i++)
    {
        if (array[i].addr == h.addr)
        {
            if (array[i].size == h.size)
            {
                for (i; i < global_Note_size - 1; i++)
                {
                    array[i].addr = array[i + 1].addr;
                    array[i].size = array[i + 1].size;
                }
                global_Note_size--;
                realloc_array();
                return 1;
            }
        }
    }
    return 0;
}

mem_handle_t my_get_block(int addr, int size)
{
    for (int i = 0; i < global_Note_size - 1; i++)
    {
        if (array[i].addr <= addr && array[i + 1].addr > addr)
        {
            if (array[i].size <= size) {
                return { array[i].addr, array[i].size };
            }
        }
    }
    return { 0,0 };
}

void setup_memory_manager(memory_manager_t* mm) {
    mm->create = my_create;
    mm->destroy = my_destroy;
    mm->alloc = my_alloc;
    mm->get_max_block_size = my_get_max_block_size;
    mm->get_free_space = my_get_free_space;
    mm->print_blocks = my_print_blocks;
    mm->free = my_free;
}

int main()
{
    int i = 0;
    my_create(160, 1);
    my_alloc(32);
    my_alloc(32);
    return 0;
}

//command: mm.create(160, 1); expected: 0, passed
//command : mm.alloc(32); expected: mem_handle_t(0, 32), passed
//command : mm.alloc(32); expected: mem_handle_t(32, 32),


//int i = 0;
//i = my_create(1003, 0);
//i = my_destroy();
//i = my_destroy();
//i = my_create(10, 0);
//my_alloc(3);
//my_alloc(4);
//my_alloc(3);
//my_alloc(4);
//i = my_destroy();
//i = my_create(15, 0);
//my_alloc(4);
//my_alloc(5);
//my_alloc(6);
//my_free({ 4,5 });
//my_alloc(3);