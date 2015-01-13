#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "platform.h"


void
fault(const char * format, ...) {
  va_list ap;
  va_start(ap, format);
  pf_vprint(format, ap);
  va_end(ap);
  exit(1);
}

