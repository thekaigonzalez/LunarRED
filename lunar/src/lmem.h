/*Copyright 2019-2023 Kai D. Gonzalez*/

#ifndef FMEM_H
#define FMEM_H

#include "lunarred.h"

void* LMalloc (size_t size);
void* LRealloc (void *ptr, size_t size);
void LFree (void *ptr);

#endif
