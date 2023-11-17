/*Copyright 2019-2023 Kai D. Gonzalez*/

#ifndef LCL_H
#define LCL_H

typedef enum _Lunar_Classification {
  LCL_UNDEFINED, // undefined value
  LCL_INTEGER, // integer
  LCL_FLOAT, // floating point
  LCL_STRING, // string
  LCL_HEXAGER, // hex number (0x42)
  LCL_TOPLEVEL, // top level (.section [...], etc)
  LCL_BOOLEAN, // boolean
  LCL_SYMBOL, // symbol
  LCL_IDENTIFIER, // identifier (.section [...], etc)
  LCL_CALL, // call
  LCL_PARAM, // parameters
  LCL_SUBROUTINE_HEADER, // sub routine header
  LCL_NOTHING, // nothing
  LCL_NULL, // null
} Lunar_Classification;

#endif
