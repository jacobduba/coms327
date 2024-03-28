#ifndef STRING327_H
#define STRING327_H

#include <iostream>

/*
string327 s, t;

if (strcmp(s.c_str(), t.c_str()) >0)
  ...

if (s.compare(t) > 0)
  ...

if (s > t) // if (s.operator>(t))
  ...

cout << s + t; // operator<<(cout, s + t)
cout << s + "foo";

(cout << s) << t; // operator<<(operator<<(cout, s), t)

s = t;
s = "foo";

string327 u;

(s = t).mutate();

(s += t) += u;

cout << s[i];

s[i] = 'f';
s.operator[](i) = 'f';

*func_that_returns_pointer() = value;
*/

class string327 {
 private:
  char *str;
 public:
  string327();
  string327(const char *);
  string327(const string327 &);
  ~string327();
  int length() const;
  bool operator>(const string327 &) const;
  bool operator>=(const string327 &) const;
  bool operator<(const string327 &) const;
  bool operator<=(const string327 &) const;
  bool operator==(const string327 &) const;
  bool operator!=(const string327 &) const;
  string327 operator+(const string327 &) const;
  string327 operator+(const char *) const;
  string327 &operator=(const string327 &);
  string327 &operator=(const char *);
  string327 &operator+=(const string327 &);
  string327 &operator+=(const char *);
  char &operator[](int) const;
  const char *c_str() const;

  // Friends have access to our privates
  friend std::istream &operator>>(std::istream &, string327 &);
};

std::ostream &operator<<(std::ostream &, const string327 &);

#endif
