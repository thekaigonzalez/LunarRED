#include "lgen.h"
#include "lerr.h"
#include "ltok.h"

#include <ctype.h>
#include <stdbool.h>

void
lunar_generate (lunar_CG *generator, Lunar_Array *tokens)
{
  _Bool subroutine = false;

  for (int i = 0; i < tokens->size; i++)
    {
      Lunar_Token *t = tokens->ptr[i];

      if (t->type == LCL_SUBROUTINE_HEADER && !subroutine)
        {
          if (generator->compiler != LUNAR_COMPILER_NEXFUSE
              && t->ptr[0] != 'M')
            {
              printf ("%sLR(generate): support error%s: subroutines are only "
                      "supported in NexFUSE.\n",
                      YELLOW, RESET);
              printf ("\t[C]: did you mean to use the -n flag?\n");
              return;
            }

          subroutine = true;
          byte hdr = t->ptr[0];

          if (hdr != 'M') // M is the signal for main, run all code at the base
            __sub (generator, hdr);
        }
      else if (t->type == LCL_SUBROUTINE_HEADER && subroutine)
        {
          if (generator->compiler != LUNAR_COMPILER_NEXFUSE
              && generator->compiler != LUNAR_COMPILER_BOTH)
            {
              printf ("%sLR(generate): support error%s: subroutines are only "
                      "supported in NexFUSE.\n",
                      YELLOW, RESET);
              printf ("\t[C]: did you mean to use the -n flag?\n");
              return;
            }
          __endsub (generator);
        }
      else if (t->type == LCL_CALL && subroutine)
        {
          lunar_cg_add (generator, std_map_search (t->ptr));
        }
      else
        {
          if (t->type != LCL_PARAM)
            {
              printf ("%sLR(generate): error%s: unexpected token: `%s'\n",
                      YELLOW, RESET, t->ptr);
              return;
            }

          lunar_cg_add (generator, lunar_asByte (t));
        }
    }
}



byte
lunar_asByte (Lunar_Token *t)
{
  if (t->ptr[0] == 'r' || t->ptr[0] == 'R')
    {
      char *rgnum = LMalloc (sizeof (char) * 10);

      strcpy (rgnum, t->ptr + 1);

      int n = atoi (rgnum);

      LFree (rgnum);

      return n;
    }
  if (t->ptr[0] == '0' && t->ptr[1] == 'x')
    {
      return strtol (t->ptr, NULL, 16);
    }
  else
    {
      if (lunar_lex_isnumeric (t->ptr))
        {
          return strtol (t->ptr, NULL, 10);
        }
    }
}
