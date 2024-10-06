#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

typedef void (*output_func_t)(char ch, void *arg);

static int format_d(output_func_t output_func, void *arg, int num) {
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
    output_func(temp[--i], arg);
  }

  return len;
}

static int format_u(output_func_t output_func, void *arg, unsigned int num) {
  static char temp[16];
  int i = 0;

  // 将数字转换为字符串，从末尾开始存入 temp
  do {
    temp[i++] = (num % 10) + '0';
    num /= 10;
  } while (num > 0);

  int len = i;

  // 反转 temp，并存入目标缓冲区
  while (i > 0) {
    output_func(temp[--i], arg);
  }

  return len;
}

static int format_x(output_func_t output_func, void *arg, unsigned int num) {
  static char temp[16];
  int i = 0;

  // 将数字转换为十六进制字符串，从末尾开始存入 temp
  do {
    int digit = num % 16;
    if (digit < 10) {
      temp[i++] = digit + '0';  // 数字部分 '0'-'9'
    } else {
      temp[i++] = digit - 10 + 'a';  // 字符部分 'a'-'f'
    }
    num /= 16;
  } while (num > 0);

  int len = i;

  // 反转 temp，并存入目标缓冲区
  while (i > 0) {
    output_func(temp[--i], arg);
  }

  return len;
}


static int format(output_func_t output_func, void *arg, const char *fmt, va_list args) {
  const char *ptr;
  int written = 0;
  for (ptr = fmt; *ptr != '\0'; ptr++) {
    if (*ptr == '%' && *(ptr + 1) != '\0') {
      ptr++;
      switch (*ptr) {
      case 'c': {
        output_func(va_arg(args, int), arg);
        written += 1;
        break;
      }
      case 's': {
        const char *str = va_arg(args, const char *);
        while (*str != '\0') {
          output_func(*str++, arg);
          written += 1;
        }
        break;
      }
      case 'd': {
        written += format_d(output_func, arg, va_arg(args, int));
        break;
      }
      case 'u': {
        written += format_u(output_func, arg, va_arg(args, unsigned int));
        break;
      }
      case 'x': {
        written += format_x(output_func, arg, va_arg(args, unsigned long long));
        break;
      }
      default: // 不支持的格式符，直接输出原字符
        output_func('%', arg);
        output_func(*ptr, arg);
        written += 2;
        break;
      }
    } else {
      output_func(*ptr, arg);
      written += 1;
    }
  }
  return written;
}

void output_to_buffer(char ch, void *arg) {
  char **buffer_ptr = (char **)arg;
  **buffer_ptr = ch;
  (*buffer_ptr)++;
}

void output_to_stdout(char ch, void *arg) {
  putch(ch);
}

int printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  int written = format(output_to_stdout, NULL, fmt, args);

  va_end(args);

  return written;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  char *out_ptr = out;
  format(output_to_buffer, &out_ptr, fmt, args);

  *out_ptr = '\0';

  va_end(args);

  return out_ptr - out;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
