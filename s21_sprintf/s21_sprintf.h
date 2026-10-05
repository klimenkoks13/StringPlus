#ifndef S21_SPRINTF_H
#define S21_SPRINTF_H

#include <stdarg.h>

#include "../s21_string.h"

typedef struct s_format_spec {
  int flag_minus;
  int flag_plus;
  int flag_space;
  int flag_zero;
  int width;
  int precision;
  int length;
  char specifier;
} t_format_spec;

void s21_parse_format(const char** format, t_format_spec* spec);
int s21_format_specifier(char* out, t_format_spec* spec, va_list* args);

int s21_append_char(char* out, char c);
int s21_append_string(char* out, const char* src, s21_size_t len);
int s21_append_padded(char* out, const char* src, s21_size_t len, int width,
                      char pad_char, int left_align);
int s21_pad(char* out, char pad_char, int count);
int s21_apply_sign(char* out, t_format_spec* spec, int is_negative);

#endif