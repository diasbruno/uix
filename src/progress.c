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

void progress_render_arc(cairo_t* context, struct progress_t* s) {
  struct argb_t* bg   = &(s->background);
  struct vec_t*  p    = &(s->frame.position);
  struct size_t* size = &(s->frame.size);

  cairo_save(context);
  cairo_set_source_rgba(context,
                        1, 1, 1, 1);
  cairo_rectangle(context,
                  p->x,
                  p->y,
                  size->w,
                  size->h);
  cairo_fill(context);
  /*
  cairo_set_source_rgba(context, 0, 0, 0, 1);
  cairo_rectangle(context,
                  p->x,
                  p->y,
                  size->w,
                  size->h);
  cairo_stroke(context);
  */

  cairo_set_source_rgba(context,
                        bg->r, bg->g, bg->b, bg->a);
  cairo_translate(context,
                  (p->x + size->w * 0.5),
                  (p->y + size->h * 0.5));
  cairo_set_line_width(context, 5);
  cairo_arc(context,
            0, 0, ((size->w - 5) * 0.5), M_PI,
            (M_PI + s->progress * 2 * M_PI));
  cairo_stroke(context);
  cairo_restore(context);
}
