#ifndef __UIX_SLIDER_H__
#define __UIX_SLIDER_H__ 1

#include <cairo.h>
#include <uix/geom.h>

struct slider_t {
  struct rect_t frame;
  struct argb_t foreground;
  struct argb_t background;
  float progress, padding;
};

void slider_progress_x(struct slider_t* s, float x);

void slider_render(cairo_t* context, struct slider_t* s);

#endif // __UIX_SLIDER_H__
