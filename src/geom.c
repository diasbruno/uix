#include <stdio.h>

#include <uix/geom.h>

void argb_update_red(struct argb_t* c, float v) { c->r = v; }
void argb_update_green(struct argb_t* c, float v) { c->g = v; }
void argb_update_blue(struct argb_t* c, float v) { c->b = v; }
void argb_update_alpha(struct argb_t* c, float v) { c->b = v; }

void argb_update(struct argb_t* c, float r, float g, float b, float a) {
  argb_update_red(c, r);
  argb_update_red(c, g);
  argb_update_red(c, b);
  argb_update_red(c, a);
}

int rect_point_is_inside(struct rect_t* r, float x, float y) {
  struct vec_t*  p  = &(r->position);
  struct size_t* si = &(r->size);
  return
    x >= p->x && y >= p->y &&
    x <= (si->w + p->x) && y <= (si->h + p->y);
}

void rect_log(struct rect_t* r) {
  struct vec_t*  p = &(r->position);
  struct size_t* s = &(r->size);
  printf("Rect: (%f, %f) size: (%f, %f)\n", p->x, p->y, s->w, s->h);
}

void vec_log(struct vec_t* v) {
  printf("Vec: (%f, %f)\n", v->x, v->y);
}

void size_log(struct size_t* s) {
  printf("Size: (%f, %f)\n", s->w, s->h);
}

void argb_log(struct argb_t* c) {
  printf("ARGB: %f, %f, %f, %f\n", c->r, c->g, c->b, c->a);
}
