#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  compare is a function that that takes two void pointers as parameters and
//  returns pointer to int
// int *compare(const void *v1, const void *v2) {}

// compare is pointer to function two void * params and returns int
// int (*compare)(const void *, const void *);

// void pointers can point to anything, but the compiler
// will not allow you to
// dereference them
void insertion_sort(void *v, int s, int n,
                    int (*compare)(const void *, const void *)) {
        int i, j;
        void *t = malloc(s);
        char *a = v;
        // a + (s * i) <- i becomes an offset
        // a + (s * i) <- the address of element i

        for (i = 1; i < n; i++) {
                for (memcpy(t, a + (s * i), s), j = i - 1;
                     j > -1 && compare(a + (s * j), t) > 0; j--) {
                        memcpy(a + (s * (j + 1)), a + (s * j), s);
                }
                memcpy(a + (s * (j + 1)), t, s);
        }
        free(t);
}

int compare_int_reverse(const void *v1, const void *v2) {
        return *((int *)v2) - *((int *)v1);
}

int string_compare(const void *v1, const void *v2) {
        return strcmp(*(char **)v1, *(char **)v2);
}

int char_compare(const void *v1, const void *v2) {
        return *((char *)v1) - *((char *)v2);
}

void insertion_sort_int(int *a, int n) {
        int i, j, t;

        for (i = 1; i < n; i++) {
                for (t = a[i], j = i - 1; j > -1 && a[j] > t; j--)
                        a[j + 1] = a[j];
        }
        a[j + 1] = t;
}

int main(int argc, char *argv[]) {
        int a[] = {5, 3, 7, 1, 9, 2, 8, 4, 6, 0};
        int i;
        char *str[] = {
            "one", "two",   "three", "four", "five",
            "six", "seven", "eight", "nine", "ten",
        };
        for (i = 0; i < 10; i++)
                printf("%2d,", a[i]);

        printf("\n");
        insertion_sort_int(a, sizeof(a) / sizeof(a[0]));
        for (i = 0; i < 10; i++)
                printf("%d, ", a[i]);
        printf("\n");
        insertion_sort(a, sizeof(a[0]), sizeof(a) / sizeof(a[0]),
                       compare_int_reverse);
        printf("\n");
        for (i = 0; i < 10; i++)
                printf("%d, ", a[i]);
        printf("\n");

        // char *s = "hello world!";
        // insertion_sort(s, 1, strlen(s), char_compare);

        // printf("%s\n", s);

        insertion_sort(str, sizeof(str[0]), sizeof(str) / sizeof(str[0]),
                       string_compare);
        for (i = 0; i < 10; i++)
                printf("%s, ", str[i]);
        printf("\n");

        return 0;
}
