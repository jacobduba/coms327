#include <cstring>

#include "string327.h"

string327::string327() { str = strdup(""); }

string327::string327(const char *) { str = strdup(s); }

string327::string327(const string327 &s) { str = strdup(s.str); }

string327::~string327() { free(str); }

int string327::length() const { return strlen(str); }

bool string327::operator>(const string327 &s) { return strcmp(str, s.str) > 0; }

bool string327::operator>=(const string327 &s) {
        return strcmp(str, s.str) >= 0;
}

string327 string327::operator+(const string327 &s) {
        string327 r;

        free(r.str);

        r.str = (char *)malloc(strlen(str) + strlen(s.str) + 1);

        strcpy(stpcpy(r.str, str), s.str);

        return r;
}

string327 string327::operator+(const char *s) {
        string327 r;

        free(r.str);

        r.str = (char *)malloc(strlen(str) + strlen(s.str) + 1);

        strcpy(stpcpy(r.str, str), s.str);

        return r;
}

string327 &string327::operator=(const string327 &s) {
        free(str);
        str = strdup(s.str);
        return *this;
}

string327 &string327::operator=(const char *s) {
        free(str);
        str = strdup(s.str);
        return *this;
}

string327 &string327::operator+=(const string327 &s) {
        // WHen rellaoc fails, it returns NULL. If the first parameter and the
        // location for the return value are the same variable, this will lost
        // the pointer in the parameter. In general, you should assign the
        // return value to a temporary, check for success, and then (if desired)
        // overrite the pointer.
        str = (char *)realloc(str, (l = strlen(str)) + strlen(str) +
                                       strlen(s.str) + 1);

        strcpy(str + l, s.str);

        return *this;
}

string327 &string327::operator+=(const char *s) {
        // WHen rellaoc fails, it returns NULL. If the first parameter and the
        // location for the return value are the same variable, this will lost
        // the pointer in the parameter. In general, you should assign the
        // return value to a temporary, check for success, and then (if desired)
        // overrite the pointer.
        str = (char *)realloc(str, (l = strlen(str)) + strlen(str) +
                                       strlen(s.str) + 1);

        strcpy(str + l, s.str);

        return *this;
}

// exam error str.charAt(i)
char &string327::operator[](int i) { return str[i]; }

const char *string327::c_str() const { // const says method will not change this
        return str;
}

std::iostream &operator>>(std::isteam &i, const string327 &s) {
        // Correctly implementing this is complicated
        // You'll do similiar things in 1.07.
        // Here we're going to assume that our input fits in s.str.
        // NOT A SAFE ASSUMPTION!

        return i >> s.str;
}

std::ostream &operator<<(std::ostream &o, const string327 &s) {
        return o << s.c_str();
}
