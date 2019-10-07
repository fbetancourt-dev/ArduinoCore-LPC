#include "wiring_string.h"

#include <stdio.h>
#include <limits.h>

//convert double to ascii
char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}