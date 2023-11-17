#include "lerr.h"

void
lunar_Problem (Lunar_Object *obj, char *msg)
{
  if (obj == NULL) {
    lunar_Pretty(msg, LERR_UNDEFINED);
  }

  if (obj->info == NULL) {
    lunar_Pretty(msg, LERR_INVALID);
  }

  if (obj->info == NULL || obj->classification == LCL_UNDEFINED) {
    lunar_Pretty(msg, LERR_UNDEFINED);
  }

  if (obj->info == NULL || obj->classification == LCL_NULL) {
    lunar_Pretty(msg, LERR_INVALID);
  }

  lunar_Pretty(msg, LERR_UNDEFINED);
}

void
lunar_Pretty (char *msg, Lunar_Error level)
{
  switch (level) {
  case LERR_UNDEFINED:
    printf("%serror%s: %s\n", RED, RESET, msg);
    printf("\t[C]: called from undefined value\n");
    break;
  case LERR_INVALID:
    printf("%serror%s: %s\n", RED, RESET, msg);
    printf("\t[C]: called from invalid value\n");
    break;
  default:
    printf("%serror%s: %s\n", RED, RESET, msg);
    printf("\t[C]: %s\n", msg);
  }
}
