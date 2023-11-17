#include "lbuf.h"
#include "lcl.h"
#include "llex.h"
#include "lmem.h"
#include "lobj.h"
#include "ltok.h"
#include "lunarred.h"
#include "lcg.h"
#include "lgen.h"

#include <assert.h>

#define PASSED printf ("function %s PASSED\n", __func__);

test_obj_str (void)
{
  Lunar_Object *obj = lunar_object_new ();

  char *n = LMalloc (sizeof (char) * 11);

  n[0] = 'a';
  n[1] = 'b';
  n[2] = 'c';
  n[3] = 'd';
  n[4] = 'e';
  n[5] = 'f';
  n[6] = 'g';
  n[7] = 'h';
  n[8] = 'i';
  n[9] = 'j';
  n[10] = '\0';

  lunar_object_set (obj, n, LCL_STRING);

  assert (obj->info == n);
  assert (obj->classification == LCL_STRING);
  assert (strcmp ((char *)obj->info, "abcdefghij") == 0);

  lunar_object_free (obj);
  PASSED;
}

test_obj_arr (void)
{
  Lunar_Array *arr = lunar_array_new ();

  lunar_array_push (arr, (void *)1);
  lunar_array_push (arr, (void *)2);
  lunar_array_push (arr, (void *)3);

  assert (arr->size == 3);
  assert (arr->cap == LUNARRED_MEMORY_INCREMENT);
  assert ((int)arr->ptr[0] == 1);
  assert ((int)arr->ptr[1] == 2);
  assert ((int)arr->ptr[2] == 3);

  // since there are no allocated blocks, we can't free it by freeing every
  // element
  lunar_array_free_dumb (arr);
  PASSED;
}

test_array_limit (void)
{
  Lunar_Array *arr = lunar_array_new ();

  for (int i = 0; i <= LUNARRED_OPENLUD_BYTE_LIMIT; i++)
    {
      lunar_array_push (arr, i);
    }

  assert (arr->size == LUNARRED_OPENLUD_BYTE_LIMIT + 1);
  assert (arr->ptr[21] == 21);
  assert (arr->ptr[256] == 256);

  lunar_array_free_dumb (arr);

  PASSED;
}

test_array_pop (void)
{
  Lunar_Array *arr = lunar_array_new ();

  lunar_array_push (arr, (void *)1);
  lunar_array_push (arr, (void *)2);
  lunar_array_push (arr, (void *)3);

  assert (arr->size == 3);
  assert (arr->cap == LUNARRED_MEMORY_INCREMENT);
  assert ((int)arr->ptr[0] == 1);
  assert ((int)arr->ptr[1] == 2);
  assert ((int)arr->ptr[2] == 3);

  void *popped = lunar_array_pop (arr);
  assert (arr->size == 2);
  assert (popped == (void *)3);

  lunar_array_free_dumb (arr);

  PASSED;
}

test_array_clear (void)
{
  Lunar_Array *arr = lunar_array_new ();

  lunar_array_push (arr, (void *)1);
  lunar_array_push (arr, (void *)2);
  lunar_array_push (arr, (void *)3);

  assert (arr->size == 3);
  assert (arr->cap == LUNARRED_MEMORY_INCREMENT);
  assert ((int)arr->ptr[0] == 1);
  assert ((int)arr->ptr[1] == 2);
  assert ((int)arr->ptr[2] == 3);

  // Clear the array
  lunar_array_clear (arr);

  // Verify that the array is now empty
  assert (arr->size == 0);
  assert (arr->cap == LUNARRED_MEMORY_INCREMENT);

  lunar_array_free_dumb (arr);

  PASSED;
}

test_array_same (void)
{
  Lunar_Array *arr = lunar_array_new ();

  lunar_array_push (arr, (void *)1);
  lunar_array_push (arr, (void *)2);
  lunar_array_push (arr, (void *)3);

  assert (arr->size == 3);

  Lunar_Array *arr2 = lunar_array_new ();

  lunar_array_push (arr2, (void *)1);
  lunar_array_push (arr2, (void *)2);
  lunar_array_push (arr2, (void *)3);

  assert (arr->size == 3);

  assert (lunar_array_same (arr, arr2));

  lunar_array_free_dumb (arr);
  lunar_array_free_dumb (arr2);
  PASSED;
}

test_buffer_limits (void)
{
  Lunar_Buffer *buf = lunar_buffer_new ();

  for (int i = 0; i <= LUNARRED_OPENLUD_BYTE_LIMIT + 500; i++)
    {
      // void* old_cap = buf->cap;
      lunar_buffer_append (buf, i);
      // if (old_cap != buf->cap) {
      // printf("buf->cap changed from %d to %d\n", old_cap, buf->cap);
      // }
    }

  assert (buf->size == LUNARRED_OPENLUD_BYTE_LIMIT + 501);

  assert (buf->ptr[0] == 0);

  lunar_buffer_free (buf);
  PASSED;
}

test_buffer_remove (void)
{
  Lunar_Buffer *buf = lunar_buffer_new ();

  // Test case 1: Remove character at index 0
  lunar_buffer_append (buf, 'a');
  lunar_buffer_append (buf, 'b');
  lunar_buffer_append (buf, 'c');
  lunar_buffer_remove (buf, 0);

  assert (buf->size == 2);

  assert (buf->ptr[0] == 'b');
  assert (buf->ptr[1] == 'c');

  lunar_buffer_clear (buf);

  // Test case 2: Remove character at index 1
  lunar_buffer_append (buf, 'd');
  lunar_buffer_append (buf, 'e');
  lunar_buffer_append (buf, 'f');
  lunar_buffer_remove (buf, 1);

  assert (buf->size == 2);
  assert (buf->ptr[0] == 'd');
  assert (buf->ptr[1] == 'f');

  // Test case 3: Remove character at index 2
  lunar_buffer_remove (buf, 2);
  assert (buf->size == 2);
  assert (buf->ptr[0] == 'd');
  assert (buf->ptr[1] == 'f');

  lunar_buffer_free (buf);
  PASSED;
}

test_token_cast (void)
{
  Lunar_Token *t = lunar_token_new ("30", LCL_INTEGER);

  assert (t->type == LCL_INTEGER);

  Lunar_Variant v = lunar_token_any (t);

  assert (v.type == LCL_INTEGER);
  assert (v.integer == 30);

  lunar_token_free (t);
  PASSED;
}

test_token_hex (void)
{
  Lunar_Token *t = lunar_token_new ("0x30", LCL_HEXAGER);

  assert (t->type == LCL_HEXAGER);
  Lunar_Variant v = lunar_token_any (t);

  assert (v.integer == 0x30);

  lunar_token_free (t);
  PASSED;
}

test_token_list (void)
{
  Lunar_Token *t = lunar_token_new ("30", LCL_INTEGER);
  Lunar_Token *t2 = lunar_token_new ("0x30", LCL_HEXAGER);
  Lunar_Token *t3 = lunar_token_new ("32", LCL_INTEGER);

  assert (t->type == LCL_INTEGER);

  Lunar_Variant v = lunar_token_any (t);

  assert (v.type == LCL_INTEGER);
  assert (v.integer == 30);

  Lexer_State *state = lunar_lexnew ();

  state->tokens->is_string_array = 0;

  lunar_lex_append (state, t);
  lunar_lex_append (state, t2);
  lunar_lex_append (state, t3);

  assert (state->tokens->size == 3);
  assert (lunar_token_at (state, 0)->type == LCL_INTEGER);
  assert (lunar_token_at (state, 1)->type == LCL_HEXAGER);
  assert (lunar_token_at (state, 2)->type == LCL_INTEGER);

  lunar_lex_free (state);

  PASSED;
}

test_lexer_tokenize (void)
{
  Lexer_State *state = lunar_lexnew ();

  FILE *f = fopen ("example.asm", "r");

  Lunar_Buffer *buf = lunar_buffer_new ();

  if (f == NULL)
    {
      return -1;
    }

  char c;

  while ((c = fgetc (f)) != EOF)
    {
      lunar_buffer_append (buf, c);
    }

  printf ("%s\n", buf->ptr);

  lunar_lex_tokenize (state, buf->ptr);

  lunar_print_tokens (state->tokens);

  lunar_lex_free (state);
  lunar_buffer_free (buf);

  fclose (f);

  PASSED;
}

test_numerics (void)
{
  assert (lunar_lex_isnumeric ("0") == 1);
  assert (lunar_lex_isnumeric ("0x10") == 1);
  assert (lunar_lex_isnumeric ("0x11") == 1);
  assert (lunar_lex_isnumeric ("123") == 1);

  // limits
  // TODO: fix these so that they are true
  // because what kind of assembler would it be if you COULDN't do this?
  assert (lunar_lex_isnumeric ("0xFF") == 0);
  assert (lunar_lex_isnumeric ("0b1010") == 0);
  assert (lunar_lex_isnumeric ("0o777") == 0);

  PASSED;
}

test_symbols (void)
{
  assert (lunar_lex_issymbol ("a") == 0);
  assert (lunar_lex_issymbol ("b") == 0);
  assert (lunar_lex_issymbol ("c") == 0);

  assert (lunar_lex_issymbol ("/") == 1);
  assert (lunar_lex_issymbol ("@") == 1);
  assert (lunar_lex_issymbol ("#") == 1);
  assert (lunar_lex_issymbol ("!") == 1);

  // strings of symbols
  assert (lunar_lex_issymbol ("*/@") == 1);

  PASSED;
}

test_whitespace (void)
{
  // this can only really go one way lol
  assert (lunar_lex_iswhitespace (" ") == 1);
  assert (lunar_lex_iswhitespace ("\t") == 1);
  assert (lunar_lex_iswhitespace ("                            ") == 1);

  // Additional whitespace tests
  assert (lunar_lex_iswhitespace ("") == 1);     // Empty string
  assert (lunar_lex_iswhitespace ("\n") == 1);   // Newline
  assert (lunar_lex_iswhitespace ("\r\n") == 1); // Windows-style newline
  assert (lunar_lex_iswhitespace (" \t ") == 1); // Whitespace with tabs
  assert (lunar_lex_iswhitespace ("\t\t") == 1); // Multiple tabs
  assert (lunar_lex_iswhitespace (" \n \t \r\n ") == 1); // Mixed whitespace

  PASSED;
}

test_lexer_parse (void)
{
  Lexer_State *state = lunar_lexnew ();

  FILE *f = fopen ("example.asm", "r");

  Lunar_Buffer *buf = lunar_buffer_new ();

  if (f == NULL)
    {
      return -1;
    }

  char c;

  while ((c = fgetc (f)) != EOF)
    {
      lunar_buffer_append (buf, c);
    }

  lunar_CG *cg = lunar_cg_init(LUNAR_COMPILER_NEXFUSE);

  lunar_lex_tokenize (state, buf->ptr);

  lunar_generate(cg, state->tokens);

  FILE * out = fopen ("example.bin", "wb");

  fwrite (cg->code, sizeof (byte), cg->size, out);

  fclose (out);

  lunar_lex_free (state);
  lunar_buffer_free (buf);
  lunar_cg_free(cg);

  fclose (f);

  PASSED;
}

test_codegen (void) {
  lunar_CG* cg = lunar_cg_init(LUNAR_COMPILER_BOTH);

  assert(cg != NULL);

  __echo(cg, 0x41);

  assert(cg->size == 3);

  lunar_cg_free(cg);
  PASSED;
}

main (void)
{
  test_obj_str ();
  test_obj_arr ();
  test_array_limit ();
  test_array_pop ();
  test_array_clear ();
  test_array_same ();
  test_buffer_limits ();
  test_buffer_remove ();
  test_token_cast ();
  test_token_hex ();
  test_token_list ();
  test_lexer_tokenize ();
  test_numerics ();
  test_symbols ();
  test_whitespace ();
  test_lexer_parse ();
  test_codegen ();
}
