.PHONY: build
build:
	gcc -o webserver.o server.c


.PHONY: run
run: build
	./webserver.o


DEFAULT_GOAL := build
