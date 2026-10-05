#include "s21_sprintf.h"

int s21_append_char(char* out, char c) {
  out[0] = c;
  return 1;
}

int s21_append_string(char* out, const char* src, s21_size_t len) {
  if (len > 0) {
    s21_memcpy(out, src, len);
  }
  return (int)len;
}

int s21_pad(char* out, char pad_char, int count) {
  int written = 0;
  if (count > 0) {
    s21_memset(out, pad_char, (s21_size_t)count);
    written = count;
  }
  return written;
}

int s21_append_padded(char* out, const char* src, s21_size_t len, int width,
                      char pad_char, int left_align) {
  int written = 0;
  int pad = (width > (int)len) ? (width - (int)len) : 0;

  if (!left_align && pad > 0) {
    written += s21_pad(out + written, pad_char, pad);
  }
  if (len > 0) {
    written += s21_append_string(out + written, src, len);
  }
  if (left_align && pad > 0) {
    written += s21_pad(out + written, ' ', pad);
  }
  return written;
}