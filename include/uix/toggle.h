#ifndef __UIX_TOGGLE_H__
#define __UIX_TOGGLE_H__ 1

#include <cairo.h>
#include <uix/geom.h>

struct toggle_t {
  struct rect_t frame;
  struct argb_t foreground;
  struct argb_t background;
  float state, padding;
};

void toggle_change_state(struct toggle_t* s);

void toggle_render(cairo_t* context, struct toggle_t* s);

#endif // __UIX_TOGGLE_H__
