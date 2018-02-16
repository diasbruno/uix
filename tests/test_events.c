#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <uix/definitions.h>
#include <uix/event_handler.h>

void event_handler1(void* UNUSED(context), event_t* UNUSED(event)) {}
void event_handler2(void* UNUSED(context), event_t* UNUSED(event)) {}

int main(int UNUSED(argc), char* UNUSED(argv[])) {
  event_handlers_init();

  static struct a_t { int b; } x = { .b = 1 };

  event_handler_add(1, 0, event_handler1);
  event_handler_add(1, &x, event_handler2);

  struct event_handlers_t* s = event_handlers_get_by_type(1);

  assert(s->event_type == 1);
  assert(s->used == 2);

  event_handlers_remove_by_callback(event_handler2);
  s = event_handlers_get_by_type(1);
  assert(s->event_type == 1);
  assert(s->used == 1);

  event_handler_add(1, 0, event_handler2);
  event_handlers_remove_by_context(0);

  s = event_handlers_get_by_type(1);
  assert(s->used == 0);

  event_handlers_unregister_type(1);
  s = event_handlers_get_by_type(1);
  assert(!s);

  event_handler_add(1, 0, event_handler1);
  event_handler_add(1, &x, event_handler2);
  event_handler_add(2, &x, event_handler1);

  event_handlers_remove_by_context(&x);

  s = event_handlers_get_by_type(1);
  assert(s->used == 1);
  s = event_handlers_get_by_type(2);
  assert(s->used == 0);

  event_handlers_terminate();

  return 0;
}
