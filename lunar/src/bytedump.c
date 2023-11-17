/*Copyright 2019-2023 Kai D. Gonzalez*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main(int argc, char *argv[]) {
  if (argc != 2) {
    usage:
    printf("usage: %s <file>\n", argv[0]);
    return 0;
  }

  if (strcmp(argv[1], "--help") == 0) {
    goto usage;
  }

  FILE *f = fopen(argv[1], "rb");

  if (f == NULL) {
    perror("fopen");
    return 1;
  }

  char __c;

  while ((__c = fgetc(f)) != EOF) {
    printf("%d ", __c);
  }

  printf("\nbytedump: %d bytes\n", (int)ftell(f));

  fclose(f);
}
