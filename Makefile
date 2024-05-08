.PHONY: all

all: App

vpath %.c src/app

main.o: main.c
	gcc -Isrc -c -o obj/main.o src/app/main.c
App: main.o 
	gcc -Isrc -o bin/App -Wall -Werror obj/main.o