MAKE=$(shell which make)
AR=$(shell which ar)

libuix:
	$(MAKE) -C src -k all

example:
	$(MAKE) -C examples -k all

test: libuix.a
	$(MAKE) -C tests -k all

libuix.a: libuix
	$(AR) crTv libuix.a src/*.o

clean-libuix:
	$(MAKE) -C src -k clean

clean-examples:
	$(MAKE) -C examples -k clean

clean-test:
	$(MAKE) -C tests -k clean

clean: clean-libuix clean-examples clean-test

all: libuix libuix.a example test
