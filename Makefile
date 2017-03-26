CC=gcc

LD=ld
STYLE=astyle

CFLAGS = -fPIC -Wall -Wextra -O0 -g # C flags

OBJ_DIR = elf

DIR=net
SRC = $(wildcard ${DIR}/*.c) main.c parse_pcap.c
INC = $(wildcard ${DIR}/*.h) parse_pcap.h
OBJ = $(SRC:.c=.o)

BIN=main.out

all: build

build: ${OBJ}
	${CC} $^ -o ${BIN}

clean:
	rm *.o *.out

style:
	${STYLE} --style=kr --indent=spaces=4 -S -H -U -p --suffix=none --recursive "*.c" "*.h"

print:
	echo ${OBJ}
