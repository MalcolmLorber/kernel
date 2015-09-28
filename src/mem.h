/*
  mem.h
*/

#pragma once
#include <stdint.h>

// These two are defined in the assembly file paging.asm
extern void loadPageDirectory(uint32_t*);
extern void enablePaging();

// Defined in the file mem.c
uint32_t* initiate_directory();
 void initiate_page_table(uint32_t* page_dir);
