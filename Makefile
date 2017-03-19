CC=gcc
C_FLAGS=-c

LD=ld

STYLE=astyle

all: parse_pcap main build

parse_pcap:
	${CC} ${C_FLAGS} parse_pcap.c 

main:
	${CC} ${C_FLAGS} main.c

build:
	${CC} main.o parse_pcap.o -o main.out

clean:
	rm *.o *.out

style:
	${STYLE} --style=kr --indent=spaces=4 -S -H -U -p --suffix=none --recursive "*.c" "*.h"
