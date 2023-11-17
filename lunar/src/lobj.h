#ifndef LOBJ_H
#define LOBJ_H

// LunarRed General Purpose Object
// Copyright 2019-2023 Kai D. Gonzalez

#include "lcl.h"
#include "lmem.h"
#include "lunarred.h"

// Lunar_Object
// Allows code to store arbitrary information
// certain functions are able to grow and shrink Lunar_Objects as long as
// they're valid
// also know that there's like 10 instances of this same exact FUCKING class
// that all do the same thing. i dont get it but who cares. it works and there's
// no leaks
typedef struct _Lunar_Object
{
  void *info;                          // pointer to info
  Lunar_Classification classification; // classification
} Lunar_Object;

// Lunar_Array
// can store allocated pointers
typedef struct _Lunar_Array
{
  void **ptr;
  int size;
  int cap;
  int is_string_array; // if this is a string array (set this to prevent memory leaks)
} Lunar_Array;

Lunar_Object *lunar_object_new ();
Lunar_Array *lunar_array_new ();

void lunar_object_set (Lunar_Object *n, void *info,
                       Lunar_Classification classification);
void *lunar_object_get (Lunar_Object *n);
void lunar_object_classify (Lunar_Object *n,
                            Lunar_Classification classification);
void lunar_object_free (Lunar_Object *n);

void lunar_array_push (Lunar_Array *a, void *info);
void *lunar_array_pop (Lunar_Array *a);
void *lunar_array_peek (Lunar_Array *a);

Lunar_Array *lunar_array_subset (Lunar_Array *a, int start, int end);
Lunar_Array *lunar_array_copy (Lunar_Array *a);

int lunar_array_same (Lunar_Array *a, Lunar_Array *b);

void lunar_array_clear (Lunar_Array *a);
void lunar_array_free (Lunar_Array *a);
void lunar_array_free_dumb (Lunar_Array *a);

#endif
