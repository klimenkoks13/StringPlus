#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "s21_string.h"

/* ============================================================= */
/* ======================== s21_mem ============================ */
/* ============================================================= */

START_TEST(test_memchr) {
  char str1[] = "Hello, School21!";
  char str2[] = "abcdef";
  ck_assert_ptr_eq(s21_memchr(str1, 'S', 16), memchr(str1, 'S', 16));
  ck_assert_ptr_eq(s21_memchr(str1, 'z', 16), memchr(str1, 'z', 16));
  ck_assert_ptr_eq(s21_memchr(str1, '\0', 16), memchr(str1, '\0', 16));
  ck_assert_ptr_eq(s21_memchr(str2, 'f', 0), memchr(str2, 'f', 0));
  ck_assert_ptr_eq(s21_memchr(str2, 'c', 6), memchr(str2, 'c', 6));

  const unsigned char hi[] = {0xFF, 0x01, 0xFF, 0x02};
  ck_assert_ptr_eq(s21_memchr(hi, 0xFF, 4), memchr(hi, 0xFF, 4));
}
END_TEST

START_TEST(test_memcmp) {
  ck_assert_int_eq(s21_memcmp("abc", "abc", 3), 0);
  ck_assert_int_lt(s21_memcmp("abc", "abd", 3), 0);
  ck_assert_int_gt(s21_memcmp("abd", "abc", 3), 0);
  ck_assert_int_eq(s21_memcmp("abc", "xyz", 0), 0);
  ck_assert_int_eq(s21_memcmp("abcdef", "abcxyz", 3), 0);

  const unsigned char a[] = {0xFF};
  const unsigned char b[] = {0x01};
  ck_assert_int_gt(s21_memcmp(a, b, 1), 0);
}
END_TEST

START_TEST(test_memcpy) {
  char src[] = "School21 is cool";
  char dst1[32] = {0};
  char dst2[32] = {0};

  s21_memcpy(dst1, src, 17);
  memcpy(dst2, src, 17);
  ck_assert_int_eq(memcmp(dst1, dst2, 17), 0);

  char dst3[8] = "AAAAAAA";
  char dst4[8] = "AAAAAAA";
  s21_memcpy(dst3, src, 0);
  memcpy(dst4, src, 0);
  ck_assert_int_eq(memcmp(dst3, dst4, 8), 0);
}
END_TEST

START_TEST(test_memset) {
  char b1[16];
  char b2[16];

  s21_memset(b1, 'A', 10);
  memset(b2, 'A', 10);
  ck_assert_int_eq(memcmp(b1, b2, 10), 0);

  s21_memset(b1, '\0', 16);
  memset(b2, '\0', 16);
  ck_assert_int_eq(memcmp(b1, b2, 16), 0);

  s21_memset(b1, 0xFF, 5);
  memset(b2, 0xFF, 5);
  ck_assert_int_eq(memcmp(b1, b2, 5), 0);

  int zero = 0;
  s21_memset(b1, 'X', zero);
  memset(b2, 'X', zero);
  ck_assert_int_eq(memcmp(b1, b2, 16), 0);
}
END_TEST

/* ============================================================= */
/* ======================== s21_str ============================ */
/* ============================================================= */

START_TEST(test_strlen) {
  ck_assert_uint_eq(s21_strlen(""), (s21_size_t)0);
  ck_assert_uint_eq(s21_strlen("a"), (s21_size_t)1);
  ck_assert_uint_eq(s21_strlen("Hello, World!"), strlen("Hello, World!"));
  ck_assert_uint_eq(s21_strlen("          "), (s21_size_t)10);
  ck_assert_uint_eq(s21_strlen("\t\n"), (s21_size_t)2);
}
END_TEST

START_TEST(test_strchr) {
  const char* s = "Hello, School21!";
  ck_assert_ptr_eq(s21_strchr(s, 'S'), strchr(s, 'S'));
  ck_assert_ptr_eq(s21_strchr(s, 'H'), strchr(s, 'H'));
  ck_assert_ptr_eq(s21_strchr(s, '!'), strchr(s, '!'));
  ck_assert_ptr_eq(s21_strchr(s, 'z'), strchr(s, 'z'));
  ck_assert_ptr_eq(s21_strchr(s, '\0'), strchr(s, '\0'));
  ck_assert_ptr_eq(s21_strchr("", 'a'), strchr("", 'a'));
  ck_assert_ptr_eq(s21_strchr("", '\0'), strchr("", '\0'));

  const char hi[] = {(char)0xFF, 0};
  int c = (int)(char)0xFF;
  ck_assert_ptr_eq(s21_strchr(hi, c), strchr(hi, c));
}
END_TEST

START_TEST(test_strrchr) {
  const char* s = "Hello, School21!";
  ck_assert_ptr_eq(s21_strrchr(s, 'o'), strrchr(s, 'o'));
  ck_assert_ptr_eq(s21_strrchr(s, 'l'), strrchr(s, 'l'));
  ck_assert_ptr_eq(s21_strrchr(s, 'S'), strrchr(s, 'S'));
  ck_assert_ptr_eq(s21_strrchr(s, 'z'), strrchr(s, 'z'));
  ck_assert_ptr_eq(s21_strrchr(s, '\0'), strrchr(s, '\0'));
  ck_assert_ptr_eq(s21_strrchr("", 'a'), strrchr("", 'a'));
  ck_assert_ptr_eq(s21_strrchr("", '\0'), strrchr("", '\0'));

  const char hi[] = {(char)0xFF, 'a', (char)0xFF, 0};
  int c = (int)(char)0xFF;
  ck_assert_ptr_eq(s21_strrchr(hi, c), strrchr(hi, c));
}
END_TEST

START_TEST(test_strncmp) {
  ck_assert_int_eq(s21_strncmp("abc", "abc", 3), 0);
  ck_assert_int_lt(s21_strncmp("abc", "abd", 3), 0);
  ck_assert_int_gt(s21_strncmp("abd", "abc", 3), 0);
  ck_assert_int_eq(s21_strncmp("abc", "xyz", 0), 0);
  ck_assert_int_lt(s21_strncmp("ab", "abc", 3), 0);
  ck_assert_int_gt(s21_strncmp("abc", "ab", 3), 0);
  ck_assert_int_eq(s21_strncmp("", "", 5), 0);
  ck_assert_int_eq(s21_strncmp("abc", "abc", 10), 0);

  const char a[] = {(char)0xFF, 'a', 0};
  const char b[] = {(char)0x01, 'a', 0};
  ck_assert_int_gt(s21_strncmp(a, b, 1), 0);
  ck_assert_int_lt(s21_strncmp(b, a, 1), 0);
}
END_TEST

START_TEST(test_strncpy) {
  char dst1[32] = "XXXXXXXXXXXXXXXX";
  char dst2[32] = "XXXXXXXXXXXXXXXX";
  const char* src = "School21";

  s21_strncpy(dst1, src, 10);
  strncpy(dst2, src, 10);
  ck_assert_int_eq(memcmp(dst1, dst2, 10), 0);

  char d1[16] = {0};
  char d2[16] = {0};
  s21_strncpy(d1, "abcdef", 3);
  memset(d2, 0, 16);
  memcpy(d2, "abcdef", 3);
  ck_assert_int_eq(memcmp(d1, d2, 3), 0);

  /* n == 0 — dest не должен измениться. */
  char z1[8] = "AAAAAAA";
  char z2[8] = "AAAAAAA";
  s21_strncpy(z1, "xyz", 0);
  ck_assert_int_eq(memcmp(z1, z2, 8), 0);
}
END_TEST

START_TEST(test_strncat) {
  char d1[32] = "Hello ";
  char d2[32] = "Hello ";
  s21_strncat(d1, "World!!!", 5);
  strncat(d2, "World!!!", 5);
  ck_assert_str_eq(d1, d2);

  char e1[32] = "abc";
  char e2[32] = "abc";
  s21_strncat(e1, "XYZ", 0);
  strncat(e2, "XYZ", 0);
  ck_assert_str_eq(e1, e2);

  char f1[32] = "abc";
  char f2[32] = "abc";
  s21_strncat(f1, "XY", 10);
  strncat(f2, "XY", 10);
  ck_assert_str_eq(f1, f2);

  char g1[32] = "";
  char g2[32] = "";
  s21_strncat(g1, "abc", 10);
  strncat(g2, "abc", 10);
  ck_assert_str_eq(g1, g2);
}
END_TEST

START_TEST(test_strcspn) {
  ck_assert_uint_eq(s21_strcspn("Hello, World!", "lo"),
                    strcspn("Hello, World!", "lo"));
  ck_assert_uint_eq(s21_strcspn("abcdef", "xyz"), strcspn("abcdef", "xyz"));
  ck_assert_uint_eq(s21_strcspn("abcdef", "a"), strcspn("abcdef", "a"));
  ck_assert_uint_eq(s21_strcspn("", "abc"), strcspn("", "abc"));
  ck_assert_uint_eq(s21_strcspn("abc", ""), strcspn("abc", ""));
}
END_TEST

START_TEST(test_strpbrk) {
  const char* s1 = "Hello, World!";
  ck_assert_ptr_eq(s21_strpbrk(s1, "od"), strpbrk(s1, "od"));
  ck_assert_ptr_eq(s21_strpbrk(s1, "z"), strpbrk(s1, "z"));
  ck_assert_ptr_eq(s21_strpbrk("abcdef", "xyz"), strpbrk("abcdef", "xyz"));
  ck_assert_ptr_eq(s21_strpbrk("", "abc"), strpbrk("", "abc"));
  ck_assert_ptr_eq(s21_strpbrk("abc", ""), strpbrk("abc", ""));
}
END_TEST

START_TEST(test_strstr) {
  const char* h = "xxSchool21xxSchool21xx";
  ck_assert_ptr_eq(s21_strstr(h, "School"), strstr(h, "School"));
  ck_assert_ptr_eq(s21_strstr(h, "21xx"), strstr(h, "21xx"));
  ck_assert_ptr_eq(s21_strstr(h, "zzz"), strstr(h, "zzz"));
  ck_assert_ptr_eq(s21_strstr(h, ""), strstr(h, ""));
  ck_assert_ptr_eq(s21_strstr("", ""), strstr("", ""));
  ck_assert_ptr_eq(s21_strstr("", "a"), strstr("", "a"));
  ck_assert_ptr_eq(s21_strstr("aaaaab", "aaab"), strstr("aaaaab", "aaab"));
}
END_TEST

START_TEST(test_strtok) {
  char s1[] = "cat, dog, elephant";
  char s2[] = "cat, dog, elephant";
  const char* d = ",";
  char* t1 = s21_strtok(s1, d);
  char* t2 = strtok(s2, d);
  while (t1 && t2) {
    ck_assert_str_eq(t1, t2);
    t1 = s21_strtok(S21_NULL, d);
    t2 = strtok(NULL, d);
  }
  ck_assert_ptr_eq(t1, t2);

  char s3[] = ",,,,";
  char s4[] = ",,,,";
  ck_assert_ptr_eq(s21_strtok(s3, ","), strtok(s4, ","));

  char s5[] = "";
  char s6[] = "";
  ck_assert_ptr_eq(s21_strtok(s5, ","), strtok(s6, ","));

  char s7[] = "cat;dog,bird";
  char s8[] = "cat;dog,bird";
  char* r1 = s21_strtok(s7, ",;");
  char* r2 = strtok(s8, ",;");
  while (r1 && r2) {
    ck_assert_str_eq(r1, r2);
    r1 = s21_strtok(S21_NULL, ",;");
    r2 = strtok(NULL, ",;");
  }
  ck_assert_ptr_eq(r1, r2);

  char s9[] = "a b c";
  char s10[] = "a b c";
  ck_assert_str_eq(s21_strtok(s9, " "), strtok(s10, " "));
  ck_assert_str_eq(s21_strtok(S21_NULL, " "), strtok(NULL, " "));
  ck_assert_str_eq(s21_strtok(S21_NULL, " "), strtok(NULL, " "));
  ck_assert_ptr_eq(s21_strtok(S21_NULL, " "), strtok(NULL, " "));
}
END_TEST

START_TEST(test_strerror) {
  ck_assert_str_eq(s21_strerror(0), strerror(0));
  ck_assert_str_eq(s21_strerror(1), strerror(1));
  ck_assert_str_eq(s21_strerror(2), strerror(2));
  ck_assert_str_eq(s21_strerror(5), strerror(5));
  ck_assert_ptr_nonnull(s21_strerror(9999));
  ck_assert_ptr_nonnull(s21_strerror(-5));
  ck_assert_ptr_nonnull(s21_strerror(INT_MIN));
}
END_TEST

/* ============================================================= */
/* ================ s21_sprintf: базовые ======================= */
/* ============================================================= */

START_TEST(test_sprintf_no_args) {
  char b1[100] = {0};
  char b2[100] = {0};

  b1[0] = '\0';
  b2[0] = '\0';
  ck_assert_int_eq(s21_sprintf(b1, ""), 0);
  ck_assert_str_eq(b1, b2);

  ck_assert_int_eq(s21_sprintf(b1, "Hello"), sprintf(b2, "Hello"));
  ck_assert_str_eq(b1, b2);

  ck_assert_int_eq(s21_sprintf(b1, "abc def 123"), sprintf(b2, "abc def 123"));
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_percent) {
  char b1[100] = {0};
  char b2[100] = {0};
  ck_assert_int_eq(s21_sprintf(b1, "%%"), sprintf(b2, "%%"));
  ck_assert_str_eq(b1, b2);
  ck_assert_int_eq(s21_sprintf(b1, "100%% ready"), sprintf(b2, "100%% ready"));
  ck_assert_str_eq(b1, b2);
  ck_assert_int_eq(s21_sprintf(b1, "%%%d%%", 5), sprintf(b2, "%%%d%%", 5));
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_char) {
  char b1[100] = {0};
  char b2[100] = {0};

  s21_sprintf(b1, "%c", 'A');
  sprintf(b2, "%c", 'A');
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%c", 66);
  sprintf(b2, "%c", 66);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%5c", 'A');
  sprintf(b2, "%5c", 'A');
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%-5c", 'A');
  sprintf(b2, "%-5c", 'A');
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%1c", 'A');
  sprintf(b2, "%1c", 'A');
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "abc%cdef", 'X');
  sprintf(b2, "abc%cdef", 'X');
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_string) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%s", "School21");
  sprintf(b2, "%s", "School21");
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%s", "");
  sprintf(b2, "%s", "");
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%10s", "abc");
  sprintf(b2, "%10s", "abc");
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%-10s", "abc");
  sprintf(b2, "%-10s", "abc");
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.2s", "School21");
  sprintf(b2, "%.2s", "School21");
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.0s", "School21");
  sprintf(b2, "%.0s", "School21");
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%10.3s", "School21");
  sprintf(b2, "%10.3s", "School21");
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%-10.3s", "School21");
  sprintf(b2, "%-10.3s", "School21");
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%3s", "School21");
  sprintf(b2, "%3s", "School21");
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.20s", "abc");
  sprintf(b2, "%.20s", "abc");
  ck_assert_str_eq(b1, b2);
}
END_TEST

/* ============================================================= */
/* ====================== s21_sprintf: %d ====================== */
/* ============================================================= */

START_TEST(test_sprintf_d_basic) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%d", 0);
  sprintf(b2, "%d", 0);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%d", 123);
  sprintf(b2, "%d", 123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%d", -123);
  sprintf(b2, "%d", -123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%d", INT_MAX);
  sprintf(b2, "%d", INT_MAX);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%d", INT_MIN);
  sprintf(b2, "%d", INT_MIN);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_d_width) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%5d", 123);
  sprintf(b2, "%5d", 123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%5d", -123);
  sprintf(b2, "%5d", -123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%2d", 123);
  sprintf(b2, "%2d", 123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%10d", 0);
  sprintf(b2, "%10d", 0);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_d_minus) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%-5d", 123);
  sprintf(b2, "%-5d", 123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%-5d", -123);
  sprintf(b2, "%-5d", -123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%-3d", 1);
  sprintf(b2, "%-3d", 1);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_d_precision) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%.5d", 123);
  sprintf(b2, "%.5d", 123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.5d", -123);
  sprintf(b2, "%.5d", -123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.0d", 0);
  sprintf(b2, "%.0d", 0);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.0d", 123);
  sprintf(b2, "%.0d", 123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.2d", 1);
  sprintf(b2, "%.2d", 1);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_d_width_precision) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%10.5d", 123);
  sprintf(b2, "%10.5d", 123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%-10.5d", 123);
  sprintf(b2, "%-10.5d", 123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%10.5d", -123);
  sprintf(b2, "%10.5d", -123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%-10.5d", -123);
  sprintf(b2, "%-10.5d", -123);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_d_zero) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%05d", 123);
  sprintf(b2, "%05d", 123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%05d", -123);
  sprintf(b2, "%05d", -123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%05d", 0);
  sprintf(b2, "%05d", 0);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%08.3d", 123);
  ck_assert_str_eq(b1, "     123");
}
END_TEST

START_TEST(test_sprintf_d_plus_space) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%+d", 123);
  sprintf(b2, "%+d", 123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%+d", -123);
  sprintf(b2, "%+d", -123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%+d", 0);
  sprintf(b2, "%+d", 0);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "% d", 123);
  sprintf(b2, "% d", 123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "% d", -123);
  sprintf(b2, "% d", -123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%+5d", 123);
  sprintf(b2, "%+5d", 123);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "% 5d", 123);
  sprintf(b2, "% 5d", 123);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_d_length_h) {
  char b1[200] = {0};
  char b2[200] = {0};

  short s = 12345;
  s21_sprintf(b1, "%hd", s);
  sprintf(b2, "%hd", s);
  ck_assert_str_eq(b1, b2);

  short neg = -12345;
  s21_sprintf(b1, "%hd", neg);
  sprintf(b2, "%hd", neg);
  ck_assert_str_eq(b1, b2);

  int big = 100000;
  s21_sprintf(b1, "%hd", big);
  sprintf(b2, "%hd", big);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_d_length_l) {
  char b1[200] = {0};
  char b2[200] = {0};

  long l = 1234567890L;
  s21_sprintf(b1, "%ld", l);
  sprintf(b2, "%ld", l);
  ck_assert_str_eq(b1, b2);

  long ln = -1234567890L;
  s21_sprintf(b1, "%ld", ln);
  sprintf(b2, "%ld", ln);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%ld", LONG_MIN);
  sprintf(b2, "%ld", LONG_MIN);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%ld", LONG_MAX);
  sprintf(b2, "%ld", LONG_MAX);
  ck_assert_str_eq(b1, b2);
}
END_TEST

/* ============================================================= */
/* ====================== s21_sprintf: %u ====================== */
/* ============================================================= */

START_TEST(test_sprintf_u_basic) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%u", 0u);
  sprintf(b2, "%u", 0u);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%u", 123u);
  sprintf(b2, "%u", 123u);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%u", UINT_MAX);
  sprintf(b2, "%u", UINT_MAX);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_u_flags) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%5u", 123u);
  sprintf(b2, "%5u", 123u);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%-5u", 123u);
  sprintf(b2, "%-5u", 123u);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.5u", 123u);
  sprintf(b2, "%.5u", 123u);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%05u", 123u);
  sprintf(b2, "%05u", 123u);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%10.5u", 123u);
  sprintf(b2, "%10.5u", 123u);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%-10.5u", 123u);
  sprintf(b2, "%-10.5u", 123u);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_u_length) {
  char b1[200] = {0};
  char b2[200] = {0};

  unsigned short us = 12345;
  s21_sprintf(b1, "%hu", us);
  sprintf(b2, "%hu", us);
  ck_assert_str_eq(b1, b2);

  unsigned long ul = 1234567890UL;
  s21_sprintf(b1, "%lu", ul);
  sprintf(b2, "%lu", ul);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%lu", ULONG_MAX);
  sprintf(b2, "%lu", ULONG_MAX);
  ck_assert_str_eq(b1, b2);
}
END_TEST

/* ============================================================= */
/* ====================== s21_sprintf: %f ====================== */
/* ============================================================= */

START_TEST(test_sprintf_f_basic) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%f", 0.0);
  sprintf(b2, "%f", 0.0);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%f", 123.456);
  sprintf(b2, "%f", 123.456);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%f", -123.456);
  sprintf(b2, "%f", -123.456);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%f", 0.1);
  sprintf(b2, "%f", 0.1);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_f_precision) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%.0f", 123.456);
  sprintf(b2, "%.0f", 123.456);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.2f", 123.456);
  sprintf(b2, "%.2f", 123.456);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.5f", 3.14159);
  sprintf(b2, "%.5f", 3.14159);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.10f", 1.0 / 3.0);
  sprintf(b2, "%.10f", 1.0 / 3.0);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.0f", 123.5);
  sprintf(b2, "%.0f", 123.5);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.0f", 124.5);
  sprintf(b2, "%.0f", 124.5);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.0f", 0.5);
  sprintf(b2, "%.0f", 0.5);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.0f", 1.5);
  sprintf(b2, "%.0f", 1.5);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.1f", 1.25);
  sprintf(b2, "%.1f", 1.25);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.0f", 999.6);
  sprintf(b2, "%.0f", 999.6);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_f_width) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%12.2f", 123.45);
  sprintf(b2, "%12.2f", 123.45);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%-12.2f", 123.45);
  sprintf(b2, "%-12.2f", 123.45);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%5.2f", 1.5);
  sprintf(b2, "%5.2f", 1.5);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%1.2f", 3.14);
  sprintf(b2, "%1.2f", 3.14);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_f_zero) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%012.2f", 123.45);
  sprintf(b2, "%012.2f", 123.45);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%012.2f", -123.45);
  sprintf(b2, "%012.2f", -123.45);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%08.3f", 3.14);
  sprintf(b2, "%08.3f", 3.14);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_f_signs) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%+.2f", 123.45);
  sprintf(b2, "%+.2f", 123.45);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%+.2f", -123.45);
  sprintf(b2, "%+.2f", -123.45);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "% .2f", 123.45);
  sprintf(b2, "% .2f", 123.45);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "% .2f", -123.45);
  sprintf(b2, "% .2f", -123.45);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_f_special) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%f", INFINITY);
  sprintf(b2, "%f", INFINITY);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%f", -INFINITY);
  sprintf(b2, "%f", -INFINITY);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%f", NAN);
  sprintf(b2, "%f", NAN);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%010f", INFINITY);
  sprintf(b2, "%010f", INFINITY);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%f", -0.0);
  sprintf(b2, "%f", -0.0);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_f_rounding) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%.0f", 2.5);
  sprintf(b2, "%.0f", 2.5);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.0f", 3.5);
  sprintf(b2, "%.0f", 3.5);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.1f", 0.25);
  sprintf(b2, "%.1f", 0.25);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.1f", 0.35);
  sprintf(b2, "%.1f", 0.35);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.2f", 0.005);
  sprintf(b2, "%.2f", 0.005);
  ck_assert_str_eq(b1, b2);
}
END_TEST

/* ============================================================= */
/* =================== Комбинированные тесты =================== */
/* ============================================================= */

START_TEST(test_sprintf_multiple) {
  char b1[300] = {0};
  char b2[300] = {0};

  s21_sprintf(b1, "%d + %d = %d", 1, 2, 3);
  sprintf(b2, "%d + %d = %d", 1, 2, 3);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%s = %d, %c = %.2f", "x", 10, 'Y', 3.14);
  sprintf(b2, "%s = %d, %c = %.2f", "x", 10, 'Y', 3.14);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%-5d|%5d|%05d|%.3d", 1, 2, 3, 4);
  sprintf(b2, "%-5d|%5d|%05d|%.3d", 1, 2, 3, 4);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%+d, % d, %u, %u", 5, 5, 5u, 5u);
  sprintf(b2, "%+d, % d, %u, %u", 5, 5, 5u, 5u);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_sprintf_return_value) {
  char b1[200] = {0};
  char b2[200] = {0};

  int r1 = s21_sprintf(b1, "Hello, %s! Number: %d", "world", 42);
  int r2 = sprintf(b2, "Hello, %s! Number: %d", "world", 42);
  ck_assert_int_eq(r1, r2);
  ck_assert_str_eq(b1, b2);

  r1 = s21_sprintf(b1, "%5d", 42);
  r2 = sprintf(b2, "%5d", 42);
  ck_assert_int_eq(r1, r2);

  r1 = s21_sprintf(b1, "%.3f", 3.14);
  r2 = sprintf(b2, "%.3f", 3.14);
  ck_assert_int_eq(r1, r2);

  b1[0] = '\0';
  r1 = s21_sprintf(b1, "");
  ck_assert_int_eq(r1, 0);
  ck_assert_str_eq(b1, "");
}
END_TEST

START_TEST(test_sprintf_edge_cases) {
  char b1[200] = {0};
  char b2[200] = {0};

  s21_sprintf(b1, "%1d", 123456);
  sprintf(b2, "%1d", 123456);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.0s", "abcdef");
  sprintf(b2, "%.0s", "abcdef");
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%-+10.3d", 42);
  sprintf(b2, "%-+10.3d", 42);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%010d", 42);
  sprintf(b2, "%010d", 42);
  ck_assert_str_eq(b1, b2);
}
END_TEST

/* ============================================================= */
/* ==================== Регистрация тестов ===================== */
/* ============================================================= */

static void add_mem_tests(TCase* tc) {
  tcase_add_test(tc, test_memchr);
  tcase_add_test(tc, test_memcmp);
  tcase_add_test(tc, test_memcpy);
  tcase_add_test(tc, test_memset);
}

static void add_str_tests(TCase* tc) {
  tcase_add_test(tc, test_strlen);
  tcase_add_test(tc, test_strchr);
  tcase_add_test(tc, test_strrchr);
  tcase_add_test(tc, test_strncmp);
  tcase_add_test(tc, test_strncpy);
  tcase_add_test(tc, test_strncat);
  tcase_add_test(tc, test_strcspn);
  tcase_add_test(tc, test_strpbrk);
  tcase_add_test(tc, test_strstr);
  tcase_add_test(tc, test_strtok);
  tcase_add_test(tc, test_strerror);
}

static void add_sprintf_basic_tests(TCase* tc) {
  tcase_add_test(tc, test_sprintf_no_args);
  tcase_add_test(tc, test_sprintf_percent);
  tcase_add_test(tc, test_sprintf_char);
  tcase_add_test(tc, test_sprintf_string);
}

static void add_sprintf_int_tests(TCase* tc) {
  tcase_add_test(tc, test_sprintf_d_basic);
  tcase_add_test(tc, test_sprintf_d_width);
  tcase_add_test(tc, test_sprintf_d_minus);
  tcase_add_test(tc, test_sprintf_d_precision);
  tcase_add_test(tc, test_sprintf_d_width_precision);
  tcase_add_test(tc, test_sprintf_d_zero);
  tcase_add_test(tc, test_sprintf_d_plus_space);
  tcase_add_test(tc, test_sprintf_d_length_h);
  tcase_add_test(tc, test_sprintf_d_length_l);
  tcase_add_test(tc, test_sprintf_u_basic);
  tcase_add_test(tc, test_sprintf_u_flags);
  tcase_add_test(tc, test_sprintf_u_length);
}

static void add_sprintf_float_tests(TCase* tc) {
  tcase_add_test(tc, test_sprintf_f_basic);
  tcase_add_test(tc, test_sprintf_f_precision);
  tcase_add_test(tc, test_sprintf_f_width);
  tcase_add_test(tc, test_sprintf_f_zero);
  tcase_add_test(tc, test_sprintf_f_signs);
  tcase_add_test(tc, test_sprintf_f_special);
  tcase_add_test(tc, test_sprintf_f_rounding);
}

static void add_sprintf_combo_tests(TCase* tc) {
  tcase_add_test(tc, test_sprintf_multiple);
  tcase_add_test(tc, test_sprintf_return_value);
  tcase_add_test(tc, test_sprintf_edge_cases);
}

int main(void) {
  Suite* s = suite_create("s21_string");

  TCase* tc_mem = tcase_create("mem");
  TCase* tc_str = tcase_create("str");
  TCase* tc_sprintf_basic = tcase_create("sprintf_basic");
  TCase* tc_sprintf_int = tcase_create("sprintf_int");
  TCase* tc_sprintf_float = tcase_create("sprintf_float");
  TCase* tc_sprintf_combo = tcase_create("sprintf_combo");

  add_mem_tests(tc_mem);
  add_str_tests(tc_str);
  add_sprintf_basic_tests(tc_sprintf_basic);
  add_sprintf_int_tests(tc_sprintf_int);
  add_sprintf_float_tests(tc_sprintf_float);
  add_sprintf_combo_tests(tc_sprintf_combo);

  suite_add_tcase(s, tc_mem);
  suite_add_tcase(s, tc_str);
  suite_add_tcase(s, tc_sprintf_basic);
  suite_add_tcase(s, tc_sprintf_int);
  suite_add_tcase(s, tc_sprintf_float);
  suite_add_tcase(s, tc_sprintf_combo);

  SRunner* sr = srunner_create(s);
  srunner_run_all(sr, CK_VERBOSE);
  int failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return failed == 0 ? 0 : 1;
}