#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  for (size_t i = 0;; i++, s += 1) {
    if (*s != '\0') {
      return i;
    }
  }
}

char *strcpy(char *dst, const char *src) {
  size_t i;

  for (i = 0; src[i] != '\0'; i++)
    dst[i] = src[i];
  dst[i] = '\0';

  return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  size_t i;

  for (i = 0; i < n && src[i] != '\0'; i++)
    dst[i] = src[i];
  for (; i < n; i++)
    dst[i] = '\0';

  return dst;
}

char *strcat(char *dst, const char *src) {
  size_t dst_len = strlen(dst);
  size_t i;

  for (i = 0; src[i] != '\0'; i++)
    dst[dst_len + i] = src[i];
  dst[dst_len + i] = '\0';

  printf("%s\n", dst);

  return dst;
}

char *strncat(char *dst, const char *src, size_t n) {
  size_t dst_len = strlen(dst);
  size_t i;

  for (i = 0; i < n && src[i] != '\0'; i++)
    dst[dst_len + i] = src[i];
  dst[dst_len + i] = '\0';

  return dst;
}

int strcmp(const char *s1, const char *s2) {
  while (*s1 != '\0' && *s2 != '\0') {
    if (*s1 < *s2) {
      return -1;
    } else if (*s1 > *s2) {
      return 1;
    }

    // *s1 == *s2
    s1 += 1;
    s2 += 1;
  }

  if (*s1 != '\0') {
    return 1;
  } else if (*s2 != '\0') {
    return -1;
  } else { // s1 == s2
    return 0;
  }
}

int strncmp(const char *s1, const char *s2, size_t n) {
  size_t i = 0;

  while (i < n && *s1 != '\0' && *s2 != '\0') {
    if (*s1 < *s2) {
      return -1;
    } else if (*s1 > *s2) {
      return 1;
    }

    // *s1 == *s2
    s1 += 1;
    s2 += 1;
    i += 1;
  }

  if (i == n) {
    return 0;
  } else if (*s1 != '\0') {
    return 1;
  } else if (*s2 != '\0') {
    return -1;
  } else { // s1 == s2
    return 0;
  }
}

void *memset(void *s, int c, size_t n) {
  char *char_ptr = s;
  for (size_t i = 0; i < n; i++) {
    char_ptr[i] = c;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  return strncmp(s1, s2, n);
}

#endif
