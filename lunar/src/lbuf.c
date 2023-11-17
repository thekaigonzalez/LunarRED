#include "lbuf.h"

Lunar_Buffer *
lunar_buffer_new ()
{
  Lunar_Buffer *n = LMalloc(sizeof (Lunar_Buffer));

  if (n == NULL) {
    printf("warning: out of memory\n");
    return NULL; // out of memory
  }

  n->ptr = LMalloc(sizeof (char) * LUNARRED_MEMORY_INCREMENT);
  n->size = 0;
  n->cap = LUNARRED_MEMORY_INCREMENT;
  n->resets = 0;

  return n;
}

void
lunar_buffer_append (Lunar_Buffer *b, char c)
{
  if (b == NULL) return;

  if (b->size == b->cap)
    {
      b->cap = b->cap + LUNARRED_MEMORY_INCREMENT;
      b->ptr = LRealloc (b->ptr, b->cap * sizeof (char));
    }

  b->ptr[b->size] = c;
  b->size++;
}

void
lunar_buffer_remove (Lunar_Buffer *b, int index)
{
  if (b == NULL || index >= b->size || index < 0) return;

  memmove (b->ptr + index, b->ptr + index + 1, b->size - index - 1);

  // escape the last character
  b->ptr[b->size - 1] = '\0';

  b->size -= 1;
}

void lunar_buffer_clear(Lunar_Buffer *b)
{
  if (b == NULL) return;
  
  memset(b->ptr, 0, b->cap * sizeof(char));

  b->size = 0;
  b->resets++;
}

char *
lunar_buffer_copy (Lunar_Buffer *b)
{
  if (b == NULL)
    return NULL;

  char *n = LMalloc(sizeof (char) * (b->size + 2));
  memcpy (n, b->ptr, b->size);
  
  n[b->size] = '\0';
  return n;
}

void
lunar_buffer_free (Lunar_Buffer *b)
{
  if (b == NULL) return;

  b->size = -1;
  b->cap = -1;

  LFree (b->ptr);
  LFree (b);
}
