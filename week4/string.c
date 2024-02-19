#include <stdio.h>
#include <string.h>

size_t strlen327(const char *s) {
        int i;

        for (i = 0; s[i]; i++)
                ;

        return i;
}

int strcmp_idiomatic(const char *s1, const char *s2) {
        while (*s1 && *s1++ == *s2++)
                ;

        return *s1 - *s2;
}

int strcmp327(const char *s1, const char *s2) {
        int i;

        for (i = 0; s1[i] && s2[i] && s1[i] == s2[i]; i++)
                ;

        return s1[i] - s2[i];
}

char *strcpy327(char *dest, const char *src) {
        int i;

        for (i = 0; src[i]; i++) {
                dest[i] = src[i];
        }

        return dest;
}

int main(int argc, char *argv[]) {
        char *s = "Hello World!";
        char a[] = "Hello World!";

        printf("%d\n", strlen327("Hello World!"));
        printf("%d\n", strlen327(s));
        printf("%d\n", strlen327(a));

        // s[0] = 'h'; // No, because s addresses immutable data
        // Not the same as 's is immutable'

        strcpy(a, "Goodbye!");

        s = "Foo!";
        printf("%d\n", strlen327(s));

        a[0] = 'h';
        printf("%s\n", a);

        printf("%s\n", &a[4]);
        printf("%s\n", a + 4);

        return 0;
}

/*
 * Pointer airthmetic -
 * int *a;
 * int i;
 *
 * a[i] == *(a + i)
 * &a[i] == a + i
 * i is an index, not an offste. The compier automatically converted to an an
 * offset for us: We write (a + i), compiler translate to (a + (4 * i))
 *                                                              ^
 *                                                     Because ints are 4 bytes
 */
