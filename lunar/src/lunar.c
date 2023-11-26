#include "lunar.h"


#include <libgen.h>
#include <stdbool.h>
#include <getopt.h>
#include <argw.h>

int
pmain (int argc, char *argv[])
{
  _Bool nexfuse = false;
  _Bool openlud = false;

  char *outfile = "a.out";

  int optiond;

  char* filename = "";

  char c;

  argw_init();

  argw_flag('n', "nexfuse", "NexFuse compiler", WBoolean);
  argw_flag('l', "openlud", "OpenLUD compiler", WBoolean);
  argw_flag('o', "output", "Output file", WString);
  argw_flag('v', "version", "Print version", WBoolean);

  argw_parse(argc, argv);

  filename = argw_positional(0);
  outfile = argw_str('o');

  nexfuse = argw_bool('n');
  openlud = argw_bool('l');
  int version = argw_bool('v');
if (version) {
    printf("lunar version: %s\n", LUNARRED_VERSION_LONG);
    argw_exit(0);
  }
  if (wArgParserHelpWanted (parser) || wArgParserError (parser) || filename == NULL)
    {
      argw_usage("[-nlo] <filename>");
      argw_exit(0);
    }

  Lunar_Compiler cz = LUNAR_COMPILER_NEXFUSE;

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

  FILE *out = fopen (outfile, "wb");

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
