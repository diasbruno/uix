#include <cairo.h>

#include <uix/toggle.h>

void toggle_change_state(struct toggle_t* s) {
  s->state = !s->state;
}

void toggle_render(cairo_t* context, struct toggle_t* t) {
  int16_t pa = t->padding;
  int16_t pa2 = pa * 2;

  struct argb_t* bg   = &(t->background);
  struct argb_t* fg   = &(t->foreground);
  struct vec_t*  p    = &(t->frame.position);
  struct size_t* size = &(t->frame.size);

  cairo_set_source_rgba(context,
                        bg->r, bg->g, bg->b, bg->a);
  cairo_rectangle(context,
                  p->x, p->y,
                  size->w, size->h);
  cairo_fill(context);

  if (t->state) {
    cairo_save(context);
    cairo_set_source_rgba(context,
                          fg->r, fg->g, fg->b, fg->a);
    cairo_rectangle(context,
                    p->x + pa, p->y + pa,
                    size->w - pa2, size->h - pa2);
    cairo_fill(context);
    cairo_restore(context);
  }
}
