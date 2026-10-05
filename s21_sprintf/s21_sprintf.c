#include "s21_sprintf.h"

int s21_sprintf(char* str, const char* format, ...) {
  va_list args;
  va_start(args, format);
  char* out = str;

  while (*format != '\0') {
    if (*format != '%') {
      *out++ = *format++;
    } else {
      format++;
      t_format_spec spec;
      s21_parse_format(&format, &spec);
      out += s21_format_specifier(
          out, &spec,
          &args);  // сдвиг указателя на число отформатированных символов
    }
  }

  *out = '\0';
  va_end(args);
  return (int)(out - str);  // возврат длинны записаной строки
}