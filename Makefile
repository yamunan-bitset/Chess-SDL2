CC = gcc
CFLAGS = -g -lSDL2 -lSDL2_image

ifeq ($(OS), Windows_NT)
	CFLAGS += -Lx86-windows\bin -Ix86-windows\include -o x86-windows\bin\a.exe
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
        CFLAGS += -o Chess.out
    endif
endif

SRC = main.c

all: $(SRC)
	$(CC) $(SRC) $(CFLAGS)