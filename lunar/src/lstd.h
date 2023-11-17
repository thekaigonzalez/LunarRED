/*Copyright 2019-2023 Kai D. Gonzalez*/

#ifndef LSTD_H
#define LSTD_H

#include "lunarred.h"

typedef struct Lunar_ByteFunction
{
  char* name;
  byte value;
} Lunar_ByteFunction;

extern Lunar_ByteFunction map_defaults[];

byte std_map_search (char *name);

#endif
