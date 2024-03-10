#include "os_def.h"
#include "os_print_internal.h"

OsPrintCharHook g_printCharHook = NULL;

void OsPrintStr(char *str) {
  char *str = str;
  U32 i;

  if (g_printCharHook == NULL) {
    return;
  }

  while (str[i] != 0) {
    g_printCharHook(str[i]);
    i++;
  }
}

void OsPrintHex(U32 num) {
  U8 off = 7;
  U8 low;
  U32 numTmp = num;
  char p;
  char buf[9] = {0};

  if (numTmp == 0) {
    buf[off--] = '0';
  }

  while (numTmp != 0) {
    low = numTmp & 0xf;
    if (low >= 0 && low <= 9) {
      p = low + '0';
    } else {
      p = low - 10 + 'A';
    }
    buf[off--] = p;
    numTmp >>= 4;
  }

  OsPrintStr(buf + (++off));
}