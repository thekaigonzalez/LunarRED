#include "lunar.h"

#include <libgen.h>
#include <stdbool.h>
#include <getopt.h>

int
pmain (int argc, char *argv[])
{
  _Bool nexfuse = false;
  _Bool openlud = false;

  char *outfile = "a.out";

  int optiond;
  if (argc != 3)
    {
      printf ("usage: %s <filename> [options]\n", argv[0]);
      return 0;
    }

  // Define the options
  struct option long_options[] = { { "nexfuse", no_argument, 0, 'n' },
                                   { "openlud", no_argument, 0, 'o' },
                                   { "both", no_argument, 0, 'b' },
                                   { "output", required_argument, 0, 'f' },
                                   { 0, 0, 0, 0 } };

  while ((optiond = getopt_long (argc, argv, "no:bf", long_options, NULL)) != -1)
    {
      switch (optiond)
        {
        case 'n':
          nexfuse = true;
          break;
        case 'o':
          openlud = true;
          break;
        case 'b':
          nexfuse = true;
          openlud = true;
          break;
        case 'f':
          outfile = optarg;
          break;
        default:
          printf ("usage: %s <filename> [options]\n", argv[0]);
          return 1;
        }
    }

  char *filename = argv[optind];

  if (filename == NULL)
    {
      printf ("usage: %s <filename> [options]\n", argv[0]);
      return 1;
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

  if (!state || !buf || !cg)
    {
      printf (
          "lunar: could not reserve space for codegen, buffer, or state\n");
      return -1;
    }

  FILE *f = fopen (filename, "r");
  if (f == NULL)
    {
      printf ("lunar: could not open file: `%s'\n", filename);
      return -1;
    }
  while ((c = fgetc (f)) != EOF)
    {
      lunar_buffer_append (buf, c);
    }

  lunar_lex_tokenize (state, buf->ptr);
  lunar_generate (cg, state->tokens);

  FILE *out = fopen ("a.out", "wb");

  if (out == NULL)
    {
      printf ("lunar: could not open file: `a.out'\n");
      return -1;
    }

  fwrite (cg->code, sizeof (byte), cg->size * sizeof (byte), out);

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
