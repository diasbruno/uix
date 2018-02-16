#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <uix/definitions.h>
#include <uix/geom.h>
#include <uix/layout/row.h>

int main(int UNUSED(argc), char* UNUSED(argv[])) {
  static struct row_layout_t s = { .padding = 0 };

  static struct rect_t a = RECT_FRAME(0, 0, 10, 10);
  static struct rect_t b = RECT_FRAME(0, 0, 10, 20);
  static struct rect_t c = RECT_FRAME(0, 0, 10, 10);

  struct rect_t* fs[3] = { &a, &b, &c };

  row_layout_format(&s, (struct rect_t**)&fs, 3);

  assert(a.position.x ==  0 && a.position.y == 0);
  assert(b.position.x == 10 && b.position.y == 0);
  assert(c.position.x == 20 && c.position.y == 0);

  return 0;
}
