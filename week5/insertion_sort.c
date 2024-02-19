#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// compare is a function that takes to 2 void * params and rtrns ptr to int
// int *compare(const void *v1, const void *v2);

// compare is ptr to function which takes 2 void * params and returns int
// int (*compare)(const void *v1, const void *v2);

void insertion_sort(int *v, int s, int n,
                    int (*compare)(const void *, const void *)) {
        int i, j;
        void *t = malloc(s);
        // use chars for pointer manipulation so a + i is a + i
        char *a = v;

        for (i = 1; i < n; i++) {
                for (memcpy(t, a + s * i, s), j = i - 1;
                     j > -1 && compare(a + (s * j), t) > 0; j--) {
                        memcpy(a + s * (j + 1), a + (s * j), s);
                }
                memcpy(a + s * (j + 1), t, s);
        }
        free(t);
}

int compare_int_reverse(const void *v1, const void *v2) {
        return *((int *)v2) - *((int *)v1);
}

void insertion_sort_int(int *a, int n) {
        int i, j, t;

        for (i = 1; i < n; i++) {
                for (t = a[i], j = i - 1; j > -1 && a[j] > t; j--) {
                        a[j + 1] = a[j];
                }
                a[j + 1] = t;
        }
}

int char_compare(const void *v1, const void *v2) {
        return *((char *)v1) - *((char *)v2);
}

int string_compare(const void *v1, const void *v2) {
        return strcmp(*(const char **)v1, *(const char **)v2);
}

int main(int argc, char *argv[]) {
        int a[] = {5, 3, 7, 1, 9, 2, 8, 4, 6, 0};
        int i;
        char *str[] = {"one", "two",   "three", "four", "five",
                       "six", "seven", "eight", "nine"};

        for (i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
                printf("%d\t", a[i]);
        }
        printf("\n");

        insertion_sort_int(a, sizeof(a) / sizeof(a[0]));

        for (i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
                printf("%d\t", a[i]);
        }
        printf("\n");

        insertion_sort(a, sizeof(a[0]), sizeof(a) / sizeof(a[0]),
                       compare_int_reverse);

        for (i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
                printf("%d\t", a[i]);
        }
        printf("\n");

        for (i = 0; i < sizeof(str) / sizeof(str[0]); i++) {
                printf("%s\t", str[i]);
        }
        printf("\n");

        insertion_sort(a, sizeof(str[0]), sizeof(str) / sizeof(str[0]),
                       string_compare);

        for (i = 0; i < sizeof(str) / sizeof(str[0]); i++) {
                printf("%s\t", str[i]);
        }
        printf("\n");
        return 0;
}
