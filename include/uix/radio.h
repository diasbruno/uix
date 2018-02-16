#ifndef __UIX_RADIO_H__
#define __UIX_RADIO_H__ 1

#include <cairo.h>
#include <uix/geom.h>

struct radio_t {
  struct rect_t frame;
  struct argb_t foreground;
  struct argb_t background;
  float state, padding;
};

void radio_change_state(struct radio_t* s);

void radio_render(cairo_t* context, struct radio_t* s);

#endif // __UIX_RADIO_H__
