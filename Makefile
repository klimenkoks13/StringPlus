CC          := gcc
CFLAGS      := -Wall -Wextra -Werror -std=c11

LIB         := s21_string.a

SRCS        := s21_mem.c s21_str.c s21_strerror.c \
               s21_sprintf/s21_apply_sign.c \
               s21_sprintf/s21_convert_and_format.c \
               s21_sprintf/s21_output_padding.c \
               s21_sprintf/s21_parse_format.c \
               s21_sprintf/s21_sprintf.c

OBJS        := s21_mem.o s21_str.o s21_strerror.o \
               s21_sprintf/s21_apply_sign.o \
               s21_sprintf/s21_convert_and_format.o \
               s21_sprintf/s21_output_padding.o \
               s21_sprintf/s21_parse_format.o \
               s21_sprintf/s21_sprintf.o

UNAME_S     := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    CHECK_FLAGS := -lcheck -lsubunit -lm -lpthread -lrt
else
    CHECK_FLAGS := -lcheck -lm
endif

.PHONY: all clean rebuild test gcov_report style

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)
	ranlib $(LIB)

s21_mem.o: s21_mem.c s21_string.h
	$(CC) $(CFLAGS) -c s21_mem.c -o s21_mem.o

s21_str.o: s21_str.c s21_string.h
	$(CC) $(CFLAGS) -c s21_str.c -o s21_str.o

s21_strerror.o: s21_strerror.c s21_string.h s21_errors.h
	$(CC) $(CFLAGS) -c s21_strerror.c -o s21_strerror.o

s21_sprintf/s21_apply_sign.o: s21_sprintf/s21_apply_sign.c s21_string.h s21_sprintf/s21_sprintf.h
	$(CC) $(CFLAGS) -c s21_sprintf/s21_apply_sign.c -o s21_sprintf/s21_apply_sign.o

s21_sprintf/s21_convert_and_format.o: s21_sprintf/s21_convert_and_format.c s21_string.h s21_sprintf/s21_sprintf.h
	$(CC) $(CFLAGS) -c s21_sprintf/s21_convert_and_format.c -o s21_sprintf/s21_convert_and_format.o

s21_sprintf/s21_output_padding.o: s21_sprintf/s21_output_padding.c s21_string.h s21_sprintf/s21_sprintf.h
	$(CC) $(CFLAGS) -c s21_sprintf/s21_output_padding.c -o s21_sprintf/s21_output_padding.o

s21_sprintf/s21_parse_format.o: s21_sprintf/s21_parse_format.c s21_string.h s21_sprintf/s21_sprintf.h
	$(CC) $(CFLAGS) -c s21_sprintf/s21_parse_format.c -o s21_sprintf/s21_parse_format.o

s21_sprintf/s21_sprintf.o: s21_sprintf/s21_sprintf.c s21_string.h s21_sprintf/s21_sprintf.h
	$(CC) $(CFLAGS) -c s21_sprintf/s21_sprintf.c -o s21_sprintf/s21_sprintf.o

# --- тесты ---
test: $(LIB)
	$(CC) $(CFLAGS) -Is21_sprintf test_s21_string.c -L. -l:$(LIB) $(CHECK_FLAGS) -o test_bin
	./test_bin

# --- HTML-отчёт о покрытии ---
gcov_report: clean
	@mkdir -p coverage
	gcc $(CFLAGS) -fprofile-arcs -ftest-coverage -Is21_sprintf \
	    $(SRCS) test_s21_string.c \
	    $(CHECK_FLAGS) -o coverage/s21_test_cov
	./coverage/s21_test_cov
	gcov -o coverage $(SRCS) > /dev/null 2>&1 || true
	gcovr --root . --html --html-details -o coverage/report.html \
	    --exclude 'test_.*\.c$$' --exclude '.*\.h$$'
	@echo "Отчёт покрытия: coverage/report.html"

# --- проверка стиля ---
style:
	clang-format -n --style=Google *.c *.h s21_sprintf/*.c s21_sprintf/*.h \
	    || echo "clang-format не найден"

clean:
	rm -f *.o *.a *.gcda *.gcno *.gcov test_bin
	rm -f s21_sprintf/*.o s21_sprintf/*.gcda s21_sprintf/*.gcno
	rm -rf build coverage

rebuild: clean all