#include "s21_string.h"

// Копирует до n символов из src в dest, добивая нулями
char* s21_strncpy(char* dest, const char* src, s21_size_t n) {
  s21_size_t i = 0;

  while (i < n && src[i] != '\0') {
    dest[i] = src[i];
    i++;
  }

  while (i < n) {
    dest[i] = '\0';
    i++;
  }

  return dest;
}

// Добавляет до n символов из src в конец dest
char* s21_strncat(char* dest, const char* src, s21_size_t n) {
  s21_size_t ind = 0;

  while (dest[ind] != '\0') {
    ind++;
  }

  for (s21_size_t i = 0; i < n && src[i] != '\0'; i++) {
    dest[ind] = src[i];
    ind++;
  }

  dest[ind] = '\0';

  return dest;
}

// Сравнивает не более первых n байтов str1 и str2
int s21_strncmp(const char* str1, const char* str2, s21_size_t n) {
  int res = 0;
  const unsigned char* ptr_str1 = (const unsigned char*)str1;
  const unsigned char* ptr_str2 = (const unsigned char*)str2;

  for (s21_size_t i = 0; i < n && res == 0; i++) {
    if (ptr_str1[i] != ptr_str2[i]) {
      res = (int)ptr_str1[i] - (int)ptr_str2[i];
    } else if (ptr_str1[i] == '\0') {
      i = n;
    }
  }

  return res;
}

// Первое вхождение символа в строке
char* s21_strchr(const char* str, int c) {
  char* result = S21_NULL;

  while (*str != '\0' && result == S21_NULL) {
    if (*str == (char)c) {
      result = (char*)str;
    }
    str++;
  }

  if ((char)c == '\0') {
    result = (char*)str;
  }

  return result;
}

// Считает, сколько символов от начала str1 не встречаются в str2.
s21_size_t s21_strcspn(const char* str1, const char* str2) {
  s21_size_t length = 0;
  int found = 0;

  while (str1[length] != '\0' && !found) {
    const char* current = str2;

    while (*current != '\0' && !found) {
      if (str1[length] == *current) {
        found = 1;
      }
      current++;
    }

    if (!found) {
      length++;
    }
  }

  return length;
}

s21_size_t s21_strlen(const char* str) {
  s21_size_t length = 0;

  while (str[length] != '\0') {
    length++;
  }

  return length;
}

// Ищет первый символ str1, который встречается в str2.
char* s21_strpbrk(const char* str1, const char* str2) {
  char* result = S21_NULL;

  while (*str1 != '\0' && result == S21_NULL) {
    const char* current = str2;

    while (*current != '\0' && result == S21_NULL) {
      if (*str1 == *current) {
        result = (char*)str1;
      }
      current++;
    }

    str1++;
  }

  return result;
}

// Ищет последнее вхождение символа в строке.
char* s21_strrchr(const char* str, int c) {
  const char* last = S21_NULL;

  while (*str != '\0') {
    if (*str == (char)c) {
      last = str;
    }
    str++;
  }

  if ((char)c == '\0') {
    last = str;
  }

  return (char*)last;
}

// Ищет первое вхождение одной строки внутри другой.
char* s21_strstr(const char* haystack, const char* needle) {
  char* result = S21_NULL;

  if (*needle == '\0') {
    result = (char*)haystack;
  }

  while (*haystack != '\0' && result == S21_NULL) {
    const char* h = haystack;
    const char* n = needle;

    while (*h != '\0' && *n != '\0' && *h == *n) {
      h++;
      n++;
    }

    if (*n == '\0') {
      result = (char*)haystack;
    }

    haystack++;
  }

  return result;
}

static int s21_is_delimiter(char c, const char* delim) {
  int result = 0;

  while (*delim != '\0' && !result) {
    if (c == *delim) {
      result = 1;
    }
    delim++;
  }

  return result;
}

char* s21_strtok(char* str, const char* delim) {
  static char* next_token = S21_NULL;
  char* token = S21_NULL;

  if (str != S21_NULL) {
    next_token = str;
  }

  if (next_token != S21_NULL) {
    while (*next_token != '\0' && s21_is_delimiter(*next_token, delim)) {
      next_token++;
    }

    if (*next_token != '\0') {
      token = next_token;

      while (*next_token != '\0' && !s21_is_delimiter(*next_token, delim)) {
        next_token++;
      }

      if (*next_token != '\0') {
        *next_token = '\0';
        next_token++;
      }
    } else {
      next_token = S21_NULL;
    }
  }

  return token;
}