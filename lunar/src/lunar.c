#include "lunar.h"

#include <libgen.h>
#include <stdbool.h>

int
pmain (int argc, char *argv[])
{
  _Bool nexfuse = false;
  _Bool openlud = false;

  if (argc != 3)
    {
      printf ("usage: lunar <filename> <compiler>\n");
      return 0;
    }

  char *filename = argv[1];
  char *compiler = argv[2];

  if (strcmp (compiler, "--nexfuse") == 0)
    {
      nexfuse = true;
    }
  else if (strcmp (compiler, "--openlud") == 0)
    {
      openlud = true;
    }
  else if (strcmp (compiler, "--both") == 0)
    {
      nexfuse = true;
      openlud = true;
    }



  char c;

  Lunar_Compiler cz = LUNAR_COMPILER_NEXFUSE;

  if (nexfuse && !openlud)
    {
      cz = LUNAR_COMPILER_NEXFUSE;
    }
  else if (openlud && !nexfuse)
    {
      cz = LUNAR_COMPILER_OPENLUD;
    }
  else if (nexfuse && openlud)
    {
      cz = LUNAR_COMPILER_BOTH;
    }

  Lexer_State *state = lunar_lexnew ();
  Lunar_Buffer *buf = lunar_buffer_new ();
  lunar_CG *cg = lunar_cg_init (cz);

  FILE *f = fopen (filename, "r");
    if (f == NULL)
    {
      printf ("could not open file: `%s'\n", filename);
      return -1;
    }
  while ((c = fgetc (f)) != EOF)
    {
      lunar_buffer_append (buf, c);
    }


  lunar_lex_tokenize (state, buf->ptr);
  lunar_generate (cg, state->tokens);

  FILE *out = fopen ("a.out", "wb");
  fwrite (cg->code, sizeof (byte), cg->size, out);
  fclose (out);
  fclose (f);
  lunar_lex_free (state);
  lunar_buffer_free (buf);
  lunar_cg_free (cg);

  return 0;
}

int
main (int argc, char *argv[])
{
  return pmain (argc, argv);
}
