/*Copyright 2019-2023 Kai D. Gonzalez*/

#include "lstd.h"

Lunar_ByteFunction map_defaults[] = {
  { "null", 00 },  // null. just null. thats it.
  { "echo", 40 },  // ECHO [byte]
  { "mov", 41 },   // MOVe [register] [byte]
  { "each", 42 },  // POPAll [register]
  { "clr", 43 },   // CLR [register]
  { "reset", 44 },   // RESET [...]
  { "push", 45 },  // PUSH [register] [byte] [position]
  { "get", 46 },   // GET [register] [position] [output_register]
  { "halt", 22 },   // END
  { "endsub", 11 }, // EXIT
  { "init", 100 }, // INIT [register]
};

byte std_map_search (char *name)
{
  int i;

  for (i = 0; i < sizeof (map_defaults) / sizeof (Lunar_ByteFunction); i++)
    {
      if (strcmp (name, map_defaults[i].name) == 0)
        {
          return map_defaults[i].value;
        }
    }

  return 0;
}
