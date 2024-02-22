// Value-type
#define FOO 42

// Function-type macro
#define min(x, y) (x < y ? x : y)
// Always parens around function-type macros

#define min3(x, y, z) (min(x, min(y, z)))
#define min4(x, y, z, w) (min(x, min(y, min(z, w))))

// This only works in C99 or later
// basic blocks have a value, equal to last line of block.
#define max(x, y)                                                              \
        ({                                                                     \
                typeof(x) _x = (x);                                            \
                typeof(x) _y = (y);                                            \
                _x < _y ? _x : _y;                                             \
        })

#define foo(x)                                                                 \
        {                                                                      \
                dddd;                                                          \
                ffff;                                                          \
        }

int main(int argc, char *argv[]) {
        printf("%d\n", FOO);

        min(a, b) + 5;

        if (x)
                foo(x);

        min(function_with_side_effect(), really_expensive_function());
        // (function_with_side_effect() < really_expensive_function() ?
        //      function_with_side_effect() : really_expensive_function());

        min3(a, b, c);
        min4(a, b, c, d);

        return 0;
}
