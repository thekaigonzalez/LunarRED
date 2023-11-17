/*Copyright 2019-2023 Kai D. Gonzalez*/

#ifndef LGEN_H
#define LGEN_H

// final LunarRED codegen
// Copyright 2019-2023 Kai D. Gonzalez
// LunarRed CodeGen

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lmem.h"
#include "lunarred.h"
#include "lstd.h"
#include "lcg.h"
#include "llex.h"

void lunar_generate(lunar_CG *generator, Lunar_Array * tokens);
byte lunar_asByte(Lunar_Token* f);

#endif
