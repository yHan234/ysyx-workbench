#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

static int print_int(char *out, int num) {
  static char temp[16];
  int i = 0;
  int is_negative = 0;

  // 处理负数
  if (num < 0) {
    is_negative = 1;
    num = -num;
  }

  // 将数字转换为字符串，从末尾开始存入 temp
  do {
    temp[i++] = (num % 10) + '0';
    num /= 10;
  } while (num > 0);

  // 如果是负数，添加负号
  if (is_negative) {
    temp[i++] = '-';
  }

  int len = i;

  // 反转 temp，并存入目标缓冲区
  while (i > 0) {
    *(out++) = temp[--i];
  }

  return len;
}

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
        int len = print_int(out_ptr, num);
        out_ptr += len;
        written += len;
        break;
      }
      default:
        return -1;
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
