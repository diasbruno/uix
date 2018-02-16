#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cairo.h>

#include <uix/button.h>

void button_render(cairo_t* context, struct button_t* b) {
  // int16_t pa = b->padding;
  // int16_t pa2 = pa * 2;

  struct argb_t* bg   = &(b->background);
  // struct argb_t* fg   = &(b->foreground);
  struct vec_t*  p    = &(b->frame.position);
  struct size_t* size = &(b->frame.size);

  cairo_set_source_rgba(context,
                        bg->r, bg->g, bg->b, bg->a);
  cairo_rectangle(context,
                  p->x, p->y,
                  size->w, size->h);
  cairo_fill(context);

  cairo_save(context);
  cairo_translate(context, p->x, p->y);
  label_render_centered_to(context, &(b->label), &(b->frame));
  cairo_restore(context);
}
