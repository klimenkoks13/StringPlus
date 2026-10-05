#include "s21_string.h"

void* s21_memchr(const void* str, int c, s21_size_t n) {
  void* res = S21_NULL;
  const unsigned char* ptr_str = (const unsigned char*)str;
  for (s21_size_t i = 0; i < n && res == S21_NULL; i++) {
    if (ptr_str[i] == (unsigned char)c) {
      res = (void*)(ptr_str + i);
    }
  }
  return res;
}

int s21_memcmp(const void* str1, const void* str2, s21_size_t n) {
  int res = 0;
  const unsigned char* ptr_str1 = (const unsigned char*)str1;
  const unsigned char* ptr_str2 = (const unsigned char*)str2;

  for (s21_size_t i = 0; i < n && res == 0; i++) {
    if (ptr_str1[i] != ptr_str2[i]) {
      res = (int)ptr_str1[i] - (int)ptr_str2[i];
    }
  }

  return res;
}

void* s21_memcpy(void* dest, const void* src, s21_size_t n) {
  const unsigned char* ptr_src = (const unsigned char*)src;
  unsigned char* ptr_dest = (unsigned char*)dest;

  for (s21_size_t i = 0; i < n; i++) {
    ptr_dest[i] = ptr_src[i];
  }

  return dest;
}

void* s21_memset(void* str, int c, s21_size_t n) {
  unsigned char* ptr_str = (unsigned char*)str;

  for (s21_size_t i = 0; i < n; i++) {
    ptr_str[i] = (unsigned char)c;
  }

  return str;
}