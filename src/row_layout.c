#include <uix/layout/row.h>

void row_layout_format(struct row_layout_t* l, struct rect_t** frames, int size) {
  float padding = l->padding;
  float next_position = padding;
  for (int i = 0; i < size; i++) {
    struct rect_t* r = *(frames + i);
    r->position.x = padding * i + next_position;
    r->position.y = padding;
    next_position += r->size.w;
  }
}
