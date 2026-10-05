#include <math.h>

#include "s21_sprintf.h"

// ---------- вспомогательные преобразования ----------

static int s21_get_uint_len(unsigned long long v) {
  int len = 1;
  while (v >= 10) {
    v /= 10;
    len++;
  }
  return len;
}

static int s21_write_int_part(unsigned long long ip, char* buf) {
  int len = s21_get_uint_len(ip);
  int pos = len;
  buf[pos] = '\0';
  while (pos > 0) {
    buf[--pos] = (char)('0' + (ip % 10));
    ip /= 10;
  }
  return len;
}

// преобразование long long в строку с обработкой переполнения

static int s21_int_to_str(long long value, char* buf) {
  unsigned long long v;
  if (value < 0) {
    v = (unsigned long long)(-(value + 1)) + 1ULL;
  } else {
    v = (unsigned long long)value;
  }
  return s21_write_int_part(v, buf);
}

static int s21_uint_to_str(unsigned long long value, char* buf) {
  return s21_write_int_part(value, buf);
}

// запись дробной части в буфер
static void s21_write_frac_part(unsigned long long frac, int precision,
                                char* buf) {
  int pos = precision;
  buf[pos] = '\0';
  while (pos > 0) {
    buf[--pos] = (char)('0' + (frac % 10));
    frac /= 10;
  }
}

// банковское округление
static unsigned long long s21_round_half_even(long double scaled,
                                              unsigned long long int_part) {
  unsigned long long frac_int = (unsigned long long)scaled;
  long double remainder = scaled - (long double)frac_int;
  long double half = 0.5L;

  if (remainder > half) {
    frac_int++;
  } else if (remainder == half) {
    unsigned long long last_digit =
        (frac_int > 0) ? (frac_int % 10) : (int_part % 10);
    if (last_digit % 2 != 0) frac_int++;
  }
  return frac_int;
}

// разделение на целую и дробную части с округлением
static void s21_float_split(long double value, int precision,
                            unsigned long long* ip, unsigned long long* frac,
                            int* negative) {
  long double v = value;
  *negative = 0;
  if (v < 0 || (v == 0 && 1.0 / v < 0)) {
    *negative = 1;
    v = -v;
  }

  long double int_part;
  long double frac_part = modfl(v, &int_part);

  long double factor = 1.0L;
  for (int i = 0; i < precision; i++) factor *= 10.0L;

  long double scaled = frac_part * factor;
  unsigned long long frac_int =
      s21_round_half_even(scaled, (unsigned long long)int_part);
  unsigned long long max_frac = (unsigned long long)factor;

  if (frac_int >= max_frac) {
    frac_int = 0;
    int_part += 1.0L;
  }

  *ip = (unsigned long long)int_part;
  *frac = frac_int;
}

// -------- long double в строку с заданной точностью --------

// формирование строки для nan или inf
static int s21_float_to_str_special(long double value, char* buf) {
  int len = 0;
  if (isnan(value)) {
    s21_memcpy(buf, "nan", 3);
    len = 3;
  } else {
    if (value < 0) buf[len++] = '-';
    s21_memcpy(buf + len, "inf", 3);
    len += 3;
  }
  return len;
}

static int s21_float_to_str(long double value, char* buf, int precision) {
  int len = 0;
  if (isnan(value) || isinf(value)) {
    len = s21_float_to_str_special(value, buf);
  } else {
    unsigned long long ip;
    unsigned long long frac;
    int negative;
    s21_float_split(value, precision, &ip, &frac, &negative);
    if (negative) buf[len++] = '-';
    len += s21_write_int_part(ip, buf + len);
    if (precision > 0) {
      buf[len++] = '.';
      s21_write_frac_part(frac, precision, buf + len);
      len += precision;
    }
  }
  return len;
}

// ---------- извлечение аргументов ----------

static long long s21_fetch_signed(va_list* args, int length) {
  long long v;
  if (length == 1)
    v = (long long)(short)va_arg(*args, int);
  else if (length == 2)
    v = va_arg(*args, long);
  else
    v = va_arg(*args, int);
  return v;
}

static unsigned long long s21_fetch_unsigned(va_list* args, int length) {
  unsigned long long v;
  if (length == 1)
    v = (unsigned long long)(unsigned short)va_arg(*args, unsigned int);
  else if (length == 2)
    v = va_arg(*args, unsigned long);
  else
    v = va_arg(*args, unsigned int);
  return v;
}

// ---------- форматирование целых ----------

// вычисление параметров
static int s21_compute_int_parts(t_format_spec* spec, unsigned long long value,
                                 int digits_len, int negative,
                                 int* leading_zeros, int* pad, int* sign_len) {
  int zero_prec = (spec->precision == 0 && value == 0);
  int prec = spec->precision > 0 ? spec->precision : 0;
  if (zero_prec) digits_len = 0;

  *leading_zeros = (prec > digits_len) ? prec - digits_len : 0;
  if (negative)
    *sign_len = 1;
  else if (spec->flag_plus || spec->flag_space)
    *sign_len = 1;
  else
    *sign_len = 0;

  int total = *sign_len + *leading_zeros + digits_len;
  int width = spec->width > 0 ? spec->width : 0;
  *pad = (width > total) ? (width - total) : 0;
  return zero_prec;
}

// форматирование целого с заполнением "0" или " "
static int s21_format_integer_zero_pad(char* out, t_format_spec* spec,
                                       int negative, const char* digits,
                                       int digits_len, int leading_zeros,
                                       int pad) {
  int pos = 0;
  pos += s21_apply_sign(out + pos, spec, negative);
  pos += s21_pad(out + pos, '0', pad);
  pos += s21_pad(out + pos, '0', leading_zeros);
  if (digits_len > 0) {
    pos += s21_append_string(out + pos, digits, (s21_size_t)digits_len);
  }
  return pos;
}

static int s21_format_integer_spaces(char* out, t_format_spec* spec,
                                     int negative, const char* digits,
                                     int digits_len, int leading_zeros,
                                     int pad) {
  int pos = 0;
  if (!spec->flag_minus) pos += s21_pad(out + pos, ' ', pad);
  pos += s21_apply_sign(out + pos, spec, negative);
  pos += s21_pad(out + pos, '0', leading_zeros);
  if (digits_len > 0) {
    pos += s21_append_string(out + pos, digits, (s21_size_t)digits_len);
  }
  if (spec->flag_minus) pos += s21_pad(out + pos, ' ', pad);
  return pos;
}

// общая функция форматирования целых
static int s21_format_integer(char* out, t_format_spec* spec,
                              unsigned long long value, int negative,
                              const char* digits, int digits_len) {
  int leading_zeros, pad, sign_len;
  int zero_prec = s21_compute_int_parts(spec, value, digits_len, negative,
                                        &leading_zeros, &pad, &sign_len);
  if (zero_prec) digits_len = 0;

  int use_zero_pad =
      (!spec->flag_minus && spec->flag_zero && spec->precision < 0 && pad > 0);

  int written;
  if (use_zero_pad) {
    written = s21_format_integer_zero_pad(out, spec, negative, digits,
                                          digits_len, leading_zeros, pad);
  } else {
    written = s21_format_integer_spaces(out, spec, negative, digits, digits_len,
                                        leading_zeros, pad);
  }
  return written;
}

// ---------- форматирование по спецификаторам ----------

//%c
int s21_format_char(char* out, t_format_spec* spec, va_list* args) {
  char c = (char)va_arg(*args, int);
  int width = spec->width > 0 ? spec->width : 0;
  int pad = (width > 1) ? (width - 1) : 0;
  int pos = 0;

  if (!spec->flag_minus) pos += s21_pad(out + pos, ' ', pad);
  pos += s21_append_char(out + pos, c);
  if (spec->flag_minus) pos += s21_pad(out + pos, ' ', pad);
  return pos;
}

//%s
int s21_format_string(char* out, t_format_spec* spec, va_list* args) {
  const char* s = va_arg(*args, const char*);
  if (s == S21_NULL) s = "(null)";

  s21_size_t len = s21_strlen(s);
  if (spec->precision >= 0 && (s21_size_t)spec->precision < len) {
    len = (s21_size_t)spec->precision;
  }

  int width = spec->width > 0 ? spec->width : 0;
  return s21_append_padded(out, s, len, width, ' ', spec->flag_minus);
}

//%d %i
int s21_format_signed_int(char* out, t_format_spec* spec, va_list* args) {
  long long value = s21_fetch_signed(args, spec->length);
  char digits[32];
  int digits_len = s21_int_to_str(value, digits);
  int negative = (value < 0);
  unsigned long long magnitude =
      (value < 0) ? (unsigned long long)(-value) : (unsigned long long)value;
  return s21_format_integer(out, spec, magnitude, negative, digits, digits_len);
}

//%u
int s21_format_unsigned_int(char* out, t_format_spec* spec, va_list* args) {
  unsigned long long value = s21_fetch_unsigned(args, spec->length);
  char digits[32];
  int digits_len = s21_uint_to_str(value, digits);
  return s21_format_integer(out, spec, value, 0, digits, digits_len);
}

//%f заполнение "0"
static int s21_format_float_zero_pad(char* out, t_format_spec* spec,
                                     int negative, const char* body,
                                     int body_len, int pad) {
  int pos = 0;
  pos += s21_apply_sign(out + pos, spec, negative);
  pos += s21_pad(out + pos, '0', pad);
  if (body_len > 0) {
    pos += s21_append_string(out + pos, body, (s21_size_t)body_len);
  }
  return pos;
}

//%f заполнение " "
static int s21_format_float_padded(char* out, t_format_spec* spec, int negative,
                                   const char* body, int body_len, int pad) {
  int pos = 0;
  if (!spec->flag_minus) pos += s21_pad(out + pos, ' ', pad);
  pos += s21_apply_sign(out + pos, spec, negative);
  if (body_len > 0) {
    pos += s21_append_string(out + pos, body, (s21_size_t)body_len);
  }
  if (spec->flag_minus) pos += s21_pad(out + pos, ' ', pad);
  return pos;
}

// основа форматирования %f
int s21_format_float(char* out, t_format_spec* spec, va_list* args) {
  long double value;
  if (spec->length == 2)
    value = va_arg(*args, long double);
  else
    value = (long double)va_arg(*args, double);

  int precision = spec->precision >= 0 ? spec->precision : 6;
  char digits[512];
  int digits_len = s21_float_to_str(value, digits, precision);

  int is_special = isnan(value) || isinf(value);
  int negative = (digits[0] == '-');
  const char* body = negative ? digits + 1 : digits;
  int body_len = negative ? digits_len - 1 : digits_len;

  int sign_len = 0;
  if (negative)
    sign_len = 1;
  else if (spec->flag_plus || spec->flag_space)
    sign_len = 1;

  int total = sign_len + body_len;
  int width = spec->width > 0 ? spec->width : 0;
  int pad = (width > total) ? (width - total) : 0;

  int use_zero_pad =
      (!spec->flag_minus && spec->flag_zero && !is_special && pad > 0);

  int written;
  if (use_zero_pad) {
    written =
        s21_format_float_zero_pad(out, spec, negative, body, body_len, pad);
  } else {
    written = s21_format_float_padded(out, spec, negative, body, body_len, pad);
  }
  return written;
}

//%%
int s21_format_percent(char* out, t_format_spec* spec, va_list* args) {
  (void)spec;
  (void)args;
  out[0] = '%';
  return 1;
}

int s21_format_specifier(char* out, t_format_spec* spec, va_list* args) {
  int written = 0;
  switch (spec->specifier) {
    case 'c':
      written = s21_format_char(out, spec, args);
      break;
    case 's':
      written = s21_format_string(out, spec, args);
      break;
    case 'd':
      written = s21_format_signed_int(out, spec, args);
      break;
    case 'u':
      written = s21_format_unsigned_int(out, spec, args);
      break;
    case 'f':
      written = s21_format_float(out, spec, args);
      break;
    case '%':
      written = s21_format_percent(out, spec, args);
      break;
    default:
      break;
  }
  return written;
}