#include "cpp-part.h"

#include <iostream>

using namespace std;

void cpp_function(const char *s) { cout << s; }

ostream *get_cout() { return &cout; }

void use_cout(ostream *o, char *s) { *o << s; }
