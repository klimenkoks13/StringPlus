#include "s21_sprintf.h"

int s21_apply_sign(char* out, t_format_spec* spec, int is_negative) {
  int written = 0;
  if (is_negative) {
    out[written++] = '-';

  } else if (spec->flag_plus) {
    out[written++] = '+';
  } else if (spec->flag_space) {
    out[written++] = ' ';
  }
  return written;
}