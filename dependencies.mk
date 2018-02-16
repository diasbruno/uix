DEPENDENCIES_CFLAGS=$(shell pkg-config --cflags cairo xcb)
DEPENDENCIES_LDFLAGS=$(shell pkg-config --libs cairo xcb)
