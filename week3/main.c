#include <stdio.h>

#include "stack.h"

int main(int argc, char *argv[]) {
  // Don't
  // stack_t *s;
  // s = malloc(sizeof(*s));

  // Instead
  stack_t s;
  int i;
  stack_init(&s);

  for (i = 0; i < 10; i++) {
    stack_push(&s, i);
  }

  stack_top(&s, &i);
  printf("Stack size: %d, i currently: %d\n", stack_size(&s), i);

  while (!stack_is_empty(&s)) {

    if (stack_pop(&s, &i)) {
      // Runtime opens 3 files before starting main, stdin, stdout, stderr
      fprintf(stderr, "Attempt to pop empty stack\n");

      return 1;
    }

    printf("Popped %d\n", i);
  }

  stack_destroy(&s);

  return 0;
}
