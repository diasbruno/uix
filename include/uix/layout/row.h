#ifndef __UIX_LAYOUT_STACK_H__
#define __UIX_LAYOUT_STACK_H__ 1

#include <uix/geom.h>

struct row_layout_t {
  float padding;
};

void row_layout_format(struct row_layout_t* l,
                       struct rect_t** frames,
                       int size);

#endif // __UIX_LAYOUT_STACK_H__
