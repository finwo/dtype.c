SRC=$(wildcard src/*.c)

INCLUDES?=
INCLUDES+=-I src

override CFLAGS?=-Wall -std=c99

include lib/.dep/config.mk

override CFLAGS+=$(INCLUDES)
override CFLAGS+=-D_DEFAULT_SOURCE

OBJ=$(SRC:.c=.o)

BIN=\
	benchmark \
	test

default: README.md $(BIN) libdtype.a libdtype.so

libdtype.a: $(OBJ)
	ar rcs $@ $^

libdtype.so: $(OBJ)
	$(CC) $(OBJ) --shared -o $@

$(BIN): $(OBJ) $(BIN:=.o)
	$(CC) $(CFLAGS) $(OBJ) $@.o -o $@

.PHONY: clean
clean:
	rm -f $(OBJ)
	rm -f $(BIN:=.o)
	rm -f test

README.md: ${SRC} src/dtype.h
	stddoc < src/dtype.h > README.md
