#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uix/event_handler.h>

static struct event_base_t {
  struct event_handlers_t* handlers;
  struct event_handlers_t* empty;
  struct event_handlers_t* end;
  int size;
} *base_event = {0};

void apply_event_handler(struct event_handler_item_t* event_handler,
                                xcb_generic_event_t* event) {
  event_handler->cb(event_handler->context, event);
}

void event_handlers_dump(void) {
  printf("Event handlers info ---------------------------\n");
  printf(" - size of event_handlers_t: %zu\n", sizeof(struct event_handlers_t));
  printf(" - size of event_handler_item_t: %zu\n", sizeof(struct event_handler_item_t));
  printf(" - size: %d\n", base_event->size);
  printf(" - address: %p\n", base_event->handlers);
  printf(" - first empty: %p\n", base_event->empty);
  printf(" - end: %p\n", base_event->end);
  printf("-----------------------------------------------\n");

  struct event_handlers_t* es = base_event->handlers;
  for (int i = 0; i < base_event->size; i++) {
    struct event_handlers_t* s = es + i;
    printf(" - Handler %p type(%d) used: %d/%d\n", s, s->event_type, s->size, s->used);
  }
  printf("\n");
}

void event_handlers_init(void) {
  if (!base_event)
    base_event = calloc(1, sizeof(struct event_base_t));

  if (!base_event->handlers) {
    base_event->size = 8;
    base_event->handlers = calloc(8, sizeof(struct event_handlers_t));
    base_event->empty = base_event->handlers;
    base_event->end = base_event->handlers + (base_event->size - 1);
  }

  struct event_handlers_t* es = base_event->handlers;
  for (int i = 0; i < base_event->size; i++) {
    struct event_handlers_t* s = es + i;
    s->event_type = UIX_NONE;
  }

  event_handlers_dump();
}

void event_handlers_terminate(void) {
  if (base_event) {
    if (base_event->handlers) {
      struct event_handlers_t* es = base_event->handlers;
      for (int i = 0; i < base_event->size; i++) {
        free(es->callbacks);
        es++;
      }
      free(base_event->handlers);
    }

    free(base_event);
  }
}

static inline void event_handlers_set_callback(struct event_handler_item_t* i,
                                               void* context, event_handler_t handler) {
  i->context = context;
  i->cb = handler;
}

static inline void event_handlers_unset_callback(struct event_handler_item_t* i) {
  i->context = 0;
  i->cb = 0;
}

static inline int event_handler_item_is_empty(struct event_handler_item_t* i) {
  return !i->context && !i->cb;
}

static inline struct event_handler_item_t* find_empty_handler(struct event_handler_item_t* xs, int size) {
  struct event_handler_item_t* item;
  for (int i = 0; i < size; i++) {
    item = xs + i;
    if (event_handler_item_is_empty(item)) {
      break;
    }
  }
  return item;
}

struct event_handlers_t* event_handlers_get_by_type(int type) {
  struct event_handlers_t* s = base_event->handlers;
  int found = 0;
  for (int i = 0;
       s < (base_event->end + 1);
       i++, s = base_event->handlers + i) {
    found = s->event_type == type;
    if (found)
      break;
  }
  return found ? s : 0;
}

void event_handler_add(int type, void* context, event_handler_t handler) {
  struct event_handlers_t* s;
  // find the slot for a given type.
  s = event_handlers_get_by_type(type);
  // if such a slot doesn't exists, create using the empty slot.
  //   if there are no more slots, resize.
  //   if there is slots available, use one.
  if (!s) {
    if (base_event->empty) {
      s = base_event->empty;
      base_event->empty++;
    } else {
      printf("require more space.\n");
    }
  }
  // otherwise, check if the slot was initialized
  //   if already initialized, just append a new item.
  //   otherwise, initialize a new slot.
  if (s->size == 0) {
    // make the proper initialization of the callbacks if needed.
    s->callbacks = calloc(8, sizeof(struct event_handler_item_t));
    s->event_type = type;
    s->size = 8;
  }
  // find an empty slot an set the callback.
  struct event_handler_item_t* i = find_empty_handler(s->callbacks, s->size);
  event_handlers_set_callback(i, context, handler);
  s->used++;

  event_handlers_dump();
}

static inline void event_handler_item_dump(struct event_handler_item_t* i) {
  printf("Handler context: %p function: %p\n", i->context, i->cb);
}

void event_handlers_remove_by_context(void* context) {
  struct event_handlers_t* s = base_event->handlers;
  for (int i = 0;
       s < (base_event->end + 1);
       i++, s = base_event->handlers + i) {

    for (int j = 0; j < s->size; j++) {
      struct event_handler_item_t* i = s->callbacks + j;
      event_handler_item_dump(i);
      if (i->context == context && i->cb) {
        printf("Removing handler %p at %d.\n", i, (int)(i - s->callbacks));
        event_handlers_unset_callback(i);
        s->used--;
      }
    }
  }
  event_handlers_dump();
}

void event_handlers_remove_by_callback(event_handler_t handler) {
  struct event_handlers_t* s = base_event->handlers;
  for (int i = 0;
       s < (base_event->end + 1);
       i++, s = base_event->handlers + i) {

    for (int j = 0; j < s->size; j++) {
      struct event_handler_item_t* i = s->callbacks + j;
      event_handler_item_dump(i);
      if (i->cb == handler) {
        printf("Removing handler %p at %d.\n", i, (int)(i - s->callbacks));
        event_handlers_unset_callback(i);
        s->used--;
      }
    }
  }

  event_handlers_dump();
}

void event_handlers_unregister_type(int type) {
  struct event_handlers_t* s = base_event->handlers;
  for (int i = 0;
       s < (base_event->end + 1);
       i++, s = base_event->handlers + i) {
    if (s->event_type == type) {
      memset(s->callbacks, 0, sizeof(struct event_handlers_t) * s->size);
      s->event_type = UIX_NONE;
      break;
    }
  }

  event_handlers_dump();
}

void event_handler_execute(int type, event_t* event) {
  struct event_handlers_t* handlers = event_handlers_get_by_type(type);
  if (!handlers)
    return;

  struct event_handler_item_t* s;
  for (s = handlers->callbacks; s; s++) {
    if (!s->cb) break;
    apply_event_handler(s, event);
  }
}
