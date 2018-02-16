#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <cairo.h>

#include <uix/geom.h>
#include <uix/progress.h>

void progress_progress_x(struct progress_t* s, float x) {
  s->progress = fmin(fmax(0, x / s->frame.size.w), 1);
}

void progress_render(cairo_t* context, struct progress_t* s) {
  int16_t pa = s->padding;
  int16_t pa2 = pa * 2;

  struct argb_t* bg   = &(s->background);
  struct argb_t* fg   = &(s->foreground);
  struct vec_t*  p    = &(s->frame.position);
  struct size_t* size = &(s->frame.size);

  cairo_set_source_rgba(context,
                        bg->r, bg->g, bg->b, bg->a);
  cairo_rectangle(context,
                  p->x, p->y,
                  size->w, size->h);
  cairo_fill(context);

  cairo_set_source_rgba(context,
                        fg->r, fg->g, fg->b, fg->a);
  cairo_rectangle(context,
                  p->x + pa, p->y + pa,
                  size->w * s->progress - pa2, size->h - pa2);
  cairo_fill(context);
}
