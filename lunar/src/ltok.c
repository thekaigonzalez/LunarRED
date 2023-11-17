#include "ltok.h"

Lunar_Token *
lunar_token_new (char *ptr, Lunar_Classification type)
{
  Lunar_Token *new = LMalloc (sizeof (Lunar_Token));

  new->ptr = ptr;
  new->type = type;

  return new;
}

Lunar_Variant
lunar_token_any (Lunar_Token *t)
{
  Lunar_Variant v;

  if (t == NULL || t->ptr == NULL)
    {
      v.type = LCL_UNDEFINED;
      v.null = NULL;
      return v;
    }

  switch (t->type)
    {
    case LCL_INTEGER:
      v.type = LCL_INTEGER;
      v.integer = atoi (t->ptr);
      return v;
    case LCL_FLOAT:
      v.type = LCL_FLOAT;
      v.floating_point = atof (t->ptr);
      return v;
    case LCL_STRING:
      v.type = LCL_STRING;
      v.string = t->ptr;
      return v;
    case LCL_HEXAGER:
      v.type = LCL_INTEGER;
      v.integer = strtol (t->ptr, NULL, 16);
      return v;
    case LCL_BOOLEAN:
      v.type = LCL_INTEGER;
      v.integer = (strcmp (t->ptr, "true") == 0) ? 1 : 0;
      return v;
    case LCL_NULL:
      v.type = LCL_UNDEFINED;
      v.null = NULL;
      return v;
    }

  v.type = LCL_UNDEFINED;
  v.null = NULL;
  return v;
}

void
lunar_token_free (Lunar_Token *t)
{
  LFree (t);
}
