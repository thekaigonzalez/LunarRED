/*Copyright 2019-2023 Kai D. Gonzalez*/

#ifndef LERR_H
#define LERR_H

// this is the start of a new, more flexible error system for LunarRed
// allows errors to be called on certain objects

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lunarred.h"
#include "lobj.h"
#include "lmem.h"

// standard UNIX ascii colors
#define RESET "\x1b[0m"
#define RED "\x1b[31;1m"
#define GREEN "\x1b[32;1m"
#define YELLOW "\x1b[33;1m"
#define BLUE "\x1b[34;1m"
#define MAGENTA "\x1b[35;1m"
#define CYAN "\x1b[36;1m"
#define WHITE "\x1b[37;1m"

typedef enum _Lunar_Error {
  LERR_UNDEFINED, // undefined value
  LERR_INVALID, // invalid value
  LERR_ANOTE, // a note
  LERR_WARNING, // warning
  LERR_SYNTAX, // syntax error
  LERR_SEMANTIC, // semantic error
  LERR_RUNTIME, // runtime error
  LERR_FATAL // fatal error
} Lunar_Error;

void lunar_Problem (Lunar_Object *obj, char *msg);

void lunar_Pretty (char *msg, Lunar_Error level);

#endif
