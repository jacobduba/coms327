#ifndef STACK_H
#define STACK_H

typedef struct stack_node {
  struct stack_node *next;
  int data;
} stack_node;

struct stack {
  struct stack_node *top;
  int size;
};

int stack_init(struct stack *s);
int stack_destroy(struct stack *s);
int stack_push(struct stack *s, int d);
int stack_pop(struct stack *s, int *d);
int stack_top(struct stack *s, int *d);
int stack_size(struct stack *s);
int stack_is_empty(struct stack *s);

#endif
