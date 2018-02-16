#include <stdio.h>
#include <cairo.h>

#include <uix/label.h>

// static inline void text_extends_dump(cairo_text_extents_t* e) {
//   printf("Text extends:\n");
//   printf("  Size: %f %f\n", e->width, e->height);
//   printf("  Advance: %f %f\n", e->x_advance, e->y_advance);
//   printf("  Bearing: %f %f\n", e->x_bearing, e->y_bearing);
// }

/**
 * Change the text of the label.
 * @param {struct label_t*} l       The label to render.
 * @param {const char*}     text    The new text.
 */
void label_change_text(struct label_t* s, const char* text) {
  s->text = (char*)text;
}

/**
 * Renders the label.
 * The rendered text will be vertically aligned to the frame
 * defined on the element and not aligned horizontally.
 * @param {cairo_t*}        context The cairo context.
 * @param {struct label_t*} l       The label to render.
 */
void label_render(cairo_t* context, struct label_t* l) {
  struct vec_t*  p  = &(l->frame.position);
  struct size_t* s  = &(l->frame.size);
  struct argb_t* bg = &(l->background);
  struct argb_t* fg = &(l->foreground);

  cairo_text_extents_t extents;
  cairo_text_extents(context, l->text, &extents);

  cairo_set_source_rgba(context,
                        bg->r, bg->g, bg->b, bg->a);
  cairo_rectangle(context,
                  p->x, p->y,
                  s->w, s->h);
  cairo_fill(context);

  cairo_set_source_rgba(context,
                        fg->r, fg->g, fg->b, fg->a);
  cairo_move_to(context, p->x,  p->y + (s->h * 0.5) - (extents.y_bearing * 0.5));
  cairo_show_text(context, l->text);
}

/**
 * Renders the label centered to a given `struct rect_t`.
 * The label is always aligned vertically, so we just have
 * to align horizontally according to the rectangle.
 * @param {cairo_t*}        context The cairo context.
 * @param {struct label_t*} l       The label to render.
 * @param {struct rect_t*}  f       The rectangle frame to align to.
 *
 * @used
 * - button.c
 */
void label_render_centered_to(cairo_t* context, struct label_t* l, struct rect_t* f) {
  struct vec_t*  p  = &(l->frame.position);
  struct size_t* s  = &(l->frame.size);
  struct argb_t* fg = &(l->foreground);

  cairo_text_extents_t extents;
  cairo_text_extents(context, l->text, &extents);

  cairo_set_source_rgba(context,
                        fg->r, fg->g, fg->b, fg->a);
  cairo_move_to(context,
                (f->size.w * 0.5) - (extents.width * 0.5),
                p->y + (s->h * 0.5) - (extents.y_bearing * 0.5));
  cairo_show_text(context, l->text);
}
