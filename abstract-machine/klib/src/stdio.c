#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  const char *ptr;
  char *out_ptr = out;
  int written = 0;

  for (ptr = fmt; *ptr != '\0'; ptr++) {
    if (*ptr == '%' && *(ptr + 1) != '\0') {
      ptr++;
      switch (*ptr) {
      case 's': {
        const char *str = va_arg(args, const char *);
        while (*str != '\0') {
          *out_ptr++ = *str++;
          written++;
        }
        break;
      }
      case 'd': {
        int num = va_arg(args, int);
        int len = sprintf(out_ptr, "%d", num);
        if (len < 0) {
          va_end(args);
          return -1; // 输出错误
        }
        out_ptr += len;
        written += len;
        break;
      }
      default:
        *out_ptr++ = '%';
        *out_ptr++ = *ptr;
        written += 2;
        break;
      }
    } else {
      *out_ptr++ = *ptr;
      written++;
    }
  }

  *out_ptr = '\0';

  va_end(args);

  return written;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
