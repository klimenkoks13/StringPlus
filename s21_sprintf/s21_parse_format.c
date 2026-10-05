#include "s21_sprintf.h"

static void s21_init_spec(t_format_spec* spec) {
  spec->flag_minus = 0;
  spec->flag_plus = 0;
  spec->flag_space = 0;
  spec->flag_zero = 0;
  spec->width = -1;
  spec->precision = -1;
  spec->length = 0;
  spec->specifier = '\0';
}

static int s21_is_flag_char(char c) {
  return (c == '-' || c == '+' || c == ' ' || c == '0');
}

static void s21_apply_flag(t_format_spec* spec, char c) {
  if (c == '-')
    spec->flag_minus = 1;
  else if (c == '+')
    spec->flag_plus = 1;
  else if (c == ' ')
    spec->flag_space = 1;
  else
    spec->flag_zero = 1;
}

// сдвиг указателя (пока есть флаги-символы)
static void s21_parse_flags(const char** format, t_format_spec* spec) {
  while (s21_is_flag_char(**format)) {
    s21_apply_flag(spec, **format);
    (*format)++;
  }
}

static void s21_parse_width(const char** format, t_format_spec* spec) {
  if (**format >= '1' && **format <= '9') {
    spec->width = 0;
    while (**format >= '0' && **format <= '9') {
      spec->width = spec->width * 10 + (**format - '0');
      (*format)++;
    }
  }
}

static void s21_parse_precision(const char** format, t_format_spec* spec) {
  if (**format == '.') {
    (*format)++;
    spec->precision = 0;
    while (**format >= '0' && **format <= '9') {
      spec->precision = spec->precision * 10 + (**format - '0');
      (*format)++;
    }
  }
}

static void s21_parse_length(const char** format, t_format_spec* spec) {
  if (**format == 'h') {
    spec->length = 1;
    (*format)++;
  } else if (**format == 'l') {
    spec->length = 2;
    (*format)++;
  }
}

static void s21_parse_specifier(const char** format, t_format_spec* spec) {
  const char* valid = "cdfsu%";
  if (**format != '\0' && s21_strchr(valid, **format) != S21_NULL) {
    spec->specifier = **format;
    (*format)++;
  }
}

void s21_parse_format(const char** format, t_format_spec* spec) {
  s21_init_spec(spec);
  s21_parse_flags(format, spec);
  s21_parse_width(format, spec);
  s21_parse_precision(format, spec);
  s21_parse_length(format, spec);
  s21_parse_specifier(format, spec);
}