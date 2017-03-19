CC=gcc
CC_OPT=-c

LD=ld

all: parse_pcap main

parse_pcap:
	${CC} ${CC_OPT} parse_pcap.c 

main:
	${CC} ${CC_OPT} main.c

build:
	${LD} main.c parse_pcap.c -o main.out

clean:
	rm *.o *.out
