#ifndef __UIX_LABEL_H__
#define __UIX_LABEL_H__ 1

#include <cairo.h>
#include <uix/geom.h>

struct label_t {
  struct rect_t frame;
  struct argb_t foreground;
  struct argb_t background;
  char* text;
  float padding;
};

void label_change_text(struct label_t* s, const char* text);

void label_render(cairo_t* context, struct label_t* l);
void label_render_centered_to(cairo_t* context, struct label_t* l, struct rect_t* f);

#endif // __UIX_LABEL_H__
