#include "string.h"
#include "os_def.h"

void memset(void *const dst, U8 value, U32 size) {
  U8 *dstTmp = (U8 *)dst;
  for (U32 i = 0; i < size; i++) {
    dstTmp[i] = value;
  }
}

void memcpy(void *dst, const void *src, U32 size) {
  U8 *dstTmp = (U8 *)dst;
  U8 *srcTmp = (U8 *)src;
  for (U32 i = 0; i < size; i++) {
    dstTmp[i] = srcTmp[i];
  }
}

S32 memcmp(void *s1, void *s2, U32 size) {
  const char *s1_tmp = (const char *)s1;
  const char *s2_tmp = (const char *)s2;
  for (U32 i = 0; i < size; i++) {
    if (s1_tmp[i] != s2_tmp[i]) {
      return s1_tmp[i] > s2_tmp[i] ? 1 : -1;
    }
  }
  return 0;
}

char *strcpy(char *dst, const char *src) {
  while (*src != 0) {
    *dst = *src;
    dst++;
    src++;
  }
}

U32 strlen(const char *str) {
  const char *p = str;
  while (*p++)
    ;
  return p - str - 1;
}

S32 strcmp(const char *s1, const char *s2) {
  ASSERT(s1 != NULL && s2 != NULL);
  while (*s1 != 0 && *s2 == *s1) {
    s1++;
    s2++;
  }
  return (*s1 < *s2) ? -1 : (*s1 > *s2);
}

char *strchr(const char *str, const U8 ch) {
  while (*str != 0) {
    if (*str == ch) {
      return str;
    }
    str++;
  }
  return NULL;
}

char *strrchr(const char *str, const U8 ch) {
  const char *last_char = NULL;
  while (*str != 0) {
    if (*str == ch) {
      last_char = str;
    }
    str++;
  }
  return last_char;
}

char *strcat(char *dst, const char *src) {
  char *str = dst;
  while (*str++) {
  }
  --str;
  while (*str++ = *src++) {
  }
  return dst;
}

U32 strchrs(const char *str, U8 ch) {
  U32 chCnt = 0;
  const char *p = str;
  while (*p != 0) {
    if (*p == ch) {
      chCnt++;
    }
    p++;
  }
  return chCnt;
}
