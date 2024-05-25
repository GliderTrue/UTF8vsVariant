.PHONY: all

all: App

vpath %.c src/app
vpath %.c src/variant
vpath %.h src/variant


variant.o: variant.c
	gcc -Isrc -c -o obj/variant.o src/variant/variant.c

main.o: main.c
	gcc -Isrc -c -o obj/main.o src/app/main.c
App: main.o variant.o
	gcc -Isrc -o bin/App -Wall -Werror obj/main.o obj/variant.o