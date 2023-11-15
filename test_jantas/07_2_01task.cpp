#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "os_mem.h"


int global_k = 0;
int exist = 0;
int global_size = 0;

int kecreate(int size, int num_pages)
{
	if (exist == 1)
	{
		return 0;
	}
	else
	{
		exist = 1;
		global_size = size;
		return 1;
	}
}


int kedestroy()
{
	if (exist == 1)
	{
		global_k = 0;
		exist = 0;
		global_size = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}


mem_handle_t kealloc(int block_size)
{
	mem_handle_t block = { 0, block_size };
	if (exist == 1 && (global_size - global_k) >= 32)
	{
		block.addr = global_k;
		global_k += 32;
		return block;
	}
	else
	{
		block = { 0, 0 };
		return block;
	}
}


int kefree(mem_handle_t h)
{
	if (h.addr == (global_k - 32) && h.size == 32 && exist == 1)
	{
		global_k -= 32;
		return 1;
	}
	else
	{
		return 0;
	}
}


mem_handle_t keget_block(int addr, int size)
{
	mem_handle_t block = { 0, 0 };
	if (addr % 32 == 0 && size == 32 && exist == 1)
	{
		block = { addr, size };
		return block;
	}
	else
	{
		return block;
	}
}


int keget_max_block_size()
{
	if (global_size - global_k >= 32 && exist == 1)
		return 32;
	else
		return 0;
}


int keget_free_space() 
{
	return global_size - global_k;
}


void keprint_blocks()
{
	if (exist == 1)
	{
		for (int i = 0; i < global_k; i += 32)
			printf("%d 32\n", i);
	}
}


void setup_memory_manager(memory_manager_t* mm)
{
	mm->create = kecreate;
	mm->destroy = kedestroy;
	mm->alloc = kealloc;
	mm->free = kefree;
	mm->get_block = keget_block;
	mm->get_max_block_size = keget_max_block_size;
	mm->get_free_space = keget_free_space;
	mm->print_blocks = keprint_blocks;
}
