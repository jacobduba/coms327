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

// Stringification - A macro parameter prefixed with a hash will be turned into
// a string
#define stringify(x) #x

struct lutable_entry {
        char *name;
        int (*func)(int);
};

struct lutable_entry lookup_table[] = {
    {"bar", bar},
    {"baz", baz},
    {"bzz", bzz},
    {"foo", foo},
};

#define luentry(x)                                                             \
        { #x, x }

struct lutable_entru lookup_table[] = {
    luentry(bar),
    luentry(baz),
    luentry(bzz),
    luentry(foo),
};

// Concatenation - Double hash hash concatenates left and right hand sides into
// a new symbol. One of these a macro parameter, which is expanded before
// concatenation. The other can be any string of symbols.
#define concat_pre(x) foo_##x
#define concat_suf(x) x##_bar

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

        bsearch(key, lookup_table, s, n, compare)->func;

        concat_pre(bar);
        concat_suf(bar);

        return 0;
}
