#include "llex.h"
#include "lerr.h"

#include <ctype.h>

Lexer_State *
lunar_lexnew ()
{
  Lexer_State *ls = LMalloc (sizeof (Lexer_State));

  if (ls == NULL)
    {
      return NULL;
    }

  ls->state = LUNAR_LEX_STATE_INITIAL;
  ls->buf = lunar_buffer_new ();
  ls->tokens = lunar_array_new ();
  ls->tokens->is_string_array = 1;

  if (ls->tokens == NULL)
    {
      lunar_lex_free (ls);
      return NULL; // out of memory
    }

  return ls;
}

void
lunar_lex_append (Lexer_State *L, Lunar_Token *t)
{
  if (L == NULL)
    {
      return;
    }

  lunar_array_push (L->tokens, t);
}

Lunar_Token *
lunar_token_at (Lexer_State *L, int index)
{
  if (L == NULL)
    {
      return NULL;
    }

  if (index < 0 || index >= L->tokens->size)
    {
      return NULL;
    }

  return L->tokens->ptr[index];
}

void
lunar_lex_tokenize (Lexer_State *L, char *input)
{
  int i = 1;
  int len = strlen (input);

  int row = 0;
  int col = 0;

  if (L == NULL)
    {
      return;
    }

  L->state = LUNAR_LEX_STATE_INITIAL;
  int previous_state = L->state;
  lunar_buffer_clear (L->buf);

  while (*input)
    {
      if ((*input == LUNARRED_TOKEN_WHITESPACE
           && L->state == LUNAR_LEX_STATE_GATHERING)
          || (*input == LUNARRED_TOKEN_WHITESPACE && i == len
              && L->state == LUNAR_LEX_STATE_GATHERING)) // .[...]
        {
          if (L->buf->size > 0)
            {
              char *z = lunar_buffer_copy (L->buf);
              Lunar_Token *t = lunar_token_new (z, lunar_resolve_type (z));
              lunar_lex_append (L, t);
              lunar_buffer_clear (L->buf);
              L->state = LUNAR_LEX_STATE_NAME_TOPLINE;
            }
        }
      else if (i == len || *input == LUNARRED_TOKEN_NEWLINE
               || *input == LUNARRED_TOKEN_SEMICOLON
                      && L->state
                             != LUNAR_LEX_STATE_IN_CHAR_LITERAL) // end clauses
        {
          if (!isspace (*input))
            lunar_buffer_append (L->buf, *input);

          if (L->buf->size <= 0)
            {
              *input++;
              i++;
              continue;
            }

          switch (L->state)
            {
            case LUNAR_LEX_STATE_NAME_TOPLINE:
              {
                char *z = lunar_buffer_copy (L->buf);

                Lunar_Token *t = lunar_token_new (z, LCL_IDENTIFIER);

                lunar_lex_append (L, t);
                lunar_buffer_clear (L->buf);
              }
              break;
            case LUNAR_LEX_STATE_ARGUMENTS:
              {
                char *z = lunar_buffer_copy (L->buf);

                Lunar_Token *t = lunar_token_new (z, LCL_PARAM);

                lunar_lex_append (L, t);

                lunar_buffer_clear (L->buf);
              }
              break;
            case LUNAR_LEX_STATE_INITIAL:
              {
                char *z = lunar_buffer_copy (L->buf);

                Lunar_Token *t = lunar_token_new (
                    z, LCL_CALL); // by default assume its a call

                lunar_lex_append (L, t);

                lunar_buffer_clear (L->buf);
              }
              break;
            case LUNAR_LEX_STATE_IGNORANT:
              {
                lunar_buffer_clear (L->buf);
                L->state = previous_state;
              }

            default:
              if (L->state != LUNAR_LEX_STATE_INITIAL || L->buf->size > 0)
                {
                  printf ("%serror:%s state is unhandled\n", RED, RESET);
                  printf ("useful information:\n");
                  printf ("\t[C]: near \033[0;32m`%s'\033[0m\n",
                          (L->buf->size > 0) ? L->buf->ptr : "<empty>");

                  return;
                }
              break;
            }

          L->state = LUNAR_LEX_STATE_INITIAL;
        }
      else if (*input == LUNARRED_TOKEN_COMMENT
               && L->state != LUNAR_LEX_STATE_IGNORANT)
        {
          previous_state = L->state;

          L->state = LUNAR_LEX_STATE_IGNORANT;
        }
      else if (*input == LUNARRED_TOKEN_WHITESPACE
               && L->state == LUNAR_LEX_STATE_INITIAL) // prob a function name
        {
          if (L->buf->size <= 0)
            {
              *input++;
              i++;
              continue;
            }

          char *z = lunar_buffer_copy (L->buf);

          Lunar_Token *t = lunar_token_new (z, LCL_CALL);

          lunar_lex_append (L, t);
          lunar_buffer_clear (L->buf);

          L->state = LUNAR_LEX_STATE_ARGUMENTS;
        }

      else if ((*input == LUNARRED_TOKEN_SEP || i == len)
               && L->state == LUNAR_LEX_STATE_ARGUMENTS) // arguments (,) (e.g.
                                                         // push eax,ebx)
        {
          if (L->buf->size <= 0)
            {
              *input++;
              i++;
              continue;
            }

          char *z = lunar_buffer_copy (L->buf);

          Lunar_Token *t = lunar_token_new (z, LCL_PARAM);

          lunar_lex_append (L, t);

          lunar_buffer_clear (L->buf);
        }
      else if ((*input == LUNARRED_TOKEN_SUBROUTINE)
               && L->state
                      == LUNAR_LEX_STATE_INITIAL) // just a subroutine (@[...])
        {
          L->state = LUNAR_LEX_STATE_GATHERING_SR;
        }

      else if ((*input == LUNARRED_TOKEN_DEFINE)
               && L->state == LUNAR_LEX_STATE_GATHERING_SR) // @[name]:
        {
          char *z = lunar_buffer_copy (L->buf);

          Lunar_Token *t = lunar_token_new (z, LCL_SUBROUTINE_HEADER);

          lunar_lex_append (L, t);

          lunar_buffer_clear (L->buf);
          L->state = LUNAR_LEX_STATE_INITIAL;
        }

      else
        {
          if (*input == '.' && L->state == LUNAR_LEX_STATE_INITIAL)
            {
              L->state = LUNAR_LEX_STATE_GATHERING;
              lunar_buffer_append (L->buf, *input);
            }
          else if (!(isspace (*input))
                   && L->state != LUNAR_LEX_STATE_IN_CHAR_LITERAL
                   && !ispunct (*input)) // if theres no sort of state going
                                         // on, and it's not a symbol
            {
              lunar_buffer_append (L->buf, *input);
            }
          else
            {
              if (L->state == LUNAR_LEX_STATE_IGNORANT) {
                *input++;
                i++;
                continue;
              }
              if (ispunct (*input)
                  && (L->state != LUNAR_LEX_STATE_IN_CHAR_LITERAL)
                  && (L->state != LUNAR_LEX_STATE_GATHERING)
                  && (L->state != LUNAR_LEX_STATE_GATHERING_SR)
                  && *input != '.' && *input != '_')
                {
                  printf ("%serror:%s undefined token '%c'\n", RED, RESET,
                          *input);
                  printf ("useful information:\n");
                  printf ("\t[C]: near \033[0;32m`%s'\033[0m\n",
                          (L->buf->size > 0) ? L->buf->ptr : "<empty>");
                  return;
                }
              else
                {
                  if (ispunct (*input))
                    {
                      lunar_buffer_append (L->buf, *input);
                    }
                }
            }
        }

      *input++;
      i++;
    }
}

void
lunar_lex_free (Lexer_State *ls)
{
  if (ls == NULL)
    {
      return;
    }

  lunar_buffer_free (ls->buf);
  lunar_array_free (ls->tokens);
  LFree (ls);
}

Lunar_Classification
lunar_resolve_type (char *t)
{
  if (lunar_lex_isnumeric (t))
    {
      return LCL_INTEGER;
    }

  else if (lunar_lex_issymbol (t))
    {
      return LCL_SYMBOL;
    }

  else if (lunar_lex_iswhitespace (t))
    {
      return LCL_NOTHING;
    }

  else if (t[0] == LUNARRED_TOKEN_TOPLINE)
    {
      return LCL_TOPLEVEL;
    }

  else
    {
      return LCL_STRING;
    }
}

int
lunar_lex_isnumeric (char *ptr)
{
  if (ptr == NULL)
    {
      return 0;
    }
  while (*ptr != '\0')
    {
      char c = *ptr;
      char prev = *(ptr - 1);
      if (!isdigit (c) && !(c == 'x' && prev == '0'))
        {
          return 0;
        }
      *ptr++;
    }
  return 1;
}

int
lunar_lex_issymbol (char *ptr)
{
  while (*ptr != '\0')
    {
      char c = *ptr;
      if (!ispunct (c))
        {
          return 0;
        }
      *ptr++;
    }
  return 1;
}

int
lunar_lex_iswhitespace (char *ptr)
{
  while (*ptr != '\0')
    {
      char c = *ptr;

      if (!isspace (c))
        {
          return 0;
        }

      *ptr++;
    }

  return 1;
}

int
lunar_lex_ispunct (char ptr)
{
  if (ispunct (ptr))
    {
      if (ptr != '.' && ptr != '@' && ptr != ':' && ptr != ',' && ptr != ';'
          && ptr != '_')
        {
          return 1;
        }
    }
  return 0;
}

void
lunar_print_tokens (Lunar_Array *tokens)
{
  if (tokens == NULL)
    {
      return;
    }

  if (tokens->size == 0)
    {
      printf ("TOKENS: EMPTY\n");
      return;
    }

  if (tokens->size < 0)
    {
      printf ("TOKENS: NEGATIVE\n");
      return;
    }
  for (int i = 0; i < tokens->size; i++)
    {
      Lunar_Token *t = tokens->ptr[i];

      if (t == NULL)
        {
          printf ("TOKEN: NULL\n");
        }
      else
        {
          printf ("TOKEN(%s, %s)\n", t->ptr, lunar_stringify (t->type));
        }
    }
}

char *
lunar_stringify (Lunar_Classification t)
{
  switch (t)
    {
    case LCL_UNDEFINED:
      return "LCL_UNDEFINED";
    case LCL_INTEGER:
      return "LCL_INTEGER";
    case LCL_FLOAT:
      return "LCL_FLOAT";
    case LCL_STRING:
      return "LCL_STRING";
    case LCL_HEXAGER:
      return "LCL_HEXAGER";
    case LCL_TOPLEVEL:
      return "LCL_TOPLEVEL";
    case LCL_BOOLEAN:
      return "LCL_BOOLEAN";
    case LCL_SYMBOL:
      return "LCL_SYMBOL";
    case LCL_NOTHING:
      return "LCL_NOTHING";
    case LCL_NULL:
      return "LCL_NULL";
    case LCL_IDENTIFIER:
      return "LCL_IDENTIFIER";
    case LCL_CALL:
      return "<call>";
    case LCL_PARAM:
      return "LCL_PARAM";
    case LCL_SUBROUTINE_HEADER:
      return "LCL_SUBROUTINE_HEADER";
    default:
      return "Unknown Type";
    }
}
