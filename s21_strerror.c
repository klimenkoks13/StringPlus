#include "s21_errors.h"
#include "s21_string.h"

static s21_size_t s21_uint_to_str(unsigned int n, char* buf) {
  s21_size_t len = 0;
  char tmp[16];
  s21_size_t t = 0;

  if (n == 0) {
    buf[len++] = '0';

  } else {
    while (n > 0) {
      tmp[t++] = (char)('0' + (n % 10));
      n /= 10;
    }
    while (t > 0) {
      buf[len++] = tmp[--t];
    }
  }
  buf[len] = '\0';
  return len;
}

static char* s21_build_unknown(int errnum) {
  static char buf[64];
  const char* prefix = "Unknown error ";
  char num_buf[16];
  s21_size_t pos = 0;

  unsigned int magnitude =
      (errnum < 0) ? (unsigned int)(-(errnum + 1)) + 1U : (unsigned int)errnum;
  s21_uint_to_str(magnitude, num_buf);

  s21_size_t prefix_len = s21_strlen(prefix);
  s21_size_t num_len = s21_strlen(num_buf);

  for (s21_size_t i = 0; i < prefix_len; i++) {
    buf[pos++] = prefix[i];
  }
  if (errnum < 0) {
    buf[pos++] = '-';
  }
  for (s21_size_t i = 0; i < num_len; i++) {
    buf[pos++] = num_buf[i];
  }
  buf[pos] = '\0';

  return buf;
}

char* s21_strerror(int errnum) {
  char* res = S21_NULL;

  if (errnum >= 0 && errnum <= S21_MAX_ERROR) {
    res = (char*)s21_error_messages[errnum];
  } else {
    res = s21_build_unknown(errnum);
  }

  return res;
}