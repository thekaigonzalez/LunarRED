#include "lmem.h"

void *
LMalloc (size_t size)
{
  void* ptr = calloc (size, sizeof (char));
  
  if (ptr == NULL) {
    printf("lunar: out of memory\n");
    exit(1);
  }

  return ptr;
}

void *
LRealloc (void *ptr, size_t size)
{
  ptr = realloc (ptr, size);

  if (ptr == NULL) {
    printf("lunar: out of memory\n");
    exit(1);
  }

  return ptr;
}

void
LFree (void *ptr)
{
  if (ptr == NULL) {
    printf("lunar: tried to free a blank pointer\n");
    exit(1);
    return;
  }
  free (ptr);
}
