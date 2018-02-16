#ifndef __UIX_PROGRESS_H__
#define __UIX_PROGRESS_H__ 1

#include <cairo.h>
#include <uix/geom.h>

struct progress_t {
  struct rect_t frame;
  struct argb_t foreground;
  struct argb_t background;
  float progress, padding;
};

void progress_progress_x(struct progress_t* s, float x);

void progress_render(cairo_t* context, struct progress_t* s);

#endif // __UIX_PROGRESS_H__
