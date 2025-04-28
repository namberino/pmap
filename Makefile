CC = $(shell which gcc)
CC ?= $(shell which clang)

C_SRC = $(wildcard src/*.c)
HEADERS = $(wildcard include/*.h)
OBJ = $(patsubst src/%.c, obj/%.o)
OBJ_GRAPHICS = $(patsubst src/%.c, obj/%.o)

CFLAGS = -std=c11 -Wall -pedantic -Iinclude

compile: bin/pmap

run: compile
	./bin/pmap

bin/pmap: $(OBJ) | bin
	"$(CC)" -o $@ $(OBJ)

bin:
	mkdir -p bin

obj/%.o: src/%.c $(HEADERS) | obj
	"${CC}" $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj

clean:
	rm -f bin/* obj/*.o

.PHONY: clean run compile