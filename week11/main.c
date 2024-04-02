#include "cpp-part.h"

int main(int argc, char *argv[]) {
        ostream *o;

        cpp_function("Hello World!\n");

        o = get_cout();

        use_cout(o, "Goodbye cruel world.");

        return 0;
}
