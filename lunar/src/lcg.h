/*Copyright 2019-2023 Kai D. Gonzalez*/

#ifndef LCG_H
#define LCG_H

// LunarCG header file
// Copyright 2019-2023 Kai D. Gonzalez
// LunarRed CodeGen

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lmem.h"
#include "lunarred.h"

typedef enum Lunar_Compiler
{
  LUNAR_COMPILER_OPENLUD,
  LUNAR_COMPILER_NEXFUSE,
  LUNAR_COMPILER_BOTH
} Lunar_Compiler;

// Lunar Codegen is modular, meaning that you have a strict list of bytecode
// and simply add to it using the different kinds of standard functions
// provided in this header file
typedef struct lunar_CG
{
  byte *code;
  int size;
  Lunar_Compiler compiler;
  int cap;
} lunar_CG;

lunar_CG *lunar_cg_init (Lunar_Compiler compiler);
void lunar_cg_add (lunar_CG *cg, byte b);
void lunar_cg_free (lunar_CG *cg);

// Bytecode Intermediate

// 00
void __nnull (lunar_CG *cg);
// 40
void __echo (lunar_CG *cg, byte __N);
// 41
void __move (lunar_CG *cg, byte __R, byte __N);
// 42
void __each (lunar_CG *cg, byte __R);
// 43
void __reset (lunar_CG *cg);
// 44
void __clear (lunar_CG *cg, byte __R);
// 45
void __put (lunar_CG *cg, byte __R, byte __C, byte __P);
// 46
void __get (lunar_CG *cg, byte __R, byte __C, byte __O);

// NexFUSE-only

// 10
void __sub (lunar_CG *cg, byte __N);
// 11
void __endsub (lunar_CG *cg);
// 15
void __call (lunar_CG *cg, byte __N);

// 70
void __initsec (lunar_CG *cg, byte __S);

// OpenLUD & NexFUSE

// 100
void __init (lunar_CG *cg, byte __R);
// 22
void __end (lunar_CG *cg);

#endif
