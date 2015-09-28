/*
  mem.h
*/

#pragma once
#include <stdint.h>

// Defined in the file mem.c
uint32_t* initiate_directory();
 void initiate_page_table(uint32_t* page_dir);
