#ifndef __UIX_EVENT_H__
#define __UIX_EVENT_H__ 1
#include <xcb/xcb_event.h>

#define UIX_NONE         -1
#define UIX_IDLE         0
#define UIX_MOUSE_MOTION XCB_MOTION_NOTIFY
#define UIX_MOUSE_PRESS  XCB_BUTTON_PRESS
#define UIX_KEY_PRESS    XCB_KEY_PRESS
#define UIX_EXPOSE       XCB_EXPOSE

typedef xcb_generic_event_t event_t;
typedef void (*event_handler_t)(void* context, event_t* event);

struct event_handler_item_t {
  void* context;
  event_handler_t cb;
};

struct event_handlers_t {
  struct event_handler_item_t* callbacks;
  int event_type;
  short size;
  short used;
};

void event_handlers_dump(void);
void event_handlers_init(void);
void event_handlers_terminate(void);
void event_handler_add(int type, void* context, event_handler_t handler);
void event_handler_remove(int type, void* context, event_handler_t handler);
void event_handlers_remove_by_context(void* context);
void event_handlers_remove_by_callback(event_handler_t handler);
void event_handlers_unregister_type(int type);
struct event_handlers_t* event_handlers_get_by_type(int type);
void event_handler_execute(int type, event_t* event);
void apply_event_handler(struct event_handler_item_t* event_handler,
                         event_t* event);

#endif // __UIX_EVENT_H__
