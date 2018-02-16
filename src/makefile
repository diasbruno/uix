CC=$(shell which clang)
CCX=$(CC)++

SOURCE=$(wildcard *.c)
OBJECTS=$(SOURCE:%.c=%.o)

include ../dependencies.mk

PROJECT_CFLAGS=-I../include
CFLAGS=-Wall -Werror $(PROJECT_CFLAGS)
CFLAGS+= $(DEPENDENCIES_CFLAGS)

LDFLAGS=$(DEPENDENCIES_LDFLAGS)

%.o: %.c
	$(CC) -g -c $(CFLAGS) $< -o $@

libuix: $(OBJECTS)
	$(CC) -g $(CFLAGS) -o $@ $^ $(LDFLAGS)

all: libuix

clean:
	-rm -rf libuix *.o
