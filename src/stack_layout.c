#include <uix/layout/stack.h>

void stack_layout_format(struct stack_layout_t* l, struct rect_t** frames, int size) {
  float next_position = l->padding;
  for (int i = 0; i < size; i++) {
    struct rect_t* r = *(frames + i);
    r->position.x = l->padding;
    r->position.y = l->padding * i + next_position;
    next_position += r->size.h;
  }
}
