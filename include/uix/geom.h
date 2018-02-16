#ifndef __UIX_GEOM_H__
#define __UIX_GEOM_H__ 1

#include <stdlib.h>

#define RECT_FRAME(px, py, pw, ph)              \
  { .position = { .x = px, .y = py },           \
    .size = { .w = pw, .h = ph }                \
  }
#define ARGB(ca, cr, cg, cb)                    \
  { .a = ca, .r = cr, .g = cg, .b = cb }
#define ARGB_GREY(ca, cg)                       \
  { .a = ca, .r = cg, .g = cg, .b = cg }

struct argb_t { float a, r, g, b; };

struct rgb_t { float r, g, b; };

typedef struct rgb_t color_t;

struct vec_t { float x, y; };

struct size_t { float w, h; };

struct rect_t {
  struct vec_t position;
  struct size_t size;
};

int rect_point_is_inside(struct rect_t* r, float x, float y);

inline void rect_update_x(struct rect_t* r, float x) {
  r->position.x = x;
}

inline void rect_update_y(struct rect_t* r, float y) {
  r->position.y = y;
}

inline void rect_update_width(struct rect_t* r, float w) {
  r->size.w = w;
}

inline void rect_update_height(struct rect_t* r, float h) {
  r->size.h = h;
}

inline void rect_update_position(struct rect_t* r,
                                 float x, float y) {
  rect_update_x(r, x);
  rect_update_y(r, y);
}

inline void rect_update_size(struct rect_t* r,
                             float width, float height) {
  rect_update_width(r, width);
  rect_update_height(r, height);
}

inline void rect_update(struct rect_t* r,
                        float x, float y,
                        float w, float h) {
  rect_update_position(r, x, y);
  rect_update_size(r, w, h);
}

inline struct rect_t* rect_create(float x, float y, float w, float h) {
  struct rect_t* r = (struct rect_t*)malloc(sizeof(struct rect_t));
  rect_update(r, x, y, w, h);
  return r;
}

void rect_log(struct rect_t* r);
void argb_log(struct argb_t* c);

#endif // __UIX_GEOM_H__
