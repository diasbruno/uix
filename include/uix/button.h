#ifndef __UIX_BUTTON_H__
#define __UIX_BUTTON_H__ 1

#include <cairo.h>
#include <uix/geom.h>
#include <uix/label.h>

struct button_t {
  struct rect_t frame;
  struct argb_t foreground;
  struct argb_t background;
  struct label_t label;
  float padding;
};

void button_progress_x(struct button_t* s, float x);

void button_render(cairo_t* context, struct button_t* s);

#endif // __UIX_BUTTON_H__
