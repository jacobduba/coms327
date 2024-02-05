#ifndef STACK_H
#define STACK_H

typedef struct stack_node stack_node_t;

typedef struct stack_node {
  stack_node_t *next;
  int data;
} stack_node_t;

typedef struct stack {
  stack_node_t *top;
  int size;
} stack_t;

int stack_init(stack_t *s);
int stack_destroy(stack_t *s);
int stack_push(stack_t *s, int d);
int stack_pop(stack_t *s, int *d);
int stack_top(stack_t *s, int *d);
int stack_size(stack_t *s);
int stack_is_empty(stack_t *s);

#endif
