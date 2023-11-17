#include "lobj.h"
#include "ltok.h"
#include "lbuf.h"

Lunar_Object *
lunar_object_new ()
{
  Lunar_Object *n = LMalloc(sizeof (Lunar_Object));

  if (n == NULL)
    {
      return NULL;
    }

  n->info = NULL;
  n->classification = LCL_UNDEFINED;

  return n;
}

Lunar_Array *
lunar_array_new ()
{
  Lunar_Array *n = LMalloc(sizeof (Lunar_Array));

  if (n == NULL)
    {
      return NULL;
    }

  n->ptr = LMalloc(sizeof (void *) * LUNARRED_MEMORY_INCREMENT);
  n->size = 0;
  n->cap = LUNARRED_MEMORY_INCREMENT;

  return n;
}

void
lunar_object_set (Lunar_Object *n, void *info,
                  Lunar_Classification classification)
{
  if (n == NULL)
    {
      return;
    }
  n->info = info;
  n->classification = classification;
}

void
lunar_object_classify (Lunar_Object *n, Lunar_Classification classification)
{
  if (n == NULL)
    {
      return;
    }
  n->classification = classification;
}

void *
lunar_object_get (Lunar_Object *n)
{
  if (n == NULL)
    {
      return NULL;
    }
  return n->info;
}

void
lunar_object_free (Lunar_Object *n)
{
  if (n == NULL)
    {
      return;
    }
  if (n->info != NULL)
    {
      free (n->info);
    }

  free (n);
}

void
lunar_array_push (Lunar_Array *a, void *info)
{
  if (a == NULL)
    {
      return; // TODO: more robust error handling
    }

  if (a->size >= a->cap)
    {
      a->cap += LUNARRED_MEMORY_INCREMENT;
      a->ptr = LRealloc (a->ptr, a->cap * sizeof (void *));
    }

  a->ptr[a->size] = info;
  a->size++;
}

void *
lunar_array_pop (Lunar_Array *a)
{
  // get the last element and remove it

  if (a == NULL)
    {
      return NULL;
    }

  if (a->size == 0)
    {
      return NULL;
    }

  a->size--;
  void *s = a->ptr[a->size];

  memset (a->ptr + a->size, 0, sizeof (void *));

  return s;
}

void
lunar_array_clear (Lunar_Array *a)
{
  if (a == NULL)
    {
      return;
    }

  // note: we are not resetting the capacity, just the size,
  // since we don't want to reallocate the array, the space that already exists
  // isn't removed, only unoccupied.
  a->size = 0;

  memset (a->ptr, 0, a->cap * sizeof (void *));
}

void *
lunar_array_peek (Lunar_Array *a)
{
  if (a == NULL)
    {
      return NULL;
    }

  return a->ptr[a->size - 1];
}

Lunar_Array *
lunar_array_subset (Lunar_Array *a, int start, int end)
{
  if (a == NULL || start < 0 || end > a->size)
    {
      return NULL;
    }
  Lunar_Array *new = lunar_array_new ();

  for (int i = start; i < end; i++)
    {
      lunar_array_push (new, a->ptr[i]);
    }

  return new;
}

Lunar_Array *
lunar_array_copy (Lunar_Array *a)
{
  if (a == NULL)
    {
      return NULL;
    }

  Lunar_Array *new = lunar_array_new ();

  for (int i = 0; i < a->size; i++)
    {
      lunar_array_push (new, a->ptr[i]);
    }

  return new;
}

int
lunar_array_same (Lunar_Array *a, Lunar_Array *b)
{
  if (a == NULL || b == NULL)
    {
      return 0;
    }

  if (a->size != b->size)
    {
      return 0;
    }

  for (int i = 0; i < a->size; i++)
    {
      if (i > b->size)
        {
          return 0;
        }

      if (a->ptr[i] != b->ptr[i])
        {
          return 0;
        }
    }
  return 1;
}

void
lunar_array_free (Lunar_Array *a)
{
  if (a == NULL)
    {
      return;
    }

  for (size_t i = 0; i < a->size; i++)
    {
      if (a->is_string_array == 1) {
        LFree (((Lunar_Buffer*) a->ptr[i])->ptr);
      }

      else if (a->is_token_array == 1) {
        LFree (((Lunar_Token*) a->ptr[i])->ptr);
      }
      
      LFree (a->ptr[i]);
    }

  a->cap = -1;
  a->size = -1;

  LFree (a->ptr);
  LFree (a);
}

void
lunar_array_free_dumb (Lunar_Array *a)
{
  if (a == NULL)
    {
      return;
    }
  LFree (a->ptr);
  LFree (a);
}
