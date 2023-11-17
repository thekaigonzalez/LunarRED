/*Copyright 2019-2023 Kai D. Gonzalez*/

#ifndef LLEX_H
#define LLEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lmem.h"
#include "lunarred.h"
#include "ltok.h"
#include "lbuf.h"

#define LUNAR_LEX_STATE_INITIAL 0 // start of any statements
#define LUNAR_LEX_STATE_IN_NUMBER 1 // [0-9]+
#define LUNAR_LEX_STATE_IN_CHAR_LITERAL 2 // '[...]'
#define LUNAR_LEX_STATE_GATHERING_SR 3 // SubRoutine @ [...]
#define LUNAR_LEX_STATE_GATHERING 4 // .[...]
#define LUNAR_LEX_STATE_NAME_TOPLINE 5 // top line name (.section [...], etc)
#define LUNAR_LEX_STATE_NAME_SUBROUTINE 6 // sub routine name @ [...
#define LUNAR_LEX_STATE_ARGUMENTS 7 // [call] [arguments]
#define LUNAR_LEX_STATE_IGNORANT 8 // general purpose ignorate state, like comments

// Lexer_State
typedef struct _Lexer_State
{
  int state; // current state
  Lunar_Buffer *buf; // buffer
  Lunar_Array *tokens ; // tokens
} Lexer_State;

Lexer_State *lunar_lexnew ();
void lunar_lex_append(Lexer_State *L, Lunar_Token *t);
Lunar_Token *lunar_token_at (Lexer_State *L, int index);
void lunar_lex_tokenize (Lexer_State *L, char* input);
void lunar_lex_free (Lexer_State *ls);

Lunar_Classification lunar_resolve_type (char *t);

int lunar_lex_isnumeric (char *ptr);
int lunar_lex_issymbol (char *ptr);
int lunar_lex_iswhitespace (char *ptr);
int lunar_lex_ispunct (char ptr);

void lunar_print_tokens (Lunar_Array *tokens);

char* lunar_stringify (Lunar_Classification t);

#endif
