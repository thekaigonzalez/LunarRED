#include "lcg.h"
/*Copyright 2019-2023 Kai D. Gonzalez*/
lunar_CG *
lunar_cg_init (Lunar_Compiler compiler)
{
  lunar_CG *cg = LMalloc (sizeof (lunar_CG));

  if (cg == NULL)
    {
      printf ("warning: out of memory\n");
      return NULL;
    }

  cg->code = LMalloc (sizeof (byte) * LUNARRED_MEMORY_INCREMENT);

  if (cg->code == NULL)
    {
      printf ("warning: out of memory\n");
      LFree (cg);
    }

  cg->size = 0;
  cg->cap = LUNARRED_MEMORY_INCREMENT;
  cg->compiler = compiler;
}

void
lunar_cg_add (lunar_CG *cg, byte b)
{
  if (cg == NULL)
    {
      return;
    }

  if (cg->size == cg->cap)
    {
      cg->cap += LUNARRED_MEMORY_INCREMENT;
      cg->code = LRealloc (cg->code, cg->cap * sizeof (byte));
    }

  cg->code[cg->size] = b;
  cg->size++;
}

void
lunar_cg_free (lunar_CG *cg)
{
  LFree (cg->code);
  LFree (cg);
}

// lots. and i mean LOTS of boilerplate
// see https://github.com/thekaigonzalez/nexfuse/blob/main/doc/fuse.pdf for
// more information on what these functions add to codegen

void
__nnull (lunar_CG *cg)
{
  lunar_cg_add (cg, 00);
}

void
__echo (lunar_CG *cg, byte __N)
{
  lunar_cg_add (cg, 40);
  lunar_cg_add (cg, __N);
  __nnull (cg);
}

void
__move (lunar_CG *cg, byte __R, byte __N)
{
  lunar_cg_add (cg, 41);
  lunar_cg_add (cg, __R);
  lunar_cg_add (cg, __N);
  __nnull (cg);
}

void
__each (lunar_CG *cg, byte __R)
{
  lunar_cg_add (cg, 42);
  lunar_cg_add (cg, __R);
  __nnull (cg);
}

void
__reset (lunar_CG *cg)
{
  lunar_cg_add (cg, 43);
  __nnull (cg);
}

void
__clear (lunar_CG *cg, byte __R)
{
  lunar_cg_add (cg, 44);
  lunar_cg_add (cg, __R);
  __nnull (cg);
}

void
__put (lunar_CG *cg, byte __R, byte __C, byte __P)
{
  lunar_cg_add (cg, 45);
  lunar_cg_add (cg, __R);
  lunar_cg_add (cg, __C);
  lunar_cg_add (cg, __P);
  __nnull (cg);
}

void
__get (lunar_CG *cg, byte __R, byte __C, byte __O)
{
  lunar_cg_add (cg, 46);
  lunar_cg_add (cg, __R);
  lunar_cg_add (cg, __C);
  lunar_cg_add (cg, __O);
  __nnull (cg);
}

void
__sub (lunar_CG *cg, byte __N)
{
  lunar_cg_add (cg, 10);
  lunar_cg_add (cg, __N);
}

void
__endsub (lunar_CG *cg) // NOTE: automatically places END
{
  __end (cg);
  lunar_cg_add (cg, 11); // ENDSUB
  __nnull (cg);
}

void
__call (lunar_CG *cg, byte __N)
{
  lunar_cg_add (cg, (byte)15);
  lunar_cg_add (cg, __N);
  __nnull (cg);
}

void
__initsec (lunar_CG *cg, byte __S)
{
  lunar_cg_add (cg, 70);
  lunar_cg_add (cg, __S);
  __nnull (cg);
}

void
__init (lunar_CG *cg, byte __R)
{
  lunar_cg_add (cg, 100);
  lunar_cg_add (cg, __R);
  __nnull (cg);
}

void
__end (lunar_CG *cg)
{
  lunar_cg_add (cg, 22);
}
