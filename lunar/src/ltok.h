/*Copyright 2019-2023 Kai D. Gonzalez*/

// lunar/src/ltok.h
#ifndef LTOK_H
#define LTOK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lmem.h"
#include "lobj.h"
#include "lunarred.h"

#define LUNARRED_TOKEN_TAB '\t'
#define LUNARRED_TOKEN_NEWLINE '\n'
#define LUNARRED_TOKEN_SEP ','
#define LUNARRED_TOKEN_WHITESPACE ' '
#define LUNARRED_TOKEN_TOPLINE '.'
#define LUNARRED_TOKEN_SUBROUTINE '@'
#define LUNARRED_TOKEN_SEMICOLON ';'
#define LUNARRED_TOKEN_DEFINE ':'
#define LUNARRED_TOKEN_COMMENT '#' // yeah yeah boi we doin this ish different
#define LUNARRED_TOKEN_LIT '\''

// Lunar_Token
// Reinventing the wheel for the THIRD damn time
// except this time we're not moving memory and stuff
// and converting their types
typedef struct _Lunar_Token
{
  char *ptr;
  Lunar_Classification type;
} Lunar_Token;

typedef struct Lunar_Variant
{
  union
  {
    int integer;
    double floating_point;
    char *string;
    void *hexager;
    void *boolean;
    void *null;
  };

  Lunar_Classification type;
} Lunar_Variant;

Lunar_Token *lunar_token_new (char *ptr, Lunar_Classification type);
Lunar_Variant lunar_token_any (
    Lunar_Token *t); // creates and returns a pointer based on the type
void lunar_token_free (Lunar_Token *t);

#endif
