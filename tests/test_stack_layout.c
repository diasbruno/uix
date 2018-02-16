#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <uix/definitions.h>
#include <uix/geom.h>
#include <uix/layout/stack.h>

int main(int UNUSED(argc), char* UNUSED(argv[])) {
  static struct stack_layout_t s = { .padding = 10 };

  static struct rect_t a = RECT_FRAME(0, 0, 10, 10);
  static struct rect_t b = RECT_FRAME(0, 0, 10, 20);
  static struct rect_t c = RECT_FRAME(0, 0, 10, 10);

  struct rect_t* fs[3] = { &a, &b, &c };

  stack_layout_format(&s, (struct rect_t**)&fs, 3);

  assert(a.position.x == 10 && a.position.y == 10);
  assert(b.position.x == 10 && b.position.y == 30);
  assert(c.position.x == 10 && c.position.y == 60);

  return 0;
}
