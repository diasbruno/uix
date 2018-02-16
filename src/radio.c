#include <math.h>
#include <cairo.h>

#include <uix/radio.h>

void radio_change_state(struct radio_t* s) {
  s->state = !s->state;
}

void radio_render(cairo_t* context, struct radio_t* t) {
  struct argb_t* bg   = &(t->background);
  struct argb_t* fg   = &(t->foreground);
  struct vec_t*  p    = &(t->frame.position);
  struct size_t* size = &(t->frame.size);

  cairo_save(context);
  cairo_set_source_rgba(context,
                        1, 1, 1, 1);
  cairo_rectangle(context,
                  p->x, p->y, size->w, size->h);
  cairo_fill(context);

  cairo_set_source_rgba(context,
                        bg->r, bg->g, bg->b, bg->a);
  cairo_translate(context,
                  p->x + size->w * 0.5,
                  p->y + size->h * 0.5);
  cairo_arc(context,
            0, 0, size->w * 0.5, 0, 2 * M_PI);
  cairo_fill(context);

  if (t->state) {
    cairo_set_source_rgba(context,
                          fg->r, fg->g, fg->b, fg->a);
    cairo_arc(context,
              0, 0, size->w * 0.3, 0,  2 * M_PI);
    cairo_fill(context);
  }
  cairo_restore(context);
}
