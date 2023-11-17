/*Copyright 2019-2023 Kai D. Gonzalez*/

#ifndef FMEM_H
#define FMEM_H

#include "lunarred.h"

#define LMalloc(x) malloc(x)
#define LRealloc(x, y) realloc(x, y)
#define LFree(x) free(x)

#endif
