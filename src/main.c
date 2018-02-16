#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <poll.h>
#include <xcb/xcb.h>
#include <cairo.h>
#include <cairo-xcb.h>

#include <uix/uix.h>

static struct label_t labelui = {
  RECT_FRAME(10, 10, 150, 30),
  ARGB_GREY(1, 0.5),
  ARGB(1, 1, 1, 1),
  .text = 0, .padding = 5
};

static struct progress_t progressui = {
  RECT_FRAME(10, 40, 150, 30),
  ARGB(1, 1, 1, 1),
  ARGB_GREY(1, 0.5),
  .progress = 0.5, .padding = 5
};

static struct toggle_t toggleui = {
  RECT_FRAME(10, 100, 30, 30),
  ARGB(1, 1, 1, 1),
  ARGB_GREY(1, 0.5),
  .padding = 5, .state = 0
};

static struct button_t buttonui = {
  RECT_FRAME(10, 150, 100, 30),
  ARGB(1, 1, 1, 1),
  ARGB_GREY(1, 0.5),
  .label = {
    RECT_FRAME(0, 0, 150, 30),
    ARGB(1, 1, 1, 1),
    ARGB_GREY(1, 0.5),
    .text = "test", .padding = 5
  },
  .padding = 5
};

static xcb_connection_t *connection;
static cairo_surface_t *surface;
static cairo_t *cr;

static xcb_visualtype_t* find_visual(xcb_connection_t* connection, xcb_visualid_t visual) {
  xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(connection));

  for (; screen_iter.rem; xcb_screen_next(&screen_iter)) {
    xcb_depth_iterator_t depth_iter = xcb_screen_allowed_depths_iterator(screen_iter.data);
    for (; depth_iter.rem; xcb_depth_next(&depth_iter)) {
      xcb_visualtype_iterator_t visual_iter = xcb_depth_visuals_iterator(depth_iter.data);
      for (; visual_iter.rem; xcb_visualtype_next(&visual_iter))
        if (visual == visual_iter.data->visual_id)
          return visual_iter.data;
    }
  }

  return NULL;
}

static inline int event_count(xcb_expose_event_t* event) {
  return event->count;
}

void render(void) {
  static struct stack_layout_t s = {
    .padding = 10
  };

  struct rect_t* fs[4] = {
    &(progressui.frame),
    &(toggleui.frame),
    &(labelui.frame),
    &(buttonui.frame)
  };

  stack_layout_format(&s, (struct rect_t**)&fs, 4);

  progress_render(cr, &progressui);
  toggle_render(cr, &toggleui);
  label_render(cr, &labelui);
  button_render(cr, &buttonui);

  cairo_surface_flush(surface);
}

void event_motion_notify(void* context, xcb_generic_event_t* event) {
  struct progress_t* si  = context;
  xcb_motion_notify_event_t* e = (xcb_motion_notify_event_t*)event;
  float x = e->event_x, y = e->event_y;
  int inside = rect_point_is_inside(&(si->frame), x, y);

  if (inside) {
    progress_progress_x(si, x);
  }

  fprintf(stdout, "mouse move at: %f %f\n", x, y);
  render();
}

void event_button_press(void* context, xcb_generic_event_t* event) {
  struct toggle_t* t = context;
  xcb_button_press_event_t *e = (xcb_button_press_event_t*)event;
  float x = e->event_x, y = e->event_y;
  int c = (int)e->detail;
  if (c == 1 && rect_point_is_inside(&(t->frame), x, y)) {
    toggle_change_state(t);
  }

  fprintf(stdout, "mouse button: %d at: %f %f\n", c, x, y);
  render();
}

void event_key_press(void* UNUSED(context), xcb_generic_event_t* event) {
  xcb_key_press_event_t* e = (xcb_key_press_event_t*)event;
  const float x = e->event_x, y = e->event_y;
  const char c = (char)e->detail;
  fprintf(stdout, "char: %c\n at: %f %f\n", c, x, y);
  render();
}

void event_expose(void* UNUSED(context), event_t* event) {
  if (event_count((xcb_expose_event_t*)event) != 0)
    return;

  render();
}

void event_idle(xcb_generic_event_t* UNUSED (event)) {
  printf("idle\n");
  render();
}

volatile sig_atomic_t gSignalStatus;

void loop(xcb_connection_t *connection) {
  xcb_allow_events(connection, XCB_ALLOW_REPLAY_POINTER, XCB_CURRENT_TIME);

  for (; gSignalStatus != SIGINT;) {
    xcb_generic_event_t *event;
    event = xcb_poll_for_event(connection);

    if (!event) {
      continue;
    }

    int type = event->response_type & ~0x80;
    event_handler_execute(type, event);

    free(event);
    xcb_flush(connection);
  }
}

void cleanup(xcb_connection_t* connection) {
  printf("clean up application.\n");
  if (connection) {
    event_handlers_terminate();
    cairo_surface_finish(surface);
    cairo_surface_destroy(surface);
    free(cr);
    xcb_disconnect(connection);
  }
}

void sighandle(int value) {
  fprintf(stdout, "received signal: %d\n", value);
  signal(SIGINT, sighandle);
  if (value == SIGINT || value == SIGTERM) {
    fprintf(stdout, "preparing for clean up.\n");
    fflush(stdout);
    gSignalStatus = value;
  }
}

void struct_stats(void) {
  printf("Struct stats --------------------------------\n");
  printf("  Colors:\n");
  printf("    RGB: %d\n", (int)sizeof(struct rgb_t));
  printf("    ARGB: %d\n\n", (int)sizeof(struct argb_t));
  printf("  Geometry:\n");
  printf("    Vec: %d\n", (int)sizeof(struct vec_t));
  printf("    Rect: %d\n\n", (int)sizeof(struct rect_t));
  printf("  Components:\n");
  printf("    Toggle: %d\n", (int)sizeof(struct toggle_t));
  printf("    Progress: %d\n", (int)sizeof(struct progress_t));
  printf("---------------------------------------------\n");
}

void screen_stats(xcb_screen_t * screen) {
  printf("\n");
  printf("Informations of screen %u:\n", screen->root);
  printf("  width.........: %u\n", screen->width_in_pixels);
  printf("  height........: %u\n", screen->height_in_pixels);
  printf("  white pixel...: %u\n", screen->white_pixel);
  printf("  black pixel...: %u\n", screen->black_pixel);
  printf("\n");
}

int main(int UNUSED(argc), char* UNUSED(argv[])) {
  struct_stats();

  // struct sigaction act;
  // act.sa_handler = sighandle;
  // act.sa_flags = 0;
  // sigemptyset(&act.sa_mask);
  // sigaction(SIGINT, &act, NULL);

  // struct sigaction act2;
  // act.sa_handler = sighandle;
  // sigaction(SIGQUIT, &act2, NULL);
  signal(SIGINT, sighandle);
  signal(SIGTERM, sighandle);

  int32_t i, screenNum;

  int16_t x = 0, y = 0;
  int16_t window_size = 200;
  int16_t width = window_size, height = window_size;
  int16_t border_width = 0;

  xcb_screen_t*     screen;
  xcb_window_t      window;
  xcb_visualtype_t* visual;
  uint32_t mask[2] = { 1,
                       XCB_EVENT_MASK_EXPOSURE     |
                       XCB_EVENT_MASK_BUTTON_PRESS |
                       XCB_EVENT_MASK_KEY_PRESS    |
                       XCB_EVENT_MASK_KEY_RELEASE  |
                       XCB_EVENT_MASK_POINTER_MOTION };

  connection = xcb_connect(NULL, &screenNum);
  if (xcb_connection_has_error(connection)) {
    fprintf(stderr, "Could not connect to X11 server.\n");
    return 1;
  }

  const xcb_setup_t *setup = xcb_get_setup (connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);

  for (i = 0; i < screenNum; ++i) {
    xcb_screen_next(&iter);
  }

  screen = iter.data;
  screen_stats(screen);

  window = xcb_generate_id(connection);
  xcb_create_window(connection, XCB_COPY_FROM_PARENT,
                    window, screen->root,
                    x, y, width, height, border_width,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual,
                    XCB_CW_OVERRIDE_REDIRECT | XCB_CW_EVENT_MASK,
                    mask);
  xcb_map_window(connection, window);

  visual = find_visual(connection, screen->root_visual);
  if (visual == NULL) {
    fprintf(stderr, "Some weird internal error...?!");
    cleanup(connection);
    return 1;
  }

  surface = cairo_xcb_surface_create(connection, window, visual, width, height);
  cr = cairo_create(surface);
  xcb_flush(connection);

  static char* text = "test label";
  labelui.text = text;

  event_handlers_init();
  event_handler_add(UIX_EXPOSE, NULL, event_expose);
  event_handler_add(UIX_MOUSE_MOTION, &progressui, event_motion_notify);
  event_handler_add(UIX_MOUSE_PRESS, &toggleui, event_button_press);

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  loop(connection);
  cleanup(connection);

  return 0;
}
