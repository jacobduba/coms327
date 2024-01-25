#include <stdio.h>

// Records in C are called 'struct'
struct foo {
  int i;
  float f;
  char *s;
};

void swap_wrong(int x, int y) {
  int tmp;

  printf("x = %d, y = %d\n", x, y);

  tmp = x;
  x = y;
  y = tmp;

  printf("x = %d, y = %d\n", x, y);
}

// A pointer is a variable that holds an address.

void swap(int *x, int *y) {
  int tmp;

  // * is the dereference operator.
  // It fetches the value addressd by the pointer.
  printf("x = %d, y = %d\n", *x, *y);

  tmp = *x;
  *x = *y;
  *y = tmp;

  printf("x = %d, y = %d\n", *x, *y);
}

// C ONLY has pass by value.
// Here we are passing the values of the addresses in.

void swap_also_wrong(int *x, int *y) {
  int *tmp;

  printf("x = %d, y = %d\n", *x, *y);

  tmp = x;
  x = y;
  y = tmp;

  printf("x = %d, y = %d\n", *x, *y);
}

// void populate_foo(struct foo, int i, float d, char *s) {
//   f.i = i;
//   f.f = d;
//   f.s = s;
// }

// How to read variable declarations in C: Start with the variable name.
//                                         Go right when we can.
//                                         Go left when we must.
int main(int argc, char *argv[]) {
  int i, j;
  struct foo f;

  i = 0;
  j = 1;

  printf("i = %d, j = %d\n", i, j);

  // & is the address operator.
  // It takes the address of the variable it is applied to.
  swap(&i, &j);

  printf("i = %d, j = %d\n", i, j);

  f.i = 0;
  f.f = 3.14;
  f.s = "Hello world";

  // populate_foo(f, 0, 3.14, "Hello World!")

  printf("%d %f %s\n", f.i, f.f, f.s);

  return 0;
}
