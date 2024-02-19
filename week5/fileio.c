#include <stdio.h>

typedef enum {
        read_text,      // = 0
        write_text,     // = 1
        read_binary,    // = 2
        write_binary,   // = 3
        invalid_action, // = 4
} action_t;

// Enums do not have to be unique!

int main(int argc, char *argv[]) {
        action_t a = invalid_action;

        struct {
                int i;
                float f;
        } s = {42, 1.616};
        FILE *f;

        if (argc == 2 && argv[1][0] == '-' && argv[1][1] != '\0' &&
            argv[1][2] != '\0' && argv[1][3] == '\0') {
                if (argv[1][1] == 'r') {
                        if (argv[1][2] == 't') {
                                a = read_text;
                        } else if (argv[1][2] == 'b') {
                                a = read_binary;
                        }
                } else if (argv[1][1] == 'w') {
                        if (argv[1][2] == 't') {
                                a = write_text;
                        } else if (argv[1][2] == 'b') {
                                a = write_binary;
                        }
                }
        }

        switch (a) {
        case read_text:
                if (!(f = fopen("textfile", "rb"))) {
                        perror("textfile");
                        return -1;
                }

                s.i = 0;
                s.f = 0;

                fscanf(f, "%d %f\n", &s.i, &s.f);
                printf("%d %f\n", s.i, s.f);
                break;
        case write_text:
                if (!(f = fopen("textfile", "wb"))) {
                        perror("textfile");
                        return -1;
                }

                fprintf(f, "%d %f\n", s.i, s.f);
                break;
        case read_binary:
                break;
        case write_binary:
                break;
        case invalid_action:
                fprintf(stderr, "Usage %s [-rt|-wt|=rb|-wb]\n", argv[0]);
                return -1;
                break;
        }

        return 0;
}
