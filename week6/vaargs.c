#include <math.h>
#include <stdarg.h>
#include <stdio.h>

// A simplified printf that prints only char, int, float, and string
void printf327(const char *format, ...) {
        va_list ap;
        int i;
        char c;
        float f;
        char *s;

        va_start(ap, format);

        while (*format) {
                switch (*format) {
                case 'c':
                        // c = va_arg(ap, char );
                        c = va_arg(ap, int);
                        printf("%c ", c);
                        break;
                case 'd':
                        va_arg(ap, int);
                        printf("%d ", i);
                        break;
                case 'f':
                        // f = va_arg(ap, float);
                        f = va_arg(ap, double);
                        printf("%f ", f);
                        break;
                case 's':
                        s = va_arg(ap, char *);
                        printf("'%s' ", s);
                        break;
                default:
                        fprintf(stderr,
                                "Invalid conversion specifier in format "
                                "string: %s\n",
                                *format);
                }

                format++;
        }
}

int main(int argc, char *argv[]) {
        printf327("csddsdcf", 'F', "Hello", 0, 7, "Ni!", 23, M_PI);
}

// Compilers used to be very expensive because computers were slow.
// So compilers didn't do many checks on the code.
// But computers have gotten a lot faster, so now there are more.
