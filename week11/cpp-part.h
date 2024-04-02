#ifndef CPP_PART_H
#define CPP_PART_H

#ifdef __cplusplus
#include <iostream>
using namespace std;

extern "C" {
#else
// This is C code - defines ostream as a pointer to void (an opaque handle)
typedef void ostream;
#endif

void cpp_function(const char *);
ostream *get_cout();
void use_cout(ostream *, const char *);

#ifdef __cplusplus
}
#endif

#endif
