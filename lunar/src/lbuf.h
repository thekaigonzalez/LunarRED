/*Copyright 2019-2023 Kai D. Gonzalez*/

#ifndef LBUF_H
#define LBUF_H

#include "lmem.h"
#include "lobj.h"
#include "lunarred.h"

// Lunar_Buffer
// Allows code to store a list of characters
// Buffers are basically a list but for strings
// can return copied pointers
// can also return loose instances of 
typedef struct _Lunar_Buffer
{
  char *ptr;
  int size;
  int cap;
  int resets; // not that this is of use to anyone, but it's here for now
} Lunar_Buffer;

Lunar_Buffer *lunar_buffer_new ();
void lunar_buffer_append(Lunar_Buffer *b, char c);
void lunar_buffer_remove(Lunar_Buffer *b, int index);
void lunar_buffer_clear(Lunar_Buffer *b);
char *lunar_buffer_copy(Lunar_Buffer *b);

void lunar_buffer_free (Lunar_Buffer *b);

#endif
