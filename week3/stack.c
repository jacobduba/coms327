#include "stack.h"
#include <stdlib.h>

int stack_init(stack_t *s) {
  // NUL is a special value for NULL pointers. It is defined (void *) 0
  s->top = NULL;
  s->size = 0;

  return 0;
}

int stack_destroy(stack_t *s) {
  // Don't do this way. Ineffiecent.
  // while (!stack_pop(s));

  stack_node_t *n;
  for (n = s->top; n; n = s->top) {
    s->top = s->top->next;
    free(n);
  }

  // This is not necessary; here because I am completionist.
  s->size = 0;

  return 0;
}

int stack_push(stack_t *s, int d) {
  stack_node_t *n;

  // sizeof is a static (i.e., compile time) operator that returns the number of
  // bytes required to store the expression given as its parameter.
  if (!(n = malloc(sizeof(*n)))) {
    return 1;
  }

  // You need to use malloc when you allocate memory on the heap. You use the
  // heap when you do not know the size of your data. Why?

  n->data = d;
  n->next = s->top;
  s->top = n;
  s->size++;

  return 0;
}

int stack_pop(stack_t *s, int *d) {
  stack_node_t *n;

  if (!s->top) {
    return 1;
  }

  *d = s->top->data;
  n = s->top;
  s->top = s->top->next;
  free(n);
  s->size--;

  return 0;
}

int stack_top(stack_t *s, int *d) {
  if (!s->top) {
    return 1;
  }

  *d = s->top->data;

  return 0;
}

int stack_size(stack_t *s) { return s->size; }

int stack_is_empty(stack_t *s) { return !s->size; }
