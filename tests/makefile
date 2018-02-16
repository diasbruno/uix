CC=$(shell which clang)
CCX=$(CC)++

SOURCE=$(wildcard *.c)
OBJECTS=$(SOURCE:%.c=%)

include ../dependencies.mk

PROJECT_CFLAGS=-I../include
CFLAGS=-Wall -Werror $(PROJECT_CFLAGS)
CFLAGS+= $(DEPENDENCIES_CFLAGS)

LDFLAGS=$(DEPENDENCIES_LDFLAGS) -L.. -luix

%: %.c
	$(CC) -g $(CFLAGS) $< -o $@ $(LDFLAGS)

all: $(OBJECTS)

clean:
	-rm -rf $(OBJECTS)

